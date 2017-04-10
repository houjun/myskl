AC_DEFUN([MYSKL_CHECK_LIBGC], [
  AC_CHECK_LIB(gc,GC_malloc,
  [
    AC_CHECK_LIB(gc,GC_pthread_create,
      [
        AC_DEFINE(HAVE_LIBGC, 1, [Define to 1 if libgc is available])
        GCLIBS="-lgc"
        msg_gc=yes
      ],
      [
        if test "x$threadsafe" != "xyes"; then
          AC_DEFINE(HAVE_LIBGC, 1, [Define to 1 if libgc is available])
          GCLIBS="-lgc"
          msg_gc=yes
        else
          AC_MSG_ERROR(*** LIBGC support will not be built (threads support not found) ***)
        fi
      ]
    )
  ],
  AC_MSG_ERROR(*** LIBGC support will not be built (LIBGC library not found) ***)
)
])