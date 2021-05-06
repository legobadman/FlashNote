cmake_minimum_required(VERSION 3.0)
set (CMAKE_CXX_STANDARD 11)
PROJECT (helloWord)
INCLUDE_DIRECTORIES(./gen-cpp)
SET( SERVER_FILES helloServer.cpp)
SET( LIB_SOURCE  ./gen-cpp/Hello.cpp )

ADD_EXECUTABLE(server ${SERVER_FILES} ${LIB_SOURCE})
TARGET_LINK_LIBRARIES(server -lthrift)

ADD_EXECUTABLE(node_server NoteInfo_server.cpp ./gen-cpp/NoteInfo.cpp ./gen-cpp/notestructure_types.cpp ./gen-cpp/notestructure_constants.cpp)
TARGET_LINK_LIBRARIES(node_server -lthrift)

ADD_EXECUTABLE(user_server UserInfo_server.cpp ./gen-cpp/UserInfo.cpp)
TARGET_LINK_LIBRARIES(user_server -lthrift)

