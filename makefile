SRC_DIR=Source

COMPILER = g++
VERSION = -std=c++11
CC = ${COMPILER} ${VERSION}

SRC_FILES_S := $(wildcard $(SRC_DIR)/Phase1.1/Server/*.cpp, wildcard $(SRC_DIR)/Phase1.1/Server/*.h, wildcard $(SRC_DIR)/Phase1.1/Message/*.cpp, wildcard $(SRC_DIR)/Phase1.1/Message/*.h, wildcard $(SRC_DIR)/Phase1.1/config.h)
SRC_FILES_C := $(wildcard $(SRC_DIR)/Phase1.1/Client/*.cpp, wildcard $(SRC_DIR)/Phase1.1/Client/*.h, wildcard $(SRC_DIR)/Phase1.1/Message/*.cpp, wildcard $(SRC_DIR)/Phase1.1/Message/*.h, wildcard $(SRC_DIR)/Phase1.1/config.h)
SRC_FILES_R := $(wildcard $(SRC_DIR)/Phase1.1/Router/*.cpp, wildcard $(SRC_DIR)/Phase1.1/Router/*.h, wildcard $(SRC_DIR)/Phase1.1/Message/*.cpp, wildcard $(SRC_DIR)/Phase1.1/Message/*.h, wildcard $(SRC_DIR)/Phase1.1/config.h)
SRC_FILES_S2 := $(wildcard $(SRC_DIR)/Phase1.2/Server/*.cpp, wildcard $(SRC_DIR)/Phase1.2/Server/*.h, wildcard $(SRC_DIR)/Phase1.2/Message/*.cpp, wildcard $(SRC_DIR)/Phase1.2/Message/*.h, wildcard $(SRC_DIR)/Phase1.2/config.h)
SRC_FILES_C2 := $(wildcard $(SRC_DIR)/Phase1.2/Client/*.cpp, wildcard $(SRC_DIR)/Phase1.2/Client/*.h, wildcard $(SRC_DIR)/Phase1.2/Message/*.cpp, wildcard $(SRC_DIR)/Phase1.2/Message/*.h, wildcard $(SRC_DIR)/Phase1.2/config.h)
SRC_FILES_R2 := $(wildcard $(SRC_DIR)/Phase1.2/Router/*.cpp, wildcard $(SRC_DIR)/Phase1.2/Router/*.h, wildcard $(SRC_DIR)/Phase1.2/Message/*.cpp, wildcard $(SRC_DIR)/Phase1.2/Message/*.h, wildcard $(SRC_DIR)/Phase1.2/config.h)
SRC_FILES_RP := $(wildcard $(SRC_DIR)/Routing/*.cpp, wildcard $(SRC_DIR)/Routing/*.h)

all: clean server.out client.out router.out server2.out client2.out router2.out routing.out

server.out: $(SRC_FILES_S)
	${CC} ${SRC_DIR}/Phase1.1/Server/*.cpp ${SRC_DIR}/Phase1.1/Message/*.cpp -o server1.out

client.out: $(SRC_FILES_C)
	${CC} ${SRC_DIR}/Phase1.1/Client/*.cpp ${SRC_DIR}/Phase1.1/Message/*.cpp -o client1.out

router.out: $(SRC_FILES_R)
	${CC} ${SRC_DIR}/Phase1.1/Router/*.cpp ${SRC_DIR}/Phase1.1/Message/*.cpp -o router1.out

server2.out: $(SRC_FILES_S2)
	${CC} ${SRC_DIR}/Phase1.2/Server/*.cpp ${SRC_DIR}/Phase1.2/Message/*.cpp -o server2.out

client2.out: $(SRC_FILES_C2)
	${CC} ${SRC_DIR}/Phase1.2/Client/*.cpp ${SRC_DIR}/Phase1.2/Message/*.cpp -o client2.out

router2.out: $(SRC_FILES_R2)
	${CC} ${SRC_DIR}/Phase1.2/Router/*.cpp ${SRC_DIR}/Phase1.2/Message/*.cpp -o router2.out

routing.out: $(SRC_FILES_RP)
	${CC} ${SRC_DIR}/Routing/*.cpp -o routing.out

clean:
	rm -f *.out

graphClean:
	rm -f graph.txt
	rm -f *.png
