AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4

lib_LTLIBRARIES = liblargenet-@LARGENET_API_VERSION@.la
liblargenet_@LARGENET_API_VERSION@_la_SOURCES = src/network/

liblargenet_@LARGENET_API_VERSION@_la_LDFLAGS = -version-info $(LARGENET_SO_VERSION)

largenet_includedir = $(includedir)/largenet-$(LARGENET_API_VERSION)

nobase_largenet_include_HEADERS = 

largenet_libincludedir = $(libdir)/example-$(LARGENET_API_VERSION)/include
nodist_largenet_libinclude_HEADERS = largenetconfig.h 

dist_noinst_SCRIPTS = autogen.sh