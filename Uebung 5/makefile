CC = g++

CFLAGS = -std=c++11 -O0 -g3 -Wall
INCLUDE = -isystem ../systemc/include
LIB_INCL = -L../systemc/lib-linux64
LIB = -Wl,-rpath,../systemc/lib-linux64 -lsystemc

SRC_PATH = ./src

SRC = 	RAuES_U5.cpp \
		router.cpp \
		ram.cpp \
		pgm.cpp \
		PaketBuffer.cpp \
		module.cpp \
		gateway.cpp \
		compute.cpp \
		cache.cpp

OBJ = 	RAuES_U5.o \
		router.o \
		ram.o \
		pgm.o \
		PaketBuffer.o \
		module.o \
		gateway.o \
		compute.o \
		cache.o

OUT = noc

$(OUT): $(OBJ)
	$(CC) -o $(OUT) $(OBJ) $(LIB_INCL) $(LIB)

RAuES_U5.o: $(SRC_PATH)/RAuES_U5.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $(SRC_PATH)/RAuES_U5.cpp

router.o: $(SRC_PATH)/router.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $(SRC_PATH)/router.cpp

ram.o: $(SRC_PATH)/ram.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $(SRC_PATH)/ram.cpp

pgm.o: $(SRC_PATH)/pgm.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $(SRC_PATH)/pgm.cpp

PaketBuffer.o: $(SRC_PATH)/PaketBuffer.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $(SRC_PATH)/PaketBuffer.cpp

module.o: $(SRC_PATH)/module.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $(SRC_PATH)/module.cpp

gateway.o: $(SRC_PATH)/gateway.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $(SRC_PATH)/gateway.cpp

compute.o: $(SRC_PATH)/compute.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $(SRC_PATH)/compute.cpp

cache.o: $(SRC_PATH)/cache.cpp
	$(CC) $(CFLAGS) -c $(INCLUDE) $(SRC_PATH)/cache.cpp
