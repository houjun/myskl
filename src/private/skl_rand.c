
/*
 * A C-program for MT19937, with initialization improved 2002/1/26.
 * Coded by Takuji Nishimura and Makoto Matsumoto.
 *
 * Before using, initialize the state by using init_genrand(seed)
 * or init_by_array(init_key, key_length).
 *
 * Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 *  3. The names of its contributors may not be used to endorse or promote
 *     products derived from this software without specific prior written
 *     permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * Any feedback is very welcome.
 * http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
 * email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
 *
 * This code was adapted to glib by Sebastian Wilhelmi and slightly further
 * modified ( just a bunch of things ) by me to fulfill this library needs
 */

# define _PROTO_PRIVATE_ERROR
# define _PROTO_PRIVATE_RAND
# define _PROTO_PRIVATE_DBG

# include "skl_priv.h"


static void rand_set_seed_array ( sklRand * rand_, const uint32 *, uint );
static void rand_set_seed ( sklRand *, uint32 );


/**
 * rand_set_seed:
 * @rand_: a #sklRand.
 * @seed: a value to reinitialize the random number generator.
 *
 * Sets the seed for the random number generator #sklRand to @seed.
 **/

static void rand_set_seed ( sklRand * randi, uint32 seed )
{
    /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
    /* In the previous version (see above), MSBs of the    */
    /* seed affect only MSBs of the array mt[].            */

    randi->mt[ 0 ] = seed;
    for ( randi->mti = 1; randi->mti < RAND_N; randi->mti++ )
        randi->mt[ randi->mti ] = 1812433253UL * (
            randi->mt[ randi->mti - 1 ] ^
            ( randi->mt[ randi->mti - 1 ] >> 30 )
            ) + randi->mti;
}



/**
 * rand_set_seed_array:
 * @rand_: a #sklRand.
 * @seed: array to initialize with
 * @seed_length: length of array
 *
 * Initializes the random number generator by an array of
 * longs.  Array can be of arbitrary size, though only the
 * first 624 values are taken.  This function is useful
 * if you have many low entropy seeds, or if you require more then
 * 32bits of actual entropy for your application.
 **/

static void rand_set_seed_array ( sklRand * randi,
    const uint32 * seed, uint seed_length )
{
    uint i, j, k;

    rand_set_seed ( randi, 19650218UL );

    i = 1;
    j = 0;
    k = ( RAND_N > seed_length ? RAND_N : seed_length );
    for ( ; k; k-- )
    {
        randi->mt[ i ] =
            (
                randi->mt[ i ] ^
                (
                    ( randi->mt[ i - 1 ] ^
                      ( randi->mt[ i - 1 ] >> 30 )
                    ) * 1664525UL )
            ) + seed[ j ] + j; /* non linear */

        /* for WORDSIZE > 32 machines */
        randi->mt[ i ] &= 0xffffffffUL;
        i++;
        j++;
        if ( i >= RAND_N )
        {
            randi->mt[ 0 ] = randi->mt[ RAND_N - 1 ];
            i = 1;
        }
        if ( j >= seed_length )
            j = 0;
    }
    for ( k = RAND_N - 1; k; k-- )
    {
        randi->mt[ i ] =
            (
                randi->mt[ i ] ^
                ( ( randi->mt[ i - 1 ] ^
                    ( randi->mt[ i - 1 ] >> 30 )
                    ) * 1566083941UL
                )
            ) - i; /* non linear */

        /* for WORDSIZE > 32 machines */
        randi->mt[ i ] &= 0xffffffffUL;
        i++;
        if ( i >= RAND_N )
        {
            randi->mt[ 0 ] = randi->mt[ RAND_N - 1 ];
            i = 1;
        }
    }

    /* MSB is 1; assuring non-zero initial array */
    randi->mt[ 0 ] = 0x80000000UL;
}



/**
 * rand_int:
 * @rand_: a #sklRand.
 *
 * Returns the next random #uint32 from @rand_ equally distributed over
 * the range [0..2^32-1].
 * Return value: A random number.
 **/

static uint32 rand_int ( sklRand * randi )
{
    uint32 y;
    static const uint32 mag01[ 2 ] = { 0x0, MATRIX_A };

    /* mag01[x] = x * MATRIX_A  for x=0,1 */

    if ( randi->mti >= RAND_N ) { /* generate N words at one time */
        int kk;

        for ( kk = 0; kk < RAND_N - M; kk++ ) {
            y = ( randi->mt[ kk ] & UPPER_MASK ) |
                ( randi->mt[ kk + 1 ] & LOWER_MASK );

            randi->mt[ kk ] = randi->mt[ kk + M ] ^
                              ( y >> 1 ) ^
                              mag01[ y & 0x1 ];
        }
        for ( ; kk < RAND_N - 1; kk++ ) {
            y = ( randi->mt[ kk ] & UPPER_MASK ) |
                ( randi->mt[ kk + 1 ] & LOWER_MASK );

            randi->mt[ kk ] = randi->mt[ kk + ( M - RAND_N ) ] ^
                              ( y >> 1 ) ^
                              mag01[ y & 0x1 ];
        }
        y = ( randi->mt[ RAND_N - 1 ] & UPPER_MASK ) |
            ( randi->mt[ 0 ] & LOWER_MASK );

        randi->mt[ RAND_N - 1 ] = randi->mt[ M - 1 ] ^
                                  ( y >> 1 ) ^
                                  mag01[ y & 0x1 ];

        randi->mti = 0;
    }

    y  = randi->mt[ randi->mti++ ];
    y ^= TEMPERING_SHIFT_U ( y );
    y ^= TEMPERING_SHIFT_S ( y ) & TEMPERING_MASK_B;
    y ^= TEMPERING_SHIFT_T ( y ) & TEMPERING_MASK_C;
    y ^= TEMPERING_SHIFT_L ( y );

    return y;
}



