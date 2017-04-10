
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

# ifndef __SKL_PROTO_H__
# define __SKL_PROTO_H__


/* ==================================== */
/* <skl_init.c>                         */
/* ==================================== */

# ifdef _PROTO_PUBLIC_INIT

MS_EXTERN MySKL_t MySKLinit
 (
    uint,
    int  (*compare)( const void *, const void * ),
    void (*dstfunc)( MySKL_n ),
    MySKL_e *
 );

# endif


/* ==================================== */
/* <skl_ins.c>                          */
/* ==================================== */

# ifdef _PROTO_PUBLIC_INSERT

MS_EXTERN MySKL_e MySKLinsertUPD_ALL
 (
    MySKL_t l,
    MySKL_n newnode,
    void updfunc ( const void *, const void * )

 );

MS_EXTERN MySKL_e MySKLinsertUPD
 (
    MySKL_t l,
    MySKL_n newnode,
    void updfunc ( const void *, const void * )

 );

MS_EXTERN MySKL_e MySKLinsertAD
 (
    MySKL_t l,
    MySKL_n n
 );

MS_EXTERN MySKL_e MySKLinsertND
 (
    MySKL_t l,
    MySKL_n n
 );

MS_EXTERN MySKL_e MySKLcopy
 (
    MySKL_t source,
    MySKL_t target,
    MySKL_n cpyfunc ( const void * )
 );

# endif


/* ==================================== */
/* <skl_del.c>                          */
/* ==================================== */

# ifdef _PROTO_PUBLIC_DELETE

MS_EXTERN MySKL_e MySKLdeleteNF
 (
    MySKL_t l,
    MySKL_n nodetodel,
    bool ( *dstfunc )( MySKL_n )
 );

MS_EXTERN MySKL_e MySKLdeleteAF
 (
    MySKL_t l,
    MySKL_n nodetodel,
    bool ( *dstfunc )( MySKL_n )
 );

# endif


/* ==================================== */
/* <skl_search.c>                       */
/* ==================================== */

# ifdef _PROTO_PUBLIC_SEARCH

MS_EXTERN MySKL_e MySKLsearch
 (
    MySKL_t l,
    MySKL_n nodetofound,
    MySKL_n * found_node
 );

MS_EXTERN MySKL_e MySKLsearchALL
 (
    MySKL_t l,
    MySKL_n nodetofound,
    MySKL_i * i
 );

# endif


/* ==================================== */
/* <skl_dst.c>                          */
/* ==================================== */

# ifdef _PROTO_PUBLIC_DESTROY

MS_EXTERN void MySKLdestroy
 (
    MySKL_t * l,
    void ( *destroy )( MySKL_n  )
 );

# endif


/* ==================================== */
/* <skl_join.c>                         */
/* ==================================== */

# ifdef _PROTO_PUBLIC_JOIN

MS_EXTERN MySKL_e MySKLjoinAD
 (
    MySKL_t target,
    MySKL_t source,
    MySKL_n cpyfunc ( const void * n )
 );

MS_EXTERN MySKL_e MySKLjoinND
 (
    MySKL_t target,
    MySKL_t source,
    MySKL_n cpyfunc ( const void * n )
 );

# endif


/* ==================================== */
/* <skl_join.c>                         */
/* ==================================== */

# ifdef _PROTO_PUBLIC_INTERSECT

MS_EXTERN MySKL_e MySKLintersect
  (
    MySKL_t source1,
    MySKL_t source2,
    MySKL_t target, MySKL_n cpyfunc ( const void * n )
  );

# endif


/* ==================================== */
/* <skl_lc.c>                           */
/* ==================================== */

# ifdef _PROTO_PUBLIC_LC

MS_EXTERN void MySKLaddtoListsContainer
 (
    MySKL_t l
 );

MS_EXTERN bool MySKLinitListsContainer
 (
    void
 );

MS_EXTERN void MySKLfreeListsContainer
 (
    void
 );

# endif


/* ==================================== */
/* <skl_misc.c>                         */
/* ==================================== */

# ifdef _PROTO_PUBLIC_MISC

MS_EXTERN bool MySKLmemoryCleaned
 (
    void
 );

MS_EXTERN uint MySKLgetMaxLevel
 (
    MySKL_t
 );

MS_EXTERN uint MySKLgetCurLevel
 (
    MySKL_t
 );

MS_EXTERN uint MySKLgetNodeNum
 (
    MySKL_t
 );
/*
MS_EXTERN Item MySKLgetItem
 (
    MySKL_n
 );

MS_EXTERN Key MySKLgetKey
 (
    MySKL_n
 );
*/
MS_EXTERN bool MySKLisEmpty
 (
    MySKL_t,
    uint k
 );

MS_EXTERN MySKL_n MySKLgetTail
 (
   MySKL_t
 );


MS_EXTERN MySKL_n MySKLgetHead
 (
   MySKL_t l
 );

MS_EXTERN const char * MySKLversion
 (
    void
 );

MS_EXTERN bool MySKLverboseDisable
 (
    void
 );

