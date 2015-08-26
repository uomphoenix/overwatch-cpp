#support linux and solaris makes... easier for testing on my VM and uni machines
include Makefile.inc

DIRS = 

VPATH:=$(VPATH) src include


overwatch: main.o AuthenticationClient.o VideoFeedClient.o
	$(COMPILER) $(LFLAGS) -o overwatch main.o AuthenticationClient.o VideoFeedClient.o logging.o $(LIBS)

clean:
	/bin/rm -f main.o AuthenticationClient.o VideoFeedClient.o pop3

main.o: main.cpp AuthenticationClient.h VideoFeedClient.h
	$(COMPILER) $(CFLAGS) main.cpp

AuthenticationClient.o: AuthenticationClient.cpp SocketClient.h
	$(COMPILER) $(CFLAGS) AuthenticationClient.cpp

VideoFeedClient.o: VideoFeedClient.cpp SocketClient.h
	$(COMPILER) $(CFLAGS) VideoFeedClient.cpp