/* ======================================================================== */
/*                                                                          */
/* PRIVATE FUNCTIONS VISIBLE - THEREFORE USABLE - OUTSIDE THIS FILE         */
/*                                                                          */
/* ======================================================================== */

/**
 * newRNG_withSeed:
 * @seed: a value to initialize the random number generator.
 *
 * Creates a new random number generator initialized with @seed.
 * Return value: the new #sklRand.
 **/

sklRand * newRNG_withSeed ( uint32 seed )
{
    sklRand * randi;
    size_t dim;

    /* debug call if enabled */
    X_ENTER ( THIS_IS_ME );

    dim = X_ALLOC ( sizeof( sklRand ), randi );
    MEM_BC_X_RETV ( randi, dim, NULL );

    rand_set_seed ( randi, seed );
    X_RETURN ( randi );
}



/**
 * newRNG_withSeedArray:
 * @seed: an array of seeds to initialize the random number generator
 * @seed_length: an array of seeds to initialize the random number
 * generator.
 *
 * Creates a new random number generator initialized with @seed.
 * Return value: the new #sklRand.
 **/

sklRand * newRNG_withSeedArray ( uint32 * seed,
    uint seed_length )
{
    sklRand * randi;
    size_t dim;

    X_ENTER ( THIS_IS_ME );
    EXP_BC_X_RETV ( seed_length >= 1, NULL );

    dim = X_ALLOC ( sizeof( sklRand ), randi );
    MEM_BC_X_RETV ( randi, dim, NULL );

    rand_set_seed_array ( randi, seed, seed_length );
    X_RETURN ( randi );
}



/**
 * RNGyeldInt32:
 * @rand_: a #sklRand.
 *
 * Returns the next random #uint32 from @rand_ equally distributed over
 * the range [0..2^32-1].
 * Return value: A random number.
 **/

uint32 RNGyieldInteger32 ( sklRand * randi )
{
    uint32 val;

    X_ENTER ( THIS_IS_ME );
    EXP_BC_X_RETV ( randi != NULL, 0 );

    val = rand_int ( randi );
    X_RETURN ( val );
}



/* transform [0..2^32] -> [0..1] */
#define RAND_DOUBLE_TRANSFORM  2.3283064365386962890625e-10


/**
 * rand_double:
 * @rand_: a #sklRand.
 *
 * Returns the next random #double from @rand_ equally distributed over
 * the range [0..1).
 *
 * Return value: A random number.
 **/
double RNGyieldDouble ( sklRand * randi )
{
    /* We set all 52 bits after the point for this, not only the first
     * 32. Thats why we need two calls to rand_int */
    double retval;

    X_ENTER ( THIS_IS_ME );
    EXP_BC_X_RETV ( randi != NULL, -1 );

    retval = rand_int ( randi ) * RAND_DOUBLE_TRANSFORM;
    retval = ( retval + rand_int ( randi ) ) * RAND_DOUBLE_TRANSFORM;

    /* The following might happen due to very bad rounding luck, but
     * actually this should be more than rare, we just try again then */
    if ( retval >= 1.0 )
        retval = RNGyieldDouble ( randi );

    /* debug call if enabled */
    X_RETURN ( retval );
}



/**
 * rand_double_range:
 * @rand_: a #sklRand.
 * @begin: lower closed bound of the interval.
 * @end: upper open bound of the interval.
 *
 * Returns the next random #double from @rand_ equally distributed over
 * the range [@begin..@end).
 *
 * Return value: A random number.
 **/

double RNGyieldDoubleRange ( sklRand * randi, double begin,
    double end )
{
    double retval;

    X_ENTER ( THIS_IS_ME );
    EXP_BC_X_RETV ( randi != NULL, begin - 1 );

    retval = RNGyieldDouble ( randi ) * ( end - begin ) + begin;
    X_RETURN ( retval );
}



/**
 * rand_free:
 * @rand_: a #sklRand.
 *
 * Frees the memory allocated for the #sklRand.
 **/

void RNGfree ( sklRand * randi )
{
    X_ENTER ( THIS_IS_ME );
    X_FREE ( randi );
    X_RETURN_VOID ();
}
