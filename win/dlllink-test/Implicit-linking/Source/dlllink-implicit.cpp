
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
 *     General, is unuseful)
 */


/**
 ** This file deals with IMPLICIT dll linking
 **/

# include <Windows.h>
# include <stdio.h>

# define MySKL_DLLNAME "libmyskl-2.dll"
# include <myskl.h>


/* Outside the VS environment, use Command Prompt and
 * move to the root directory <dlllink-test>. Then:
 *
 * set PATH=Library;%PATH%
 * Implicit-linking\Debug\Implicit-linking.exe
 */

int main( int argc,char *argv[] )
{
    MySKLverboseEnable();
    printf( "%s: version <%s>.\n", MySKL_DLLNAME, MySKLversion() );

    exit ( EXIT_SUCCESS );
}