MS_EXTERN bool MySKLCustomVerboseEnable
 (
    const char *
 );

MS_EXTERN bool MySKLverboseEnable
 (
    void
 );

MS_EXTERN bool MySKLverboseResume
 (
    void
 );

MS_EXTERN bool MySKLverboseSuspend
 (
    void
 );

# endif


/* ==================================== */
/* <skl_draw.c>                         */
/* ==================================== */

# ifdef _PROTO_PUBLIC_DRAW

MS_EXTERN void MySKLdrawList
 (
    MySKL_t l,
    int ( *sizeofData )( MySKL_n ),
    void ( *printData )( MySKL_n )
 );

MS_EXTERN void MySKLdisplayListStruct
 (
    MySKL_t l
 );

# endif


/* ==================================== */
/* <skl_iter.c>                         */
/* ==================================== */

# ifdef _PROTO_PUBLIC_ITERATOR

MS_EXTERN void MySKLsetIterator
 (
    MySKL_t l,
    MySKL_i * i,
    MySKL_n n
 );

MS_EXTERN void MySKLresetIterator
 (
    MySKL_i * iter,
    MySKL_n n
 );

MS_EXTERN MySKL_n MySKLgetNextNode
 (
    MySKL_i * i,
    uint k
 );

MS_EXTERN MySKL_n MySKLgetPrevNode
 (
    MySKL_i * iter,
    uint lev
 );

MS_EXTERN void MySKLdestroyIterator
 (
    MySKL_i * iter
 );

# endif


/* ==================================== */
/* <skl_gen.c>                          */
/* ==================================== */

# ifdef _PROTO_PUBLIC_GEN

/* MS_EXTERN uint MySKLgenLevel ( MySKL_t l ); */

# endif


/* ==================================== */
/* <skl_priv.c>                         */
/* ==================================== */

# ifdef _PROTO_PRIVATE_ERROR

extern void _sklAssertFail
 (
    const char *filename,
    const char * func,
    uint line,
    const char *expr
 );

extern void _sklFatal
 (
    const char * routine,
    const char * format,
    ...
 );

extern void _sklWarning
  (
    const char * routine,
    const char * format,
    ...
  );

extern void _sklFreeMemBlocks
  (
    void *, ...
  );

# endif


/* ==================================== */
/* <skl_priv.c>                         */
/* ==================================== */

# ifdef _PROTO_PRIVATE_VERSION

extern const char * _sklVersion
 (
    void
 );

# endif


/* ==================================== */
/* <skl_lc.c>                           */
/* ==================================== */

# ifdef _PROTO_PRIVATE_LC

extern bool _createListsContainer
 (
    char
 );

extern void _containerPushAcl
 (
    MySKL_t
 );

extern void _destroyListsContainer
 (
    void
 );

# endif


/* ==================================== */
/* <skl_ins.c>                          */
/* ==================================== */

# ifdef _PROTO_PRIVATE_INSERT

extern MySKL_e _insertI
 (
    MySKL_t l,
    MySKL_n n,
    void updfunc ( const void *, const void * ),
    uint level,
    uint8
 );

extern MySKL_e _insertPlain
 (
    MySKL_t l,
    MySKL_n n,
    uint level,
    uint8
 );

# endif


/* ==================================== */
/* <skl_iter.c>                       */
/* ==================================== */

# ifdef _PROTO_PRIVATE_ITERATOR

extern void setIteratorNoMutex
 (
    MySKL_t l,
    MySKL_i iter,
    MySKL_n node,
    MySKL_n term
 );

extern void setIterator
 (
    MySKL_t l,
    MySKL_i iter,
    MySKL_n node,
    MySKL_n term
 );

extern MySKL_n getNextNoMutex
 (
    MySKL_i iter,
    uint lev
 );

extern MySKL_n getNext
 (
    MySKL_i iter,
    uint lev
 );

# endif


/* ==================================== */
/* <skl_search.c>                       */
/* ==================================== */

# ifdef _PROTO_PRIVATE_SEARCH

extern MySKL_n search
 (
    MySKL_t l,
    MySKL_n nodetofound
 );

# endif


/* ==================================== */
/* <skl_init.c>                         */
/* ==================================== */

# ifdef _PROTO_PRIVATE_INIT

extern MySKL_t init
 (
    uint max,
    int  ( * compare )( const void *, const void * ),
    void ( * dstfunc )( MySKL_n ),
    MySKL_e *
 );

# endif


/* ==================================== */
/* <skl_priv.c>                         */
/* ==================================== */

# ifdef _PROTO_PRIVATE_DBG

extern void * _no_db_alloc(size_t );
extern void   _no_db_free( void * );

# endif


/* ==================================== */
/* <skl_gen.c>                          */
/* ==================================== */

# ifdef _PROTO_PRIVATE_GEN

extern uint genLevel ( MySKL_t l );

# endif


/* ==================================== */
/* <rand.c>                             */
/* ==================================== */

# ifdef _PROTO_PRIVATE_RAND
  # include "skl_rand.h"
# endif

# endif /* __SKL_PROTO_H__ */
