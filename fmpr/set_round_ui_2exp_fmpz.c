/*=============================================================================

    This file is part of ARB.

    ARB is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    ARB is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ARB; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2013 Fredrik Johansson

******************************************************************************/

#include "fmpr.h"

long
fmpr_set_round_ui_2exp_fmpz(fmpr_t z,
        mp_limb_t lo, const fmpz_t exp, int negative,
        long prec, fmpr_rnd_t rnd)
{
    long lead, trail, bc, shift, shift2, ret;

    shift = 0;

    if ((lo & 1) == 0)
    {
        if (lo == 0)
        {
            fmpr_zero(z);
            return FMPR_RESULT_EXACT;
        }

        count_trailing_zeros(trail, lo);
        lo >>= trail;
        shift = trail;
    }

    count_leading_zeros(lead, lo);
    bc = FLINT_BITS - lead;

    ret = FMPR_RESULT_EXACT;
    if (bc > prec)
    {
        shift2 = bc - prec;
        lo = (lo >> shift2) + rounds_up(rnd, negative);
        count_trailing_zeros(trail, lo);
        lo >>= trail;
        shift += shift2;
        shift += trail;
        ret = trail;
    }

    if (!negative)
        fmpz_set_ui(fmpr_manref(z), lo);
    else
        fmpz_neg_ui(fmpr_manref(z), lo);

    fmpz_add_si_inline(fmpr_expref(z), exp, shift);
    return ret;
}
