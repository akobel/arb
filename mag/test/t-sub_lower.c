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

    Copyright (C) 2014 Fredrik Johansson

******************************************************************************/

#include "mag.h"

int main()
{
    long iter;
    flint_rand_t state;

    printf("sub_lower....");
    fflush(stdout);

    flint_randinit(state);

    for (iter = 0; iter < 100000; iter++)
    {
        fmpr_t x, y, z, z2, w;
        mag_t xb, yb, zb;

        fmpr_init(x);
        fmpr_init(y);
        fmpr_init(z);
        fmpr_init(z2);
        fmpr_init(w);

        mag_init(xb);
        mag_init(yb);
        mag_init(zb);

        mag_randtest_special(xb, state, 100);
        mag_randtest_special(yb, state, 100);

        mag_get_fmpr(x, xb);
        mag_get_fmpr(y, yb);

        fmpr_sub(z2, x, y, 100, FMPR_RND_DOWN);
        if (fmpr_sgn(z2) < 0)
            fmpr_zero(z2);

        fmpr_mul_ui(z, z2, 1023, MAG_BITS, FMPR_RND_DOWN);
        fmpr_mul_2exp_si(z, z, -10);

        mag_sub_lower(zb, xb, yb);
        mag_get_fmpr(w, zb);

        MAG_CHECK_BITS(xb)
        MAG_CHECK_BITS(yb)
        MAG_CHECK_BITS(zb)

        if (!(fmpr_cmpabs(z, w) <= 0 && fmpr_cmpabs(w, z2) <= 0))
        {
            printf("FAIL\n\n");
            printf("x = "); fmpr_print(x); printf("\n\n");
            printf("y = "); fmpr_print(y); printf("\n\n");
            printf("z = "); fmpr_print(z); printf("\n\n");
            printf("w = "); fmpr_print(w); printf("\n\n");
            abort();
        }

        if (n_randint(state, 2))
        {
            mag_sub_lower(xb, xb, yb);

            if (!mag_equal(xb, zb))
            {
                printf("FAIL (aliasing 1)\n\n");
                abort();
            }
        }
        else
        {
            mag_sub_lower(yb, xb, yb);

            if (!mag_equal(yb, zb))
            {
                printf("FAIL (aliasing 2)\n\n");
                abort();
            }
        }

        fmpr_clear(x);
        fmpr_clear(y);
        fmpr_clear(z);
        fmpr_clear(z2);
        fmpr_clear(w);

        mag_clear(xb);
        mag_clear(yb);
        mag_clear(zb);
    }

    flint_randclear(state);
    flint_cleanup();
    printf("PASS\n");
    return EXIT_SUCCESS;
}

