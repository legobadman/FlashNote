cmake_minimum_required(VERSION 3.0)
set (CMAKE_CXX_STANDARD 11)
PROJECT (helloWord)
INCLUDE_DIRECTORIES(../3rdparty ./gen-cpp)
SET( SERVER_FILES helloServer.cpp)
SET( LIB_SOURCE  ./gen-cpp/Hello.cpp )

ADD_EXECUTABLE(server ${SERVER_FILES} ${LIB_SOURCE} )
TARGET_LINK_LIBRARIES(server thrift)

