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

# include <stdlib.h>
# include <pthread.h>

# include "dbug-ts-ext.h"

# define key_dbug_state_err "Can't create TSD key for DBUG_STATE: error %d\n"
# define key_code_state_err "Can't create TSD key for CODE_STATE: error %d\n"
# define set_specthread_err "Unable to set TSD key: error %d\n"

static void clean_dbug_debug_state( void * );
static void clean_dbug_code_state ( void * );

# if defined (LINUX_OS) || defined(CYGWIN_OS)
static void clean_dbug_debug_state_at_exit( void );
static void clean_dbug_code_state_at_exit( void );
# endif

pthread_once_t key_once_dbug_state = PTHREAD_ONCE_INIT;
pthread_once_t key_once_code_state = PTHREAD_ONCE_INIT;

/* thread specif data key */

pthread_key_t THR_KEY_code_state;
pthread_key_t THR_KEY_dbug_state;

/* see code_state() for details */
/* pthread_mutex_t THR_once_mutex = PTHREAD_MUTEX_INITIALIZER; */


/* Programming with POSIX Threads", by David R. Butenhof
 * When you no longer need a mutex that you dynamically initialized by
 * calling pthread_mutex_init, you should destroy the mutex by calling
 * pthread_mutex_destroy. You do not need to destroy a mutex that was
 * statically initialized using the PTHREAD_MUTEX_INITIALIZER macro
 */

pthread_mutex_t THR_my_pthread_mutex_lock_dbug = PTHREAD_MUTEX_INITIALIZER;


/* We can think of TSD (thread specif data) keys as a two dimensional
 * array, with threads across, keys down and each cell as a data (TSD)
 * association between a key and a thread.
 * pthread_key_delete() removes the key for ALL threads, but does not
 * call the destructor for ANY threads. The destructor is assigned by
 * pthread_key_create() and called at thread exit.
 * Exiting a thread removes a column of TSD associations, deleting a
 * key removes a row of TSD associations.
 *
 *
 *               Thread_1 (t1) | Thread_2 (t2)
 *                             |
 *     Key1 (k1)  data_k1_t1   |  data_k1_t2
 *     Key2 (k2)  data_k2_t1   |  data_k2_t2
 *      ......     ......           ......
 *
 * In other words, if <Thread_1> exits, two destructor callbacks are
 * being called and 'data_k1_t1' and 'data_k2_t1 are freed. Besides,
 * the <Thread_1> column is removed.
 *
 * [LINUX AND CYGWIN SYSTEMS]
 * The main() function is a thread as well and may have a key and the
 * relative data bound to it; anyway, when it exits, no destructor is
 * called causing a memory leak (since associated data are not freed)
 * There needs to be a callback invoked at main exiting that ensures
 * the data destruction for main thread. This is done by using atexit
 * function.
 * [/LINUX AND CYGWIN SYSTEMS]
 */

/* callback that creates a thread specific data key that will be used
 * to store a pointer to a CODE_STATE structure
 */

static void init_dbug_code_state( void )
{
    /* DBUGPRINT: printf ("<init_dbug_code_state> callback\n"); */

    /* callback 'clean_dbug_code_state' is called at thread exit */

    if ( pthread_key_create( &THR_KEY_code_state,
                             clean_dbug_code_state ) )
    {
        DbugExit( key_code_state_err );
    };

# if defined (LINUX_OS) || defined (CYGWIN_OS)

    /* register a callback to call when exiting */
    atexit( clean_dbug_code_state_at_exit );

# endif
}



/* callback that creates a thread specific data key that will be used
 * to store a pointer to a DBUG_STATE structure
 */

static void init_dbug_debug_state( void )
{
    /* DBUGPRINT: printf ("<init_dbug_debug_state> callback\n"); */

    /* callback 'clean_dbug_debug_state' is called at thread exit */

    if ( pthread_key_create( &THR_KEY_dbug_state,
                             clean_dbug_debug_state ) )
    {
        DbugExit( key_dbug_state_err );
    };

# if defined (LINUX_OS) || defined (CYGWIN_OS)

    /* register a callback to call when exiting */
    atexit( clean_dbug_debug_state_at_exit );

# endif
}



/* callback that destroies a thread key and its associated data
 * ( DBUG_STATE structure ) at main process exit
 */

# if defined (LINUX_OS) || defined (CYGWIN_OS)
static void clean_dbug_debug_state_at_exit( void )
{
    DBUG_STATE * dbug_state;
    dbug_state = _db_status_();

    if ( dbug_state != NULL )
        DEALLOC( dbug_state );

    set_thread_var( NULL, THR_KEY_dbug_state );
    /* DBUGPRINT: printf ("<clean_dbug_debug_state_at_exit> callback\n"); */
}
# endif



/* callback that destroies a thread key and its associated data
 * ( CODE_STATE structure ) at main process exit
 */

# if defined (LINUX_OS) || defined (CYGWIN_OS)
static void clean_dbug_code_state_at_exit( void )
{
    CODE_STATE * _code_state;
    _code_state = code_state();

    if ( _code_state != NULL )
        FreeState( _code_state );

    set_thread_var( NULL, THR_KEY_code_state );
    /* DBUGPRINT: printf ("<clean_dbug_code_state_at_exit> callback\n"); */
}
# endif



/* callback that destroies a thread key and its associated data
 * ( DBUG_STATE structure )
 */

static void clean_dbug_debug_state( void * dbug_state )
{
    if ( dbug_state != NULL )
        DEALLOC( dbug_state );

    set_thread_var( NULL, THR_KEY_dbug_state );
    /* DBUGPRINT: printf ("<clean_dbug_debug_state> callback\n"); */

    /* not called since it doesn't delete the key for the
     * current thread but for all threads!!!     */
    /* pthread_key_delete( THR_KEY_dbug_state ); */
}



/* callback that destroies a thread key and its associated data
 * ( CODE_STATE structure )
 */

static void clean_dbug_code_state( void * _code_state )
{
    if ( _code_state != NULL )
        FreeState( _code_state );

    set_thread_var( NULL, THR_KEY_code_state );
    /* DBUGPRINT:  printf ("<clean_dbug_code_state> callback\n"); */

    /* not called since it doesn't delete the key for the
     * current thread but for all threads!!!     */
    /* pthread_key_delete( THR_KEY_code_state ); */
}



/* ****************************
 *
 *  USEFUL FUNCTIONS
 *
 * ****************************/

void set_thread_var( const void * data, pthread_key_t thread_key )
{
    /* NOTE: on success returns 0, an error code otherwise */

    if ( pthread_setspecific( thread_key, data ) != 0 )
    {
        DbugExit( set_specthread_err );
    }
}



void del_thread_key( pthread_key_t thread_key )
{
    /* remove the thread specific data key */

    pthread_key_delete( thread_key );
}



void * get_thread_var( pthread_key_t thread_key )
{
    /* returns the value currently associated with the
     * key for the current thread */

    return pthread_getspecific( thread_key );
}



const char * thread_name( void )
{
    static char name_buff[ THREAD_NAME_SIZE ];
    pthread_t id = pthread_self();

    sprintf( name_buff, "T@%ld", id );
    return name_buff;
}



/* Associates a CODE_STATE structure pointer to the relative
 * thread key */

CODE_STATE * code_state( void )
{
    CODE_STATE * state = 0;

    /* NOTE: valgrind complains about a race condition on pthread_once().
     * It is sufficient to protect the pthread_once call with a mutex to
     * avoid the problem. Anyway, I did not found any code where a such
     * solution is adopted, so may be a valgrind's false positive.
     */

    /* pthread_mutex_lock(&THR_once_mutex); */
    pthread_once( &key_once_code_state, init_dbug_code_state );
    /* pthread_mutex_unlock(&THR_once_mutex); */

    /* get_thread_var() returns a pointer to a <CODE_STATE>
     * structure bound to the current thread. If it returns
     * a NULL pointer, it means that there's not any 'link'
     * so we make it by using set_thread_var()
     */

    if ( !(state = ( CODE_STATE * )
            get_thread_var( THR_KEY_code_state )) )
    {
        /* DBUGPRINT: printf ("<code_state> get_thread_var does not have any associated key\n"); */
        state = ( CODE_STATE * ) DbugMalloc( sizeof ( *state) );

        bzero( ( char * ) state, sizeof ( *state) );
        state->func = "?func";
        state->file = "?file";
        set_thread_var( state, THR_KEY_code_state );
    }

    return ( state);
}



/* Associates a DBUG_STATE structure pointer to the relative thread
 * specific data key */

DBUG_STATE * _db_status_( )
{
    DBUG_STATE * db_status;

    /* NOTE: valgrind complains about a race condition on pthread_once().
     * It is sufficient to protect the pthread_once call with a mutex to
     * avoid the problem. Anyway, I did not found any code where a such
     * solution is adopted, so may be a valgrind's false positive.
     *
     * The first call to pthread_once() by any thread in a process, with a
     * given once_control, will call the init_routine() with no arguments.
     * Subsequent calls of pthread_once() with the same once_control will
     * not call the init_routine(). On return from pthread_once(), it is
     * guaranteed that init_routine() has completed. The once_control
     * parameter (key_once) is used to determine whether the associated
     * initialisation routine has been called
     */

    /* pthread_mutex_lock(&THR_once_mutex); */
    pthread_once( &key_once_dbug_state, init_dbug_debug_state );
    /* pthread_mutex_unlock(&THR_once_mutex); */

    if ( !(db_status = ( DBUG_STATE * )
            get_thread_var( THR_KEY_dbug_state )) )
    {
        /* DBUGPRINT: printf ("<_db_status_> get_thread_var does not have any associated key\n"); */
        db_status = ALLOC( sizeof ( DBUG_STATE ) );

        db_status->db_on = FALSE;
        db_status->no_db = TRUE;
        db_status->db_fp = ( FILE * ) 0;
        db_status->db_pfp = ( FILE *) 0;
        db_status->process = "dbug";
        db_status->db_pon = FALSE;

        set_thread_var( db_status, THR_KEY_dbug_state );
    }

    return db_status;
}
