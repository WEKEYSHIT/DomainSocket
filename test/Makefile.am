bin_PROGRAMS=Server Server_cc Client Client_cc

Server_SOURCES=ServerMain.c
Server_LDADD=../outlibs/server/libServer.a -lstdc++
Server_CPPFLAGS=-I ../outlibs/server

Server_cc_SOURCES=ServerMain.cc
Server_cc_LDADD=../outlibs/server/libServer.a
Server_cc_CPPFLAGS=--std=c++0x -I ../outlibs/server

Client_SOURCES=ClientMain.c
Client_CPPFLAGS=-I ../outlibs/client
Client_LDADD=../outlibs/client/libClient.a -lstdc++

Client_cc_SOURCES=ClientMain.cc
Client_cc_CPPFLAGS=--std=c++0x -I../outlibs/client
Client_cc_LDADD=../outlibs/client/libClient.a

release:all
	@arm-linux-gnueabi-strip ${bin_PROGRAMS}

pack:release
	fput ${bin_PROGRAMS}
