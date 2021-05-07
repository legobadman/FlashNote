cmake_minimum_required(VERSION 3.0)

#add_compile_definitions(THRIFT_EXPORT)
set (CMAKE_CXX_STANDARD 11)
PROJECT (client)
INCLUDE_DIRECTORIES(
	../3rdparty
	../3rdparty/boost_1_55_0
	../server/gen-cpp
)
SET( SERVER_FILES client.cpp)
SET( LIB_SOURCE  ../server/gen-cpp/Hello.cpp )

ADD_EXECUTABLE(client ${SERVER_FILES} ${LIB_SOURCE})

target_link_libraries(client thrift)