CC = g++

CFLAGS = -std=c++11 -O0 -g3 -Wall
INCLUDE = -isystem ../systemc/include
LIB_INCL = -L../systemc/lib-linux64
LIB = -Wl,-rpath,../systemc/lib-linux64 -lsystemc

SRC_PATH = ./src

SRC = 	RAuES_U4A3.cpp \
		producer.cpp \
		switch.cpp

OBJ = 	RAuES_U4A3.o \
		producer.o \
		switch.o

OUT = switch

$(OUT): $(OBJ)
	$(CC) -o $(OUT) $(OBJ) $(LIB_INCL) $(LIB)

RAuES_U4A3.o: $(SRC_PATH)/RAuES_U4A3.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $(SRC_PATH)/RAuES_U4A3.cpp

producer.o: $(SRC_PATH)/producer.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $(SRC_PATH)/producer.cpp

switch.o: $(SRC_PATH)/switch.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $(SRC_PATH)/switch.cpp
