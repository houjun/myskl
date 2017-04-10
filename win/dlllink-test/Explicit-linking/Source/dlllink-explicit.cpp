
/*
 * With IMPLICIT LINKING, the executable using the DLL links to an import
 * library (.LIB file) provided by the maker of the DLL. The operating
 * system loads the DLL when the executable using it is loaded. The client
 * executable calls the DLL's exported functions just as if the functions
 * were contained within the executable.
 *
 * With EXPLICIT LINKING, the executable using the DLL must make function
 * calls to explicitly load and unload the DLL, and to access the DLL's
 * exported functions. The client executable must call the exported
 * functions through a function pointer.
 *
 * With both implicit and explicit linking, Windows first searches the
 * set of pre-installed DLLs such as the performance library (KERNEL32.DLL)
 * and the security library (USER32.DLL). Windows then searches for the
 * DLLs in the following sequence:
 *
 *  1. The directory where the executable module for the current
 *     process is located.
 *  2. The current directory.
 *  3. The Windows system directory. The GetSystemDirectory function
 *     retrieves the path of this directory.
 *  4. The Windows directory. The GetWindowsDirectory function
 *     retrieves the path of this directory.
 *  5. The directories listed in the PATH environment variable.
 *
 *  NOTE: the LIBPATH environment variable is NOT used (so setting the
 *     field <Additional Library Directories>, in Properties - Linker -
 *     General,  is unuseful)
 */


/**
 ** This file deals with EXPLICIT dll linking
 **/

# include <Windows.h>
# include <stdio.h>

# define MySKL_DLLNAME "libmyskl-2.dll"
# include <myskl.h>

/*
 * a flag indicating whether the DLL has been
 * already loaded and the DLL instance address
 */

static char MySKL_DLLLoaded = 0;
static HINSTANCE MySKL_DLLhLib = NULL;

/*
 * create new types as pointers to functions
 */

typedef char * (*MySKLversionFPT)( void );
typedef bool   (*MySKLverboseFPT)( void );



/* ============================================================== */
/* ============================================================== */

/*
 * load DLL library
 */

int DLL_LoadLibrary( void )
{
    if ( MySKL_DLLLoaded )
        return 0;

    if ( !( MySKL_DLLhLib = LoadLibrary( MySKL_DLLNAME ) ) )
    {
        fprintf( stderr, "Loading of %s failed. Giving up.\n", MySKL_DLLNAME );
        return -1;
    }

    /* library has been loaded */

    MySKL_DLLLoaded = 1;
    return 0;
}


/*
 * import a single library function from the DLL
 */

# define DLL_ImportFunction( ftype, fptr, fname )                  \
{                                                                  \
   if ( MySKL_DLLhLib != NULL )                                    \
   {                                                               \
      fptr = ( ftype ) GetProcAddress( MySKL_DLLhLib, fname );     \
      if ( !fptr )                                                 \
      {                                                            \
         fprintf( stderr, "%s : GetProcAddress : %d : failed!\n",  \
           fname, __LINE__ );                                      \
      }                                                            \
   }                                                               \
   else fprintf( stderr, "%s is not loaded.\n", MySKL_DLLNAME );   \
}


/*
 * free DLL library
 */

int DLL_FreeLibrary( void )
{
    if ( MySKL_DLLLoaded )
    {
        FreeLibrary( MySKL_DLLhLib );
        MySKL_DLLLoaded = 0;

        MySKL_DLLhLib = NULL;
        return 1;
    }

    return 0;
}



/* ============================================================== */
/* ============================================================== */

/* Outside the VS environment, use Command Prompt and
 * move to the root directory <dlllink-test>. Then:
 *
 * set PATH=Library;%PATH%
 * Explicit-linking\Debug\Explicit-linking.exe
 */

int main( int argc,char *argv[] )
{
    MySKLversionFPT MySKLversionPtr;
    MySKLverboseFPT MySKLverbosePtr;

    if ( DLL_LoadLibrary() < 0 )
        exit ( EXIT_FAILURE );

    DLL_ImportFunction( MySKLversionFPT, MySKLversionPtr, "MySKLversion" );
    DLL_ImportFunction( MySKLverboseFPT, MySKLverbosePtr, "MySKLverboseEnable" );

    MySKLverbosePtr();
    printf( "%s: version <%s>.\n", MySKL_DLLNAME, MySKLversionPtr() );

    DLL_FreeLibrary();
    exit ( EXIT_SUCCESS );
}