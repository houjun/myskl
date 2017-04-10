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

# ifndef __SKL_CHECKS__
# define __SKL_CHECKS__

/* ============================================================= */
/* This file implements a sort of barrier - ie a set of checks - */
/* to verify some things before proceeding with the body code of */
/* a function. This barrier can either stop the execution of the */
/* main programm or simple return from the function where it was */
/* called. This header file needs to be included in <skl_priv.h> */
/* ============================================================= */

/* macros for skipping checks on Data|List|Node */

# define DSKIP  (MySKL_d) -1
# define LSKIP  (MySKL_t) -1
# define NSKIP  (MySKL_n) -1


# if defined (CHECK_DATA_WARN_ON_ERROR) || \
     defined (CHECK_MEMO_WARN_ON_ERROR)

  /* --------------------------------------------------
   * each of these func returns from the function where
   * error occured but does not cause the exit from the
   * main program
   * --------------------------------------------------
   */

  # include "skl_warn.h"
# endif

# if defined (CHECK_DATA_EXIT_ON_ERROR) || \
     defined (CHECK_MEMO_EXIT_ON_ERROR)

  /* ------------------------------------------------
   * each of these func causes the exit from the main
   * program and should be used for testing only
   * ------------------------------------------------
   */

  # include "skl_fatal.h"
# endif

/* list, data, node */

# define LDN_BC_X_RETV( l, d, n, v )    \
{                                       \
    if ( l != LSKIP ) {                 \
       verifyList( l, X_RETV, v );      \
    }                                   \
    /*if ( d != DSKIP ) {               \
       verifyData( d, X_RETV, v );      \
    }*/                                 \
    if ( n != NSKIP ) {                 \
       verifyNode( n, X_RETV, v );      \
    }                                   \
}

# define LDN_BC_X_VOID( l, d, n )       \
{                                       \
    if ( l != LSKIP ) {                 \
       verifyList( l, X_VOID, NULL );   \
    }                                   \
    /*if ( d != DSKIP ) {               \
       verifyData( d, X_VOID, NULL );   \
    }*/                                 \
    if ( n != NSKIP ) {                 \
       verifyNode( n, X_VOID, NULL );   \
    }                                   \
}

# define LDN_BC_RETV( l, d, n, v )      \
{                                       \
    if ( l != LSKIP ) {                 \
       verifyList( l, RETV, v );        \
    }                                   \
    /* if ( d != DSKIP ) {              \
       verifyData( d, RETV, v );        \
    }*/                                 \
    if ( n != NSKIP ) {                 \
       verifyNode( n, RETV, v );        \
    }                                   \
}

# define LDN_BC_VOID( l, d, n )         \
{                                       \
    if ( l != LSKIP ) {                 \
       verifyList( l, VOID, NULL );     \
    }                                   \
    /*if ( d != DSKIP ) {               \
       verifyData( d, VOID, NULL );     \
    }*/                                 \
    if ( n != NSKIP ) {                 \
       verifyNode( n, VOID, NULL );     \
    }                                   \
}

/* expression */

# define EXP_BC_X_RETV( exp, v )        \
    verifyExp( exp, X_RETV, v )

# define EXP_BC_X_VOID( exp )           \
    verifyExp( exp, X_VOID, NULL )

# define EXP_BC_RETV( exp, v )          \
    verifyExp( exp, RETV, v )

# define EXP_BC_VOID( exp )             \
    verifyExp( exp, VOID, NULL )

/* iter */

# define ITR_BC_X_RETV( exp, v )        \
    verifyIter( exp, X_RETV, v )

# define ITR_BC_X_VOID( exp )           \
    verifyIter( exp, X_VOID, NULL )

# define ITR_BC_RETV( exp, v )          \
    verifyIter( exp, RETV, v )

# define ITR_BC_VOID( exp )             \
    verifyIter( exp, VOID, NULL )

/* memory */

# define MEM_BC_X_RETV( obj, sz, v )    \
    verifyMemory( obj, sz,              \
      X_RETV, v                         \
    )

# define MEM_BC_X_VOID( obj, sz )       \
    verifyMemory( obj, sz,              \
      X_VOID, NULL                      \
    )

# define MEM_BC_RETV( obj, sz, v )      \
    verifyMemory( obj, sz,              \
      RETV, v                           \
    )

# define MEM_BC_VOID( obj, sz )         \
    verifyMemory( obj, sz,              \
      VOID, NULL                        \

# endif   /* __SKL_CHECKS__ */
