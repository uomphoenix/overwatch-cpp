COMPILER = g++
CFLAGS = -c -std=gnu99 -g -Wall -Wextra
LFLAGS = -std=gnu99 -g -Wall -Wextra

UNAME_S:= $(shell uname)

ifeq ($(UNAME_S),Linux)
LIBS = -lpthread -lrt -lnsl
endif