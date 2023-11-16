#Makefile
CC = g++
CFLAGS = -Wall -Werror -Wmissing-prototypes -std=c++17 -O3
OBJS = ../../build/tcpScaleServer.o
# OBJS1 = ../../build/i2cMux.o ../../build/i2cScaleNAU7802.o 
PROG = ../../output/tcpScaleServer
# LOCALLIBDIR = /usr/local/lib
# LDFLAGS = -L$(LOCALLIBDIR)
LDLIBS  = -lpqxx -lpq
# INC1=-I../../include

all: $(PROG)

# $(PROG): $(OBJS)
#     $(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS) $(LDLIBS)
$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(INC) $(OBJS) -o $@ $(LDLIBS)

../../build/tcpScaleServer.o: tcpScaleServer.cpp
	mkdir -p ../../build
	mkdir -p ../../output
	$(CC) -c $(INC1) -o ../../build/tcpScaleServer.o tcpScaleServer.cpp

.PHONY: clean

clean:
	rm -f ../../build/*~ ../../build/*.o $(PROG) core a.out

run: $(PROG)
	./$(PROG) $(ARGS)
