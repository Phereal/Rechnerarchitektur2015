CC = g++

CFLAGS = -std=c++11 -O0 -g3 -Wall -Wextra -Wconversion -pedantic
INCLUDE = -isystem ../systemc/include
LIB_INCL = -L../systemc/lib-linux64
LIB = -Wl,-rpath,../systemc/lib-linux64 -lsystemc

SRC_PATH = ./src

SRC = RAuES_U4A2.cpp
OBJ = RAuES_U4A2.o
OUT = RAuES_U4A2

$(OUT): $(OBJ)
	$(CC) -o $(OUT) $(OBJ) $(LIB_INCL) $(LIB)

RAuES_U4A2.o: $(SRC_PATH)/RAuES_U4A2.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $(SRC_PATH)/RAuES_U4A2.cpp
