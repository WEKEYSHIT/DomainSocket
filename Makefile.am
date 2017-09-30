SUBDIRS=src

pack:
	@mkdir -p outlibs/client >/dev/null 2>&1 ; cp src/incs/Client.h  src/incs/Client++.h  src/libClient.a outlibs/client
	@mkdir -p outlibs/server >/dev/null 2>&1 ; cp src/incs/Server.h src/incs/lock.h src/incs/SafeList.h src/libServer.a outlibs/server

