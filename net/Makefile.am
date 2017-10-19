noinst_PROGRAMS=DialDeamon

DialDeamon_SOURCES=AT.cc glib.cc main.cc serial.cc
DialDeamon_LDADD=${top_builddir}/outlibs/server/libServer.a
DialDeamon_CPPFLAGS=--std=c++0x -I ${top_builddir}/outlibs/server

release:all
	@arm-linux-gnueabi-strip ${noinst_PROGRAMS}

pack:release
	fput ${noinst_PROGRAMS}
