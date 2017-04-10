AC_DEFUN([MSKL_SET_THREAD_MODEL], [

  AC_MSG_CHECKING([for thread model])

  # check for gcc thread model
  THREADS="$($CC -v 2>&1 | sed -n 's/^Thread model: //p')"

  if test -z "$THREADS"; then
    # check for solaris cc: we currently support only posix threads
    # solaris threads perhaps will be added in a future release
    ( $CC -V 2>&1 | grep 'Sun C' > /dev/null ) && THREADS="posix"
  fi

  AC_MSG_RESULT([$THREADS])

  case "$THREADS" in
    sthread ) # currently solaris threads are not supported
      case "$host_os" in
        *linux*)
          AC_DEFINE(LINUX_THREADS,, [Enable linux threads])
          AC_DEFINE(_REENTRANT,, [Enable reentrant functions])
          ;;
        *solaris*)
          AC_DEFINE(SOLARIS_THREADS,, [Enable solaris threads])
          AC_DEFINE(_REENTRANT,, [Enable reentrant functions])
          ;;
        *)
          AC_MSG_ERROR([MySKL does not support <solaris threads> on this platform.])
          ;;

      esac
      THREADLIBS=-mt
      ;;

    posix | pthreads )
      case "$host_os" in
        *linux*)
          AC_DEFINE(LINUX_THREADS,, [Enable linux threads])
          AC_DEFINE(_REENTRANT,, [Enable reentrant functions])
          ;;
        *cygwin*)
          AC_DEFINE(WIN32_THREADS,, [Enable win32 threads])
          AC_DEFINE(_REENTRANT,, [Enable reentrant functions])
          ;;
        *solaris*)
          AC_DEFINE(SOLARIS_THREADS,, [Enable solaris threads])
          AC_DEFINE(_REENTRANT,, [Enable reentrant functions])
          ;;
        *)
          AC_MSG_ERROR([MySKL does not support <pthreads> on this platform.])
          ;;
      esac
      THREADLIBS=-lpthread
      ;;

    win32)
      case "$host_os" in
        *mingw*)
          AC_DEFINE(_REENTRANT,, [Enable reentrant functions])
          AC_DEFINE(WIN32_THREADS,, [Enable win32 threads])
          ;;
        *)
          AC_MSG_ERROR([MySKL does not support <win32> on this platform.])
          ;;
      esac
      THREADLIBS=-lpthread
      ;;

    *)
      AC_MSG_ERROR([$THREADS is an unknown thread package.])
      ;;
  esac

  AC_DEFINE(THREAD, [1], [Enable Thread-safety] )

  if test "x$threadsafefull" = "xyes"; then
    AC_DEFINE(THREAD_FULL, [1], [Enable full Thread-safety] )
  fi
])
