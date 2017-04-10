
# ifndef __RAND_H__
# define __RAND_H__


/* ==================
 * Period parameters
 * ==================
 */

/* the biggest integer value returnable from RNGyieldInteger32() */
# define RAND_MAX_N 0xffffffff   /* decimal 4294967295 */

#define RAND_N 624
#define M 397
#define MATRIX_A 0x9908b0df      /* constant vector a */
#define UPPER_MASK 0x80000000    /* most significant w-r bits */
#define LOWER_MASK 0x7fffffff    /* least significant r bits */

/* Tempering parameters */

#define TEMPERING_MASK_B 0x9d2c5680
#define TEMPERING_MASK_C 0xefc60000
#define TEMPERING_SHIFT_U(y)  (y >> 11)
#define TEMPERING_SHIFT_S(y)  (y << 7)
#define TEMPERING_SHIFT_T(y)  (y << 15)
#define TEMPERING_SHIFT_L(y)  (y >> 18)

#define SEED_VALUE 0x1571

typedef struct _sklRand sklRand;
struct _sklRand
{
   /* the array for the state vector  */
   uint32 mt[ RAND_N ];
   uint mti;
};

extern sklRand * newRNG_withSeedArray (
   uint32 * seed, uint seed_length );

extern sklRand * newRNG_withSeed (
   uint32 seed );

extern uint32 RNGyieldInteger32 (
   sklRand* randi );

extern double RNGyieldDouble (
   sklRand* randi );

extern double RNGyieldDoubleRange (
   sklRand * randi, double begin,
   double end );

extern void RNGfree (
   sklRand * randi );

# endif /* __RAND_H__ */
