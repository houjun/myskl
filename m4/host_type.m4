AC_DEFUN([MSKL_AC_DEFINE_HOST], [

  case "$host_os" in
    *linux*)
      AC_DEFINE(LINUX_OS,, [Linux OS])
      ;;
    *cygwin*)
      AC_DEFINE(CYGWIN_OS,, [CYGWIN OS])
      ;;
    *mingw*)
      AC_DEFINE(MINGW_OS,, [MINGW_OS])
      ;;
    *solaris*)
      ;;
    *)
      AC_DEFINE(OTHER_OS,, [OTHER OS])
      ;;
  esac
])