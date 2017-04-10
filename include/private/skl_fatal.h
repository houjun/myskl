/***************************************************************************
 *   Copyright (C) 2008 by Andrea Ghersi                                   *
 *   ghangenit@users.sourceforge.net                                       *
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

# ifndef __SKL_FATAL_H__
# define __SKL_FATAL_H__

# ifdef CHECK_DATA_EXIT_ON_ERROR

/* ===============
 *
 * CHECK LIST
 *
 * ===============
 */

# define verifyList( object, dunp1, dump2 )      \
{                                                \
   _verifyExp( object != NULL );                 \
   if ( (object)->magic != MYSKL_LIST_MAGIC )    \
     _sklFatal                                   \
     (                                           \
        THIS_IS_ME,                              \
        "Bad magic (found 0x%08lx instead "      \
        "of 0x%08lx).\nBad list found!",         \
        (object)->magic,                         \
        MYSKL_LIST_MAGIC                         \
     );                                          \
                                                 \
   _verifyExp( (object)->header != NULL );       \
   _verifyExp( (object)->maxlevel > 0 );         \
}


/* ===============
 *
 * CHECK DATA
 *
 * ===============
 */
# if 0
# define verifyData( data, dunp1, dump2 )        \
{                                                \
   _verifyExp( (data) != NULL );                 \
   if ( (data)->magic != MYSKL_DATA_MAGIC )      \
     _sklFatal                                   \
     (                                           \
        THIS_IS_ME,                              \
        "Bad magic (found 0x%08lx instead "      \
        "of 0x%08lx).\nUnsafe data found!\n",    \
        (data)->magic,                           \
        MYSKL_DATA_MAGIC                         \
     );                                          \
                                                 \
   _verifyExp( (data)->key != NULL );            \
}
# endif


/* ===============
 *
 * CHECK ITER
 *
 * ===============
 */

# define verifyIter( itr, dunp1, dump2 )         \
{                                                \
   _verifyExp( (itr ) != NULL );                 \
   _verifyExp( (*itr) != NULL );                 \
                                                 \
   if ( (*itr)->magic != MYSKL_ITER_MAGIC )      \
   {  _sklFatal                                  \
     (                                           \
        THIS_IS_ME,                              \
        "Bad magic (found 0x%08lx instead "      \
        "of 0x%08lx). Unsafe iterator found! ",  \
        (*itr)->magic,                           \
        MYSKL_ITER_MAGIC                         \
     );                                          \
   }                                             \
}


/* ===============
 *
 * CHECK EXP
 *
 * ===============
 */

# define _verifyExp( e )                         \
{                                                \
   if ( !(e) ) _sklAssertFail                    \
   ( __FILE__, THIS_IS_ME, __LINE__, #e );       \
}

# define verifyExp( e, dump1, dump2 )            \
   _verifyExp( e )


/* ===============
 *
 * CHECK NODE
 *
 * ===============
 */

# define verifyNode( n, dunp1, dump2 )           \
{                                                \
   _verifyExp ( n != NULL );                     \
   _verifyExp ( (n)->sz > 0 );                   \
}


/* ===============
 *
 * CHECK KEY
 *
 * ===============
 */

# define verifyKey( k, dump1, dump2 )            \
{                                                \
   _verifyExp ( k != NULL );                     \
   /* nothing else to do */                      \
}

# endif /* CHECK_DATA_EXIT_ON_ERROR */


# ifdef CHECK_MEMO_EXIT_ON_ERROR

/* ===============
 *
 * CHECK MEMORY
 *
 * ===============
 */

# define verifyMemory( ptr, dim, dump1, dump2 )  \
{                                                \
   if ( (ptr) == NULL )                          \
   {  _sklFatal                                  \
     (                                           \
       THIS_IS_ME,                               \
        "line %u : <%s>. Failed allocating <%u " \
        "bytes> in size. ",                      \
        __LINE__,                                \
        #ptr,                                    \
        dim                                      \
     );                                          \
   }                                             \
}

# endif  /* CHECK_MEMO_EXIT_ON_ERROR */
# endif  /* __SKL_FATAL_H__ */
