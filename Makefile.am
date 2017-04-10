# not a GNU package. You can remove this line, if
# have all needed files, that a GNU package needs
#AUTOMAKE_OPTIONS = foreign 1.4

# Set ACLOCAL_AMFLAGS so that m4
# directory is used by aclocal

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = scripts win Makefile.cvs libmyskl.pc.in
SUBDIRS = src test include

# besides a "gzip" file, it creates a "bzip"
# and a "zip" file when 'make dist' is used

AUTOMAKE_OPTIONS = dist-bzip2 dist-zip


pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libmyskl.pc

files:
	@files=`ls $(DISTFILES) 2> /dev/null `; for p in $$files; do \
	  echo $$p; \
	done