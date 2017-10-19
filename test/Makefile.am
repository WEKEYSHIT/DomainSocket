noinst_PROGRAMS=Server Server_cc Client Client_cc

Server_SOURCES=ServerMain.c
Server_LDADD=${top_builddir}/outlibs/server/libServer.a -lstdc++
Server_CPPFLAGS=-I ${top_builddir}/outlibs/server

Server_cc_SOURCES=ServerMain.cc
Server_cc_LDADD=${top_builddir}/outlibs/server/libServer.a
Server_cc_CPPFLAGS=--std=c++0x -I ${top_builddir}/outlibs/server

Client_SOURCES=ClientMain.c
Client_CPPFLAGS=-I ${top_builddir}/outlibs/client
Client_LDADD=${top_builddir}/outlibs/client/libClient.a -lstdc++

Client_cc_SOURCES=ClientMain.cc
Client_cc_CPPFLAGS=--std=c++0x -I${top_builddir}/outlibs/client
Client_cc_LDADD=${top_builddir}/outlibs/client/libClient.a

release:all
	@arm-linux-gnueabi-strip ${noinst_PROGRAMS}

pack:release
	fput ${noinst_PROGRAMS}
