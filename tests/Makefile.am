TESTS = check_downsample
check_PROGRAMS = check_downsample
check_downsample_SOURCES = check_downsample.c $(top_builddir)/downsample.c
check_downsample_CFLAGS = @CHECK_CFLAGS@
check_downsample_LDADD = @CHECK_LIBS@

check_downsample.c: check_downsample.c.in
	checkmk $< > $@
