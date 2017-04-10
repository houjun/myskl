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

# ifndef __SKL_DEBUG_H__
# define __SKL_DEBUG_H__

# include "skl_priv.h"
# include "skl_mstat.h"


/* *********************************************************** */
/* LAYER FOR DBUG LIBRARY (IF USED) AND MEMORY MANAGEMENT      */
/* *********************************************************** */

# ifndef DBUG_OFF

  /* include dbug macros and functions */
  # include "dbug.h"

  # define X_ALLOC( dim, obj )                       \
  (                                                  \
     ( obj = NULL, dim > 0 ) &&                      \
     ( obj = DBUG_ALLOC ( dim ) ) != NULL ? dim: dim \
  )

  # define X_FREE( obj )                             \
  (                                                  \
     obj != NULL ? (DBUG_FREE ( obj ), obj = NULL) : \
           (NULL)                                    \
  )

  # define X_PRINT( keyword, arglist )               \
      DBUG_PRINT( keyword, arglist )

  # define X_RETURN_VOID()       DBUG_VOID_RETURN
  # define X_RETURN( v )         DBUG_RETURN( v )
  # define X_ENTER( v )          DBUG_ENTER( v )
  # define X_DEBUG_END           DBUG_END
  # define X_DEBUG_OFF           DEBUGGER_OFF
  # define X_DEBUG_ON            DEBUGGER_ON
  # define X_DEBUG_IS_ON         DBUG_IS_ON

  /* Ok, it's a bit dumb define and then undefine
   * these macros but it's for keeping code clean
   */

  # ifdef HAVE_LIBGC  /* we use gc for memory deallocation */
  #   undef  X_FREE
  #   define X_FREE(obj) do {} while(0)
  # endif

# else /* DBUG_OFF */

  # define X_ALLOC( dim, obj )          \
  (                                     \
     ( obj = NULL, ( dim > 0 ) ) &&     \
     ( obj = _no_db_alloc ( dim )       \
     ) != NULL ? dim: dim               \
  )

  # define X_FREE( x )                  \
  (                                     \
     (x != NULL) ?                      \
        (_no_db_free( x ),x = NULL ):   \
        (NULL)                          \
  )

  # define X_PRINT( keyword, arglist )
  # define X_RETURN_VOID() return
  # define X_RETURN( v )   return(v)
  # define X_ENTER( v )
  # define X_DEBUG_END
  # define X_DEBUG_OFF     1
  # define X_DEBUG_ON      0
  # define X_DEBUG_IS_ON   0

# endif
# endif /* __SKL_DEBUG_H__ */
