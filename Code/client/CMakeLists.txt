cmake_minimum_required(VERSION 3.0)

#add_compile_definitions(THRIFT_EXPORT)
set (CMAKE_CXX_STANDARD 11)
PROJECT (client)
INCLUDE_DIRECTORIES(
	../3rdparty
	../3rdparty/boost_1_55_0
	../server/gen-cpp
)

SET( 
	LIB_SOURCE
	../server/gen-cpp/Hello.cpp
	../server/gen-cpp/UserInfo.cpp
	../server/gen-cpp/NoteInfo.cpp
	../server/gen-cpp/notestructure_constants.cpp
	../server/gen-cpp/notestructure_types.cpp
)

FIND_LIBRARY(BOOST_LIB
	NAMES libboost_thread-vc120-mt-gd-1_55
	HINTS ../3rdparty/boost_1_55_0/stage/lib)

message(${BOOST_LIB})

#link_directories要在add_executable之前加
link_directories(../3rdparty/boost_1_55_0/stage/lib)

ADD_EXECUTABLE(client
	client.cpp
	${LIB_SOURCE}
)

target_link_libraries(client thrift libboost_thread-vc120-mt-gd-1_55.lib)