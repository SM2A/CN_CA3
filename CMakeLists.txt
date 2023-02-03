cmake_minimum_required(VERSION 3.22)
project(CN_CA3)

set(CMAKE_CXX_STANDARD 11)

SET (PROJECT_ROOT "Source")
SET (ROUTER_DIR "${PROJECT_ROOT}/Router")
SET (SERVER_DIR "${PROJECT_ROOT}/Server")
SET (CLIENT_DIR "${PROJECT_ROOT}/Client")
SET (MESSAGE_DIR "${PROJECT_ROOT}/Message")

add_executable(router
        ${ROUTER_DIR}/main.cpp
        ${ROUTER_DIR}/Router.cpp
        ${ROUTER_DIR}/Router.h
        ${MESSAGE_DIR}/Message.cpp
        ${MESSAGE_DIR}/Message.h
        ${PROJECT_ROOT}/config.h
)

add_executable(server
        ${SERVER_DIR}/main.cpp
        ${SERVER_DIR}/Server.cpp
        ${SERVER_DIR}/Server.h
        ${MESSAGE_DIR}/Message.cpp
        ${MESSAGE_DIR}/Message.h
        ${PROJECT_ROOT}/config.h
)

add_executable(client
        ${CLIENT_DIR}/main.cpp
        ${CLIENT_DIR}/Client.cpp
        ${CLIENT_DIR}/Client.h
        ${MESSAGE_DIR}/Message.cpp
        ${MESSAGE_DIR}/Message.h
        ${PROJECT_ROOT}/config.h
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(router PRIVATE Threads::Threads)
