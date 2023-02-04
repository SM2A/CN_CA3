cmake_minimum_required(VERSION 3.22)
project(CN_CA3)

set(CMAKE_CXX_STANDARD 11)

SET (PROJECT_ROOT "Source")

SET (ROUTER_DIR_1 "${PROJECT_ROOT}/Phase1.1/Router")
SET (SERVER_DIR_1 "${PROJECT_ROOT}/Phase1.1/Server")
SET (CLIENT_DIR_1 "${PROJECT_ROOT}/Phase1.1/Client")
SET (MESSAGE_DIR_1 "${PROJECT_ROOT}/Phase1.1/Message")

SET (ROUTER_DIR_2 "${PROJECT_ROOT}/Phase1.2/Router")
SET (SERVER_DIR_2 "${PROJECT_ROOT}/Phase1.2/Server")
SET (CLIENT_DIR_2 "${PROJECT_ROOT}/Phase1.2/Client")
SET (MESSAGE_DIR_2 "${PROJECT_ROOT}/Phase1.2/Message")

SET (ROUTING_DIR "${PROJECT_ROOT}/Routing")

add_executable(router-1.1
        ${ROUTER_DIR_1}/main.cpp
        ${ROUTER_DIR_1}/Router.cpp
        ${ROUTER_DIR_1}/Router.h
        ${MESSAGE_DIR_1}/Message.cpp
        ${MESSAGE_DIR_1}/Message.h
        ${PROJECT_ROOT}/Phase1.1/config.h
)

add_executable(server-1.1
        ${SERVER_DIR_1}/main.cpp
        ${SERVER_DIR_1}/Server.cpp
        ${SERVER_DIR_1}/Server.h
        ${MESSAGE_DIR_1}/Message.cpp
        ${MESSAGE_DIR_1}/Message.h
        ${PROJECT_ROOT}/Phase1.1/config.h
)

add_executable(client-1.1
        ${CLIENT_DIR_1}/main.cpp
        ${CLIENT_DIR_1}/Client.cpp
        ${CLIENT_DIR_1}/Client.h
        ${MESSAGE_DIR_1}/Message.cpp
        ${MESSAGE_DIR_1}/Message.h
        ${PROJECT_ROOT}/Phase1.1/config.h
)

add_executable(router-1.2
        ${ROUTER_DIR_2}/main.cpp
        ${ROUTER_DIR_2}/Router.cpp
        ${ROUTER_DIR_2}/Router.h
        ${MESSAGE_DIR_2}/Message.cpp
        ${MESSAGE_DIR_2}/Message.h
        ${PROJECT_ROOT}/Phase1.2/config.h
        )

add_executable(server-1.2
        ${SERVER_DIR_2}/main.cpp
        ${SERVER_DIR_2}/Server.cpp
        ${SERVER_DIR_2}/Server.h
        ${MESSAGE_DIR_2}/Message.cpp
        ${MESSAGE_DIR_2}/Message.h
        ${PROJECT_ROOT}/Phase1.2/config.h
        )

add_executable(client-1.2
        ${CLIENT_DIR_2}/main.cpp
        ${CLIENT_DIR_2}/Client.cpp
        ${CLIENT_DIR_2}/Client.h
        ${MESSAGE_DIR_2}/Message.cpp
        ${MESSAGE_DIR_2}/Message.h
        ${PROJECT_ROOT}/Phase1.2/config.h
        )

add_executable(routing
        ${ROUTING_DIR}/main.cpp
        ${ROUTING_DIR}/Node.cpp
        ${ROUTING_DIR}/Node.h
        ${ROUTING_DIR}/Network.cpp
        ${ROUTING_DIR}/Network.h
        ${ROUTING_DIR}/CommandParser.cpp
        ${ROUTING_DIR}/CommandParser
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(router-1.1 PRIVATE Threads::Threads)
target_link_libraries(router-1.2 PRIVATE Threads::Threads)
target_link_libraries(routing PRIVATE Threads::Threads)
