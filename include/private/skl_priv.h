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

# ifndef __SKL_PRIV_H__
# define __SKL_PRIV_H__

# include "skl_conf.h"
# include "skl_defs.h"


/* Use something like <IFDOUBLE( statement; )> is a
 * bit weird, I know. But I prefer doing that than
 * put a lot of '#ifdef' in the code
 */

# ifdef THREAD

#   define IFTHREAD(code) code
#   define IFNOTHREAD(code)

#   if defined THREAD_FULL && \
       defined THREAD_READERS_WRITERS_V1

#     define IFTHREAD_RW_V1(code) code
#     define IFTHREAD_RW_V2(code)

#   elif defined THREAD_FULL && \
         defined THREAD_READERS_WRITERS_V2

#     define IFTHREAD_RW_V2(code) code
#     define IFTHREAD_RW_V1(code)
#   else
#     define IFTHREAD_RW_V2(code)
#     define IFTHREAD_RW_V1(code)
#   endif
# else
#   define IFTHREAD_RW_V1(code)
#   define IFTHREAD_RW_V2(code)
#   define IFTHREAD(code)
#   define IFNOTHREAD(code) code
# endif

# ifdef MYSKL_DOUBLE_LL_TYPE
#   define IFDOUBLE(code) code
# else
#   define IFDOUBLE(code)
# endif

# ifdef MYSKL_NO_RAND
#   define IFNORAND(code) code
#   define IFRAND(code)
# else
#   define IFNORAND(code)
#   define IFRAND(code)  code
# endif

# include "skl_threads.h"



/* ************************************************ */
/*                                                  */
/*           Typedefs for data types                */
/*                                                  */
/* ************************************************ */

typedef struct MySKLnode * MySKL_n;
typedef struct _sklRand  * Rand_t;

typedef int  ( *CMPFUNC_T )( const void *, const void *);
typedef void ( *DSTFUNC_T )( MySKL_n );


/* list structure */
typedef struct MySKLlist
{
   uint32 magic;
   CMPFUNC_T cmp;
   DSTFUNC_T dst;

   IFRAND ( Rand_t rng_item; )

   uint curlevel,
        maxlevel,
        nodenum;

   /* use implementation 1 of readers/writers */

   IFTHREAD_RW_V1 (
        rwlock_t rw_lock;
   )

   /* use implementation 2 of readers/writers */

   IFTHREAD_RW_V2 (
        pthread_mutex_t lock;
        pthread_cond_t read;
        pthread_cond_t write;
        uint readers;
        uint writers;
        uint read_waiters;
        uint write_waiters;
   )

   IFNORAND (
        uint nodenumelem[MYSKL_MAXLEVELS];
   )

   MySKL_n header;
   MySKL_n tail;
}  MySKL_ls, * MySKL_t;


/* node structure */
typedef struct MySKLnode
{
   MySKL_n * next;
   IFDOUBLE ( MySKL_n * prev; )
   uint sz;
}  MySKL_ns;


/* container structure */
typedef struct MySKLcontainer
{
   IFTHREAD (
        pthread_mutex_t mtx_lock;
   )

   MySKL_t * acl;
   uint max_lists;
   uint lists_cnt;
   char mode;
}  MySKL_cs;


/* iterator structure */
typedef struct MySKLiterator
{
   ulong magic;
   MySKL_n * next;
   MySKL_n * prev;
   MySKL_t list;
   MySKL_n term;  /* terminator node */
   uint prevlev;
   IFTHREAD ( pthread_mutex_t mtx_lock; )
}  MySKL_is, * MySKL_i;


/* returnable error codes */
typedef enum
{
   MYSKL_NODE_NOT_DELETED,
   MYSKL_MEM_EXHAUSTED,
   MYSKL_RANDGEN_ERROR,
   MYSKL_DUPLICATE_KEY,
   MYSKL_KEY_NOT_FOUND,
   MYSKL_KEY_FOUND,
   MYSKL_STATUS_OK,
   MYSKL_ERROR
}  MySKL_e;

/* skl_priv.c */
extern MySKL_cs lc;



/* ************************************************ */
/*                                                  */
/*          Macros for node operations              */
/*                                                  */
/* ************************************************ */

# define next_node_while_key_equal( l, n )     \
   while (                                     \
     n->next[ 0 ] != NIL ( l ) &&              \
     eq ( l, n->next[ 0 ], n )                 \
   ) n = next_node( n )

# define next_node(n)                          \
   ( *(n->next) )     /* n->next[0] */

# define next_node_level(n,lev)                \
   ( *(n->next+lev) ) /* n->next[lev] */



/* ************************************************ */
/*                                                  */
/*          Other useful macros                     */
/*                                                  */
/* ************************************************ */

/* macros for list operations and comparisons */

# define less( l, a, b )( ( *l->cmp )( a, b ) < 0 )
# define eq( l, a, b )  ( !( *l->cmp ) ( a, b ) )
# define end( t, k )    ( t->next[ k ] == NIL )
# define NIL( l )       (l)->header
# define key( a )       ( a->key )

/* macros for list behavior */

# define AD_MODE        0x000001  /* Allow duplications */
# define ND_MODE        0x000002  /* No duplications */
# define UP_ONE_MODE    0x000004  /* Update one node only */
# define UP_ALL_MODE    0x000010  /* Update all equal nodes */
# define CP_MODE        0x000020  /* Copy */
# define SHR_ALL        0x000040  /* Search all */

# define NOP_INP        0x000001  /* no operations in progress */
# define DST_OP_IN_PROGRESS  0x000002  /* destruction in progess */

/* magic debug values */

# define MYSKL_LIST_MAGIC 0x12345678LU
# define MYSKL_DATA_MAGIC 0x0ea1b33fLU
# define MYSKL_ITER_MAGIC 0x3544DA2ALU
# define MYSKL_FUNC_MAGIC 0x9823af7eLU /* unused */

/* macros for container */

# define LC_MSTL  10   /* max. storable list */
# define LC_FULL  1
# define LC_NONE  0

/* function not available and function name */

# define warnfna( msg ) fprintf ( stderr, msg, THIS_IS_ME )
# define THIS_IS_ME  __FUNCTION__

/* functions useful on error to free previously
 * allocated memory */

# define ON_ERROR_FREE_MEM(ptr,dim,args...) \
if (!ptr)                                   \
{                                           \
    _sklFreeMemBlocks ( args, NULL );       \
    MEM_BC_X_RETV ( ptr, dim, NULL );       \
}

# define ON_ERROR_FREE_EXP(ptr,args...) \
if (!ptr)                               \
{                                       \
    _sklFreeMemBlocks ( args, NULL );   \
    EXP_BC_RETV ( ptr, NULL );          \
}

# include "skl_checks.h"
# include "skl_protos.h"
# include "skl_debug.h"

# endif /* __SKL_PRIV_H__ */
