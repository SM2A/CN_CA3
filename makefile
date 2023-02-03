SRC_DIR=Source

COMPILER = g++
VERSION = -std=c++11
CC = ${COMPILER} ${VERSION}

SRC_FILES_S := $(wildcard $(SRC_DIR)/Server/*.cpp, wildcard $(SRC_DIR)/Server/*.h, wildcard $(SRC_DIR)/Message/*.cpp, wildcard $(SRC_DIR)/Message/*.h, wildcard $(SRC_DIR)/config.h)
SRC_FILES_C := $(wildcard $(SRC_DIR)/Client/*.cpp, wildcard $(SRC_DIR)/Client/*.h, wildcard $(SRC_DIR)/Message/*.cpp, wildcard $(SRC_DIR)/Message/*.h, wildcard $(SRC_DIR)/config.h)
SRC_FILES_R := $(wildcard $(SRC_DIR)/Router/*.cpp, wildcard $(SRC_DIR)/Router/*.h, wildcard $(SRC_DIR)/Message/*.cpp, wildcard $(SRC_DIR)/Message/*.h, wildcard $(SRC_DIR)/config.h)
SRC_FILES_RP := $(wildcard $(SRC_DIR)/Routing/*.cpp, wildcard $(SRC_DIR)/Routing/*.h)

all: clean server.out client.out router.out routing.out

server.out: $(SRC_FILES_S)
	${CC} ${SRC_DIR}/Server/*.cpp ${SRC_DIR}/Message/*.cpp -o server.out

client.out: $(SRC_FILES_C)
	${CC} ${SRC_DIR}/Client/*.cpp ${SRC_DIR}/Message/*.cpp -o client.out

router.out: $(SRC_FILES_C)
	${CC} ${SRC_DIR}/Router/*.cpp ${SRC_DIR}/Message/*.cpp -o router.out

routing.out: $(SRC_FILES_RP)
	${CC} ${SRC_DIR}/Routing/*.cpp -o routing.out

clean:
	rm -f *.out
