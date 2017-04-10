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

# ifndef __SKL_WARN_H__
# define __SKL_WARN_H__

# define return_X_RETV( val )  X_RETURN( val )
# define return_X_VOID( val )  X_RETURN_VOID()

# ifdef NWARNING
   # define _sklRaiseWr(arg)  do { } while(0)
# else
   # define _sklRaiseWr(arg)  _sklWarning arg
# endif

# define return_RETV( val )   return( val )
# define return_VOID( val )   return

# ifdef CHECK_DATA_WARN_ON_ERROR

/* =========================
 *
 *  CHECK LIST
 *
 * =========================
 */

# define verifyList( obj, funid, val )           \
{                                                \
   if ( (obj) == NULL )                          \
   {  _sklRaiseWr                                \
     ((                                          \
        THIS_IS_ME,                              \
        "line %u. Trying to execute operation "  \
        "on a 'NULL' list. Invalid operation. ", \
        __LINE__                                 \
     ));                                         \
     return_##funid( val );                      \
   }                                             \
                                                 \
   if ( (obj)->magic != MYSKL_LIST_MAGIC )       \
   {  _sklRaiseWr                                \
     ((                                          \
        THIS_IS_ME,                              \
        "Bad magic (found 0x%08lx instead "      \
        "of 0x%08lx). Bad list found! ",         \
        (obj)->magic,                            \
        MYSKL_LIST_MAGIC                         \
     ));                                         \
     return_##funid( val );                      \
   }                                             \
                                                 \
   if ( (obj)->header == NULL )                  \
   {  _sklRaiseWr                                \
     ((                                          \
        THIS_IS_ME,                              \
        "line %u. Trying to execute operation "  \
        "on a broken list: (header == NULL). ",  \
        __LINE__                                 \
     ));                                         \
     return_##funid( val );                      \
   }                                             \
                                                 \
   if ( (obj)->maxlevel <= 0 )                   \
   {  _sklRaiseWr                                \
     ((                                          \
        THIS_IS_ME,                              \
        "line %u. Trying to execute operation "  \
        "on a broken list: (maxlevel <= 0). ",   \
        __LINE__                                 \
     ));                                         \
     return_##funid( val );                      \
   }                                             \
}


/* =========================
 *
 *  CHECK PENDING OPERATIONS
 *
 * =========================
 */

# define verifyPendingOperations( obj, funid, val ) \
{                                                   \
   if ( (obj)->opr_pending != NOP_INP )             \
       return_##funid( val );                       \
}

# if 0
# define verifyData( data, funid, val )          \
{                                                \
   if ( (data) == NULL )                         \
   {  _sklRaiseWr                                \
     ((                                          \
        THIS_IS_ME,                              \
        "line %u. Trying to execute operation "  \
        "on 'NULL' data. Invalid operation. ",   \
        __LINE__                                 \
     ));                                         \
     return_##funid( val );                      \
   }                                             \
                                                 \
   if ( (data)->magic != MYSKL_DATA_MAGIC )      \
   {  _sklRaiseWr                                \
     ((                                          \
        THIS_IS_ME,                              \
        "Bad magic (found 0x%08lx instead "      \
        "of 0x%08lx). Unsafe data found! ",      \
        (data)->magic,                           \
        MYSKL_DATA_MAGIC                         \
     ));                                         \
     return_##funid( val );                      \
   }                                             \
                                                 \
   if ( (data)->key == NULL )                    \
   {  _sklRaiseWr                                \
     ((                                          \
        THIS_IS_ME,                              \
        "line %u. Trying to execute operation "  \
        "on data having not key. Not valid. ",   \
        __LINE__                                 \
     ));                                         \
     return_##funid( val );                      \
   }                                             \
}
# endif


/* =========================
 *
 *  CHECK ITERATOR
 *
 * =========================
 */

# define verifyIter( iter, funid, val )          \
{                                                \
   if ( (iter) == NULL || (*iter) == NULL )      \
   {  _sklRaiseWr                                \
     ((                                          \
        THIS_IS_ME,                              \
        "line %u. Trying to execute operation "  \
        "on a 'NULL' iterator. Not permitted. ", \
        __LINE__                                 \
     ));                                         \
     return_##funid( val );                      \
   }                                             \
                                                 \
   if ( (*iter)->magic != MYSKL_ITER_MAGIC )     \
   {  _sklRaiseWr                                \
     ((                                          \
        THIS_IS_ME,                              \
        "Bad magic (found 0x%08lx instead "      \
        "of 0x%08lx). Unsafe iterator found! ",  \
        (*iter)->magic,                          \
        MYSKL_ITER_MAGIC                         \
     ));                                         \
     return_##funid( val );                      \
   }                                             \
}


/* =========================
 *
 *  CHECK NODE
 *
 * =========================
 */

# define verifyNode( n, funid, val )             \
{                                                \
   if ( (n) == NULL )                            \
   {  _sklRaiseWr                                \
     ((                                          \
        THIS_IS_ME,                              \
        "line %u. Trying to execute operation "  \
        "on a 'NULL' node. Invalid operation. ", \
        __LINE__                                 \
     ));                                         \
     return_##funid( val );                      \
   }                                             \
                                                 \
   if ( (n)->sz <= 0 )                           \
   {  _sklRaiseWr                                \
     ((                                          \
        THIS_IS_ME,                              \
        "line %u. Trying to execute operation "  \
        "on a broken node. ( size <= 0 ). ",     \
        __LINE__                                 \
     ));                                         \
     return_##funid( val );                      \
   }                                             \
}


/* =========================
 *
 *  CHECK KEY
 *
 * =========================
 */

# define verifyKey( k, funid, val )              \
{                                                \
   if ( (k) == NULL )                            \
   {  _sklRaiseWr                                \
     ((                                          \
        THIS_IS_ME,                              \
        "line %u. Trying to execute operation "  \
        "having a 'NULL' key. Invalid key. ",    \
        __LINE__                                 \
     ));                                         \
     return_##funid( val );                      \
   }                                             \
}


/* =========================
 *
 *  CHECK GENERIC EXPRESSION
 *
 * =========================
 */

# define verifyExp( exp, funid, val )            \
{                                                \
   if ( !(exp) )                                 \
   {  _sklRaiseWr                                \
     ((                                          \
        THIS_IS_ME,                              \
        "line %u. Trying to execute operation "  \
        "with invalid condition: (%s) untrue. ", \
        __LINE__,                                \
        #exp                                     \
     ));                                         \
     return_##funid( val );                      \
   }                                             \
}

# endif  /* CHECK_DATA_WARN_ON_ERROR */


# ifdef CHECK_MEMO_WARN_ON_ERROR

/* =========================
 *
 *  CHECK MEMORY
 *
 * =========================
 */

# define verifyMemory( ptr, dim, funid, val )    \
{                                                \
   if ( (ptr) == NULL )                          \
   {  _sklRaiseWr                                \
     ((                                          \
        THIS_IS_ME,                              \
        "line %u : <%s>. Failed allocating <%u " \
        "bytes> in size. ",                      \
        __LINE__,                                \
        #ptr,                                    \
        dim                                      \
     ));                                         \
     return_##funid( val );                      \
   }                                             \
}

# endif /* CHECK_MEMO_WARN_ON_ERROR */
# endif /* __SKL_WARN_H__ */
