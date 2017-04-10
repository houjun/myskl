/***************************************************************************
 *   Copyright (C) 2010 by Andrea Ghersi                                   *
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

# ifndef __SKL_THREADS_H__
# define __SKL_THREADS_H__

# include <pthread.h>
# include "skl_defs.h"

# if THREAD

  # define cond_init_default( sync_cond )                   \
    pthread_cond_init( &sync_cond, NULL )

  # define cond_init( sync_cond, attr )                     \
    pthread_cond_init( &sync_cond, &attr )

  # define cond_wait( sync_cond, sync_lock )                \
    pthread_cond_wait( &sync_cond, &sync_lock )

  # define mutex_init_default( sync_lock )                  \
    pthread_mutex_init( &sync_lock, NULL )

  # define mutex_init( sync_lock, attr )                    \
    pthread_mutex_init( &sync_lock, &attr )

  # define cond_broadcast( sync_cond )                      \
    pthread_cond_broadcast( &sync_cond )

  # define cond_signal( sync_cond )                         \
    pthread_cond_signal( &sync_cond )

  # define mutex_unlock( sync_lock )                        \
    pthread_mutex_unlock( &sync_lock )

  # define mutex_lock( sync_lock )                          \
    pthread_mutex_lock( &sync_lock )

  # define cond_destroy( sync_cond)                         \
    pthread_cond_destroy( &sync_cond )

  # define mutex_destroy( sync_lock )                       \
    pthread_mutex_destroy( &sync_lock )

  /* This will make <errno> a per-thread variable
   * _REENTRANT also forces redefinition of some
   * other functions to their *_r
   */

  # ifdef _REENTRANT
  #   define errno (*(__errno_location()))
  # endif

  # if THREAD_FULL

   /* =========================================================
    *
    * This implementation is slightly biased towards writers
    * ( a horde of writers can starve readers indefinitely ):
    * just modify WRITER_EXIT if you'd rather the balance be
    * the other way around. It is sufficient to swap the if/
    * else condition.
    *
    * =========================================================*/

    # ifdef THREAD_READERS_WRITERS_V2

      # define READER_ENTER( l )                            \
      {                                                     \
        mutex_lock( (l)->lock );                            \
                                                            \
        if ( (l)->writers || (l)->write_waiters )           \
        {                                                   \
          (l)->read_waiters++;                              \
                                                            \
          do cond_wait( (l)->read, (l)->lock);              \
          while ( (l)->writers || (l)->write_waiters );     \
                                                            \
          (l)->read_waiters--;                              \
        }                                                   \
                                                            \
        (l)->readers++;                                     \
        mutex_unlock( (l)->lock );                          \
      }

      # define READER_EXIT( l )                             \
      {                                                     \
        mutex_lock( (l)->lock );                            \
        (l)->readers--;                                     \
                                                            \
        if ( (l)->write_waiters )                           \
        cond_signal( (l)->write );                          \
                                                            \
        mutex_unlock( (l)->lock );                          \
      }

      # define WRITER_ENTER( l )                            \
      {                                                     \
        mutex_lock( (l)->lock );                            \
        if ( (l)->readers || (l)->writers )                 \
        {                                                   \
            (l)->write_waiters++;                           \
                                                            \
            do cond_wait( (l)->write, (l)->lock );          \
            while ( (l)->readers || (l)->writers );         \
                                                            \
            (l)->write_waiters--;                           \
        }                                                   \
                                                            \
        (l)->writers = 1;                                   \
        mutex_unlock( (l)->lock );                          \
      }

      # define WRITER_EXIT( l )                             \
      {                                                     \
        mutex_lock( (l)->lock );                            \
        (l)->writers = 0;                                   \
                                                            \
        /* change the order of <if/else> condition to       \
         * biase this implementation towards readers */     \
                                                            \
        if ( (l)->write_waiters )                           \
            cond_signal( (l)->write );                      \
        else if ( (l)->read_waiters )                       \
            cond_broadcast( (l)->read );                    \
                                                            \
        mutex_unlock( (l)->lock );                          \
      }

      # define RW_INIT( l )                                 \
      {                                                     \
        mutex_init_default( (l)->lock );                    \
        cond_init_default ( (l)->read );                    \
        cond_init_default( (l)->write );                    \
                                                            \
        (l)->read_waiters = (l)->write_waiters =            \
        (l)->readers = l->writers =  0;                     \
      }

      # define RW_DESTROY( l )                              \
      {                                                     \
        mutex_destroy( (*l)->lock );                        \
        cond_destroy ( (*l)->read );                        \
        cond_destroy( (*l)->write );                        \
      }

    # endif

   /* ============================================================
    *
    * Another implementation
    *
    * ============================================================*/

    # ifdef THREAD_READERS_WRITERS_V1

      # include "rwlock.h"

      # define READER_ENTER( l )                                \
      {                                                         \
        rwl_readlock ( &((l)->rw_lock)  );                      \
      }

      # define READER_EXIT( l )                                 \
      {                                                         \
        rwl_readunlock ( &((l)->rw_lock));                      \
      }

      # define WRITER_ENTER( l )                                \
      {                                                         \
        rwl_writelock ( &((l)->rw_lock));                       \
      }

      # define WRITER_EXIT( l )                                 \
      {                                                         \
        rwl_writeunlock ( &((l)->rw_lock) );                    \
      }

      # define RW_INIT( l )                                     \
      {                                                         \
        rwl_init ( &((l)->rw_lock) );                           \
      }

      # define RW_DESTROY( l )                                  \
      {                                                         \
        rwl_destroy( &((l)->rw_lock) );                         \
      }

    # endif /* THREAD_READERS_WRITERS_V1 */
  # endif   /* THREAD_FULL */

  # ifndef THREAD_FULL
  # define READER_ENTER( l )
  # define READER_EXIT( l )
  # define WRITER_ENTER( l )
  # define WRITER_EXIT( l )
  # define RW_DESTROY( l )
  # define RW_INIT(l)
  # endif

# else /* ! THREAD */

  # define cond_init_default( sync_cond )
  # define cond_init( sync_cond, attr )
  # define cond_wait( sync_cond, sync_lock )
  # define mutex_init_default( sync_lock )
  # define mutex_init( sync_lock, attr )
  # define cond_broadcast( sync_cond )
  # define mutex_destroy( sync_lock )
  # define mutex_unlock( sync_lock )
  # define cond_destroy( sync_cond )
  # define cond_signal( sync_cond )
  # define mutex_lock( sync_lock )
  # define READER_ENTER( l )
  # define READER_EXIT( l )
  # define WRITER_ENTER( l )
  # define WRITER_EXIT( l )
  # define RW_DESTROY( l )
  # define RW_INIT(l)

# endif
# endif /* __SKL_THREADS_H__ */
