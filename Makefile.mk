###### Editable variables########
BIN_DIR = bin$(BITS)

WX_SETUP_H_INCLUDE_DIR = $(WX_LIB_DIR)/mswu
WX_LIBS = -lwxmsw32u_core -lwxbase32u # -lwxpng -lwxzlib

PLATFORM_LIBS = -luser32 -lkernel32 -lws2_32 -liphlpapi  -lgdi32 -lwinmm -limm32 -lversion -lole32 -loleaut32 -lsetupapi -luxtheme  -lwinspool -lcomctl32 -lshlwapi -lwsock32 -luuid -loleacc
PLATFORM_LDFLAGS = -mwindows



TTT_DIR=u-tic-tac-toe

RM = rm # rm for unix-like OS

STRIP = strip


DBUTTONCOLOR = \(100,100,100\) #Red,Green,Blue

P1COLOR = \(100,0,0\) #Red,Green,Blue

P2COLOR = \(0,0,100\) #Red,Green,Blue


###### Editable variables########

CC = gcc
CFLAGS = -O2 -std=c99 -pedantic -Wall

CXX = g++
CXXFLAGS = -O2 -std=c++14 -pedantic -Wall -I$(WX_INCLUDE_DIR) -I$(WX_SETUP_H_INCLUDE_DIR) -I$(TTT_DIR) -DDBUTTONCOLOR=wxColor$(DBUTTONCOLOR) -DP1COLOR=wxColor$(P1COLOR) -DP2COLOR=wxColor$(P2COLOR) -DBUILT_IN_VARS
LDFLAGS = $(PLATFORM_LDFLAGS) -L$(WX_LIB_DIR)

BIN_NAME = u3t-gui
BIN_PVP = $(BIN_DIR)/$(BIN_NAME)_pvp$(BITS).exe
BIN_PVE = $(BIN_DIR)/$(BIN_NAME)_pve$(BITS).exe

.PHONY: all strip clean distclean

all: $(BIN_PVP) $(BIN_PVE)

$(BIN_PVP):	mainPVP$(BITS).o ttt_engine$(BITS).o glibcrng$(BITS).o
	$(CXX) -o $(BIN_PVP) mainPVP$(BITS).o ttt_engine$(BITS).o glibcrng$(BITS).o $(LDFLAGS) $(WX_LIBS) $(PLATFORM_LIBS)
	

$(BIN_PVE):	mainPVE$(BITS).o ttt_engine$(BITS).o glibcrng$(BITS).o
	$(CXX) -o $(BIN_PVE) mainPVE$(BITS).o ttt_engine$(BITS).o glibcrng$(BITS).o $(LDFLAGS) $(WX_LIBS) $(PLATFORM_LIBS)


mainPVP$(BITS).o:	mainPVP.cpp
	$(CXX) -o mainPVP$(BITS).o $(CXXFLAGS) -c mainPVP.cpp

mainPVE$(BITS).o:	mainPVE.cpp
	$(CXX) -o mainPVE$(BITS).o $(CXXFLAGS) -c mainPVE.cpp


ttt_engine$(BITS).o:	$(TTT_DIR)/ttt_engine.c
	$(CC) -o ttt_engine$(BITS).o $(CFLAGS) -c $(TTT_DIR)/ttt_engine.c


glibcrng$(BITS).o:	$(TTT_DIR)/glibcrng.c
	$(CC) -o glibcrng$(BITS).o $(CFLAGS) -c $(TTT_DIR)/glibcrng.c

strip:	
	$(STRIP) -s $(BIN_PVP) $(BIN_PVE)

clean:
	$(RM) *.ic *.o  *~ *.log *.exe

distclean:
	make clean ; $(RM) $(BIN_PVP) $(BIN_PVE) ; make -C $(BIN_DIR) clean

