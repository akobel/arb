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

    Copyright (C) 2015 Fredrik Johansson

******************************************************************************/

#include "acb_poly.h"

void
_acb_poly_cot_pi_series(acb_ptr g, acb_srcptr h, long hlen, long len, long prec)
{
    hlen = FLINT_MIN(hlen, len);

    if (hlen == 1)
    {
        acb_cot_pi(g, h, prec);
        _acb_vec_zero(g + 1, len - 1);
    }
    else
    {
        acb_ptr t, u;

        t = _acb_vec_init(len);
        u = _acb_vec_init(len);

        if (arf_cmpabs_2exp_si(arb_midref(acb_imagref(h)), 0) < 0)
        {
            _acb_poly_sin_cos_pi_series(t, u, h, hlen, len, prec);
            _acb_poly_div_series(g, u, len, t, len, len, prec);
        }
        else
        {
            _acb_vec_scalar_mul_2exp_si(t, h, hlen, 1);

            if (arf_sgn(arb_midref(acb_imagref(h))) > 0)
            {
                acb_const_pi(u, prec);
                acb_mul_onei(u, u);
                _acb_vec_scalar_mul(t, t, hlen, u, prec);
                _acb_poly_exp_series(t, t, hlen, len, prec);
                acb_sub_ui(u, t, 1, prec);
                _acb_vec_set(u + 1, t + 1, len - 1);
                _acb_poly_div_series(g, t, len, u, len, len, prec);
                _acb_vec_scalar_mul_2exp_si(g, g, len, 1);
                acb_sub_ui(g, g, 1, prec);
                _acb_vec_scalar_mul_onei(g, g, len);
            }
            else
            {
                acb_const_pi(u, prec);
                acb_div_onei(u, u);
                _acb_vec_scalar_mul(t, t, hlen, u, prec);
                _acb_poly_exp_series(t, t, hlen, len, prec);
                acb_sub_ui(u, t, 1, prec);
                _acb_vec_set(u + 1, t + 1, len - 1);
                _acb_poly_div_series(g, t, len, u, len, len, prec);
                _acb_vec_scalar_mul_2exp_si(g, g, len, 1);
                acb_sub_ui(g, g, 1, prec);
                _acb_vec_scalar_mul_onei(g, g, len);
                _acb_vec_neg(g, g, len);
            }
        }

        _acb_vec_clear(t, len);
        _acb_vec_clear(u, len);
    }
}

void
acb_poly_cot_pi_series(acb_poly_t res, const acb_poly_t f, long len, long prec)
{
    acb_poly_fit_length(res, len);

    if (f->length == 0 || len == 0)
        _acb_vec_indeterminate(res->coeffs, len);
    else
        _acb_poly_cot_pi_series(res->coeffs, f->coeffs, f->length, len, prec);

    _acb_poly_set_length(res, len);
    _acb_poly_normalise(res);
}

