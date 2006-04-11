SUBDIRS = lib

libedivaldocdir = ${prefix}/doc/edival-0.1.0
libedivaldoc_DATA = \
	README          \
	COPYING         \
	AUTHORS         \
	ChangeLog       \
	INSTALL         \
	NEWS            \
	TODO

EXTRA_DIST = $(libedivaldoc_DATA)

# Copy all the spec files. Of cource, only one is actually used.
dist-hook:
	for specfile in *.spec; do           \
		if test -f $$specfile; then      \
			cp -p $$specfile $(distdir); \
		fi                               \
	done

