
/******************************************************************************
 *                                                                            *
 *                                 N O T I C E                                *
 *                                                                            *
 *                    Copyright Abandoned, 1987, Fred Fish                    *
 *                                                                            *
 *                                                                            *
 *      This previously copyrighted work has been placed into the  public     *
 *      domain  by  the  author  and  may be freely used for any purpose,     *
 *      private or commercial.                                                *
 *                                                                            *
 *      Because of the number of inquiries I was receiving about the  use     *
 *      of this product in commercially developed works I have decided to     *
 *      simply make it public domain to further its unrestricted use.   I     *
 *      specifically  would  be  most happy to see this material become a     *
 *      part of the standard Unix distributions by AT&T and the  Berkeley     *
 *      Computer  Science  Research Group, and a standard part of the GNU     *
 *      system from the Free Software Foundation.                             *
 *                                                                            *
 *      I would appreciate it, as a courtesy, if this notice is  left  in     *
 *      all copies and derivative works.  Thank you.                          *
 *                                                                            *
 *      The author makes no warranty of any kind  with  respect  to  this     *
 *      product  and  explicitly disclaims any implied warranties of mer-     *
 *      chantability or fitness for any particular purpose.                   *
 *                                                                            *
 ******************************************************************************
 */

# ifndef _dbug_h
# define _dbug_h

# include <sys/types.h>

# include "skl_conf.h"
# include "skl_mstat.h"
# include "skl_defs.h"

# ifdef __cplusplus
extern "C"
{
# endif

 /* external memory stat structure and global var definition name
  * EVENTUALLY CHANGE THEIR NAME
  */

# define DBUG_EXT_MSTAT_ST _skl_mstat_st /* skl_priv.c */
# define DBUG_EXT_MSTAT    _skl_mstat    /* skl_priv.c */


# if !defined (DBUG_OFF)

   typedef struct st_dbug_state
   {
       bool db_on;                  /* TRUE if debugging currently on */
       bool no_db;                  /* TRUE if no debugging at all */
       FILE * db_fp;                /* Output stream, default stderr */
       FILE * db_pfp;               /* Profile stream, 'dbugmon.out' */
       bool db_pon;                 /* TRUE if profile currently on */
       const char *process;         /* Pointer to process name; usually argv[0] */
   } DBUG_STATE;

   typedef struct st_code_state
   {
       int lineno;                  /* Current debugger output line number */
       int level;                   /* Current function nesting level */
       const char * func;           /* Name of current user function */
       const char * file;           /* Name of current user file */
       char ** framep;              /* Pointer to current frame */
       struct db_settings *stack;   /* debugging settings : opaque pointer */
       int jmplevel;                /* Remember nesting level at setjmp () */
       const char * jmpfunc;        /* Remember current function for setjmp */
       const char * jmpfile;        /* Remember current file for setjmp */

       /*
        * The following variables are used to hold the state information
        * between the call to _db_pargs_() and _db_doprnt_(), during
        * expansion of the DBUG_PRINT macro.  This is the only macro
        * that currently uses these variables.
        *
        * These variables are currently used only by _db_pargs_() and
        * _db_doprnt_().
        */

        uint u_line;                /* User source code line number */
        const char * u_keyword;     /* Keyword for current macro */
        int locked;                 /* If locked with _db_lock_file */
   } CODE_STATE;

   typedef struct st_previous_state
   {
      const char * _sfunc_;         /* where previous _func_ is to be retrieved */
      const char * _sfile_;         /*where previous _file_ is to be retrieved */
      uint  _slevel_;               /* where previous level was stashed */
      char ** _sframep_;
   } PREV_STATE;

   extern void _db_enter_(
      const char *_func_,
      const char *_file_,
      uint _line_,
      PREV_STATE * st
   );

   extern void _db_return_(
      uint _line_,
      PREV_STATE * st
   );

   extern void _db_pargs_(
      uint _line_,
      const char *keyword
   );

   extern void _db_dump_(
      uint _line_,
      const char *keyword,
      const char *memory,
      uint length
   );

   extern char * DbugMalloc ( int size );
   extern void DbugExit ( const char * why );
   extern void FreeState ( void * cs );

   extern DBUG_STATE * _db_status_();
   extern char _dig_vec[];
   extern bool _db_keyword_(const char *keyword);
   extern void _db_setjmp_(void);
   extern void _db_longjmp_(void);
   extern void _db_push_(const char *control);
   extern void  _db_pop_(void *, void *);
   extern void _db_doprnt_( const char *format, ... );
   extern void _db_process_(const char *name);
   extern void _db_lock_file(void);
   extern void _db_unlock_file(void);
   extern void _db_end_(void);

   extern void *_db_alloc_(
      size_t dim,
      const char *arg,
      const char *_func_,
      const char *_file_,
      uint _line_
   );
   extern void _db_free_(
      void *ptrmem,
      const char *arg,
      const char *_func_,
      const char *_file_,
      uint _line_
   );

   extern CODE_STATE * code_state( void );
   extern DBUG_STATE * _db_status_( void );

  /* set initial debug condition to "d:t" */
  # define _DBUG_CONDITION_

  # define DBUG_RETURN(a1)                    { DBUG_LEAVE; return (a1); }
  # define DBUG_VOID_RETURN                   { DBUG_LEAVE; return; }
  # define DBUG_PROCESS(a1)                   _db_process_(a1)
  # define DBUG_PUSH(a1)                      _db_push_(a1)
  # define DBUG_POP()                         _db_pop_(NULL,NULL)
  # define DBUG_END()                         _db_end_ ()
  # define DBUG_FILE                          (_db_status_()->db_fp)
  # define DBUG_SETJMP(a1)                    (_db_setjmp_(), setjmp(a1))
  # define DBUG_LONGJMP(a1, a2)               (_db_longjmp_(), longjmp(a1, a2))
  # define DBUG_IN_USE                        (_db_status_()->db_fp && _db_status_()->db_fp != stderr)
  # define DEBUGGER_OFF                       { _db_status_()->no_db = 1; _db_status_()->db_on = 0; }
  # define DEBUGGER_ON                        { _db_status_()->no_db = 0; _db_status_()->db_on = 1; }
  # define DBUG_my_pthread_mutex_lock_FILE    { _db_lock_file(); }
  # define DBUG_my_pthread_mutex_unlock_FILE  { _db_unlock_file(); }
  # define DBUG_ASSERT(A)                     assert(A)
  # define DBUG_IS_ON                         ( _db_status_()->db_on ? 1 : 0 )

  # define DBUG_ENTER(a)                                        \
    PREV_STATE _sstate_;                                        \
    if ( !_db_status_()->no_db )                                \
        _db_enter_(a, __FILE__, __LINE__, &_sstate_ )

  # define DBUG_ALLOC(x)                                        \
    ( ( !_db_status_()->no_db ) ?                               \
        _db_alloc_( x, #x, __FUNCTION__, __FILE__, __LINE__ ) : \
        _no_db_alloc( x )                                       \
    )

  # define DBUG_FREE(x)                                         \
    ( ( !_db_status_()->no_db ) ?                               \
        _db_free_( x, #x, __FUNCTION__, __FILE__,__LINE__) :    \
        _no_db_free( x )                                        \
    )

  # define DBUG_LEAVE                               \
    {                                               \
       if ( !_db_status_()->no_db )                 \
            _db_return_(__LINE__, &_sstate_);       \
    }

  # define DBUG_PRINT(keyword, arglist)             \
    {                                               \
        if ( _db_status_()->db_on )                 \
        {                                           \
            _db_pargs_( __LINE__, keyword );        \
            _db_doprnt_ arglist;                    \
        }                                           \
    }

  # define DBUG_EXECUTE(keyword, a1)                \
    {                                               \
        if ( _db_status_()->db_on )                 \
        { if (_db_keyword_(keyword)) { a1 } }       \
    }

  # define DBUG_DUMP(keyword, a1, a2)               \
    {                                               \
        if ( _db_status_()->db_on )                 \
        { _db_dump_(__LINE__, keyword, a1, a2); }   \
    }


# else          /* No debugger */

  # define DBUG_ENTER(a1)
  # define DBUG_RETURN(a1)                 return (a1)
  # define DBUG_VOID_RETURN                return
  # define DBUG_EXECUTE(keyword, a1)       do { } while(0)
  # define DBUG_PRINT(keyword, arglist)    do { } while(0)
  # define DBUG_PUSH(a1)                   do { } while(0)
  # define DBUG_POP()
  # define DBUG_PROCESS(a1)
  # define DBUG_FILE                       (stderr)
  # define DBUG_SETJMP                     setjmp
  # define DBUG_LONGJMP                    longjmp
  # define DBUG_DUMP(keyword, a1, a2)
  # define DBUG_IN_USE
  # define DBUG_END()
  # define DEBUGGER_OFF
  # define DEBUGGER_ON
  # define DBUG_my_pthread_mutex_lock_FILE
  # define DBUG_my_pthread_mutex_unlock_FILE
  # define DBUG_ASSERT(A)

  # define DBUG_IS_ON                     0

# endif
# ifdef __cplusplus
}
# endif
# endif
