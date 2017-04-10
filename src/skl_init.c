
/***************************************************************************
 *   Copyright (C) 2008 by Andrea Ghersi                                   *
 *   ghangenit@users.sourceforge.net                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

# define _PROTO_PRIVATE_ERROR
# define _PROTO_PRIVATE_RAND
# define _PROTO_PRIVATE_DBG
# define _PROTO_PUBLIC_INIT
# define _PROTO_PRIVATE_LC

# include "skl_priv.h"


# define setinfo(l,x,max,dsf,cpf,rng)                   \
{                                                       \
    uint i = 0;                                         \
                                                        \
    l->magic = MYSKL_LIST_MAGIC;                        \
    IFRAND( l->rng_item = rng; )                        \
    l->maxlevel = x->sz = max;                          \
    l->header = l->tail = x;                            \
    l->curlevel = 1;                                    \
    l->nodenum  = 0;                                    \
    l->dst = dsf;                                       \
    l->cmp = cpf;                                       \
                                                        \
    for ( ; i < max; i++ )                              \
    {                                                   \
        l->header->next[ i ] = NIL ( l );               \
        IFDOUBLE ( l->header->prev[ i ] = NIL ( l ); )  \
        IFNORAND ( l->nodenumelem[ i ] = 0; )           \
    }                                                   \
                                                        \
    RW_INIT ( l );                                      \
}



/** ******************************************************************
 *                                                                   *
 * Genera una sklist vuota con un numero massimo di livelli uguale a *
 * <max> { lev 1, lev 2,........., lev max }.                        *
 *                                                                   *
 * La skiplist vuota e' rappresentata da un header ( uguale in tutto *
 * agli elementi effettivi della lista ) che contiene, tra le altre  *
 * cose, un array di puntatori a elementi della lista di dim. <max>  *
 *                                                                   *
 ** ******************************************************************/

static MySKL_t init (
    uint max,
    int  ( * compare )( const void *, const void * ),
    void ( * dstnode )( MySKL_n ),
    MySKL_e * err
)
{
    Rand_t rng = NULL;
    MySKL_t l;
    MySKL_n x;
    size_t dim;

    /* debug call if enabled */
    X_ENTER ( THIS_IS_ME );

    /* assume that something abaout RNG goes wrong */
    if ( err ) *err = MYSKL_RANDGEN_ERROR;

    IFRAND(
        rng = newRNG_withSeed ( SEED_VALUE );
        EXP_BC_X_RETV ( rng, NULL );
    )

    /* assume that something about memory goes wrong */
    if ( err ) *err = MYSKL_MEM_EXHAUSTED;

    dim = X_ALLOC ( sizeof( MySKL_ls ), l );
    ON_ERROR_FREE_MEM ( l, dim, rng );

    /* header field: NIL element */

    dim = X_ALLOC ( sizeof( MySKL_ns ), x );
    ON_ERROR_FREE_MEM ( x, dim, l, rng );

    /* array of next node pointers */

    dim = X_ALLOC ( max * sizeof( MySKL_n ), x->next );
    ON_ERROR_FREE_MEM ( x->next, dim, x, l, rng );

    IFDOUBLE(
        dim = X_ALLOC ( max * sizeof( MySKL_n ), x->prev );
        ON_ERROR_FREE_MEM( x->prev, dim, x->next, x, l, rng);
    )

    if ( err ) *err = MYSKL_STATUS_OK;
    setinfo( l, x, max, dstnode, compare, rng );

    /* debug call if enabled */
    X_RETURN ( l );
}



/** ***********************************************************
 ** ***********************************************************
 +                                                            +
 +                                                            +
 +              FUNZIONI/PROCEDURE PUBBLICHE                  +
 +                                                            +
 +                                                            +
 ** ***********************************************************
 ** ***********************************************************/

MySKL_t MySKLinit (
    uint maxlev,
    int  ( * compare )( const void *, const void * ),
    void ( * dstnode )( MySKL_n ),
    MySKL_e * err
)
{
    MySKL_t l = NULL;
    X_ENTER ( THIS_IS_ME );

    /* since function could may be stopped before calling init */
    if ( err ) *err = MYSKL_ERROR;

    EXP_BC_RETV ( maxlev > 0 && maxlev <= MYSKL_MAXLEVELS, NULL );
    EXP_BC_RETV ( compare != NULL, NULL );

    l = init ( maxlev, compare, dstnode, err );
    X_RETURN ( l );
}
