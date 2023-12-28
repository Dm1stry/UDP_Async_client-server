cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME UDP_Async) 
project(${PROJECT_NAME} LANGUAGES CXX C)

option(USE_GCOV "Create a GCov-enabled build." OFF)
if (USE_GCOV)
  set(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
  set(GCC_COVERAGE_LINK_FLAGS    "-lgcov")
endif()

set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -I/usr/local/include" )
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -I/usr/local/include" )

set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )

find_library(LIBEVENT2_CORE event_core)

include_directories(src src/server scr/client)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(SERVER_NAME Server)
set(CLIENT_NAME Client)

set(SERVER_FOLDER src/server)
set(CLIENT_FOLDER src/client)
set(GENERAL_FOLDER src)

set(GENERAL_SOURCES
    ${GENERAL_FOLDER}/error.hpp    ${GENERAL_FOLDER}/error.cpp
    libs/LNL.h                     libs/LNL.cpp
)

set(SERVER_SOURCES
    ${SERVER_FOLDER}/main_server.cpp
    ${SERVER_FOLDER}/baseserver.cpp         ${SERVER_FOLDER}/baseserver.hpp
    ${SERVER_FOLDER}/udpserver.cpp          ${SERVER_FOLDER}/udpserver.hpp
    ${SERVER_FOLDER}/tcpserver.cpp          ${SERVER_FOLDER}/tcpserver.hpp
    ${SERVER_FOLDER}/dataconverter.hpp
    ${SERVER_FOLDER}/numbersgetter.cpp      ${SERVER_FOLDER}/numbersgetter.hpp
)

set(
    CLIENT_SOURCES
    ${CLIENT_FOLDER}/main_client.cpp
    ${CLIENT_FOLDER}/udpclient.cpp          ${CLIENT_FOLDER}/udpclient.hpp
    ${CLIENT_FOLDER}/tcpclient.cpp          ${CLIENT_FOLDER}/tcpclient.hpp
)

add_executable(
    ${SERVER_NAME}
    ${SERVER_SOURCES}
    ${GENERAL_SOURCES}
)
target_link_libraries(${SERVER_NAME} ${LIBEVENT2_CORE})

add_executable(
    ${CLIENT_NAME}
    ${CLIENT_SOURCES}
)

