INCLUDE_DIRECTORIES(
	../3rdparty
	../3rdparty/boost_1_55_0
	../server/gen-cpp
	../notecore
	../facility
	../include
)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

link_directories(../3rdparty/boost_1_55_0/stage/lib)

ADD_LIBRARY(rpcservice STATIC
	../server/gen-cpp/Hello.cpp
	../server/gen-cpp/UserInfo.cpp
	../server/gen-cpp/NoteInfo.cpp
	../server/gen-cpp/notestructure_constants.cpp
	../server/gen-cpp/notestructure_types.cpp
	rpcservice.cpp
	rpcservice.h
	thriftclient.cpp
	thriftclient.h
	)

target_link_libraries(rpcservice Qt5::Widgets thrift libboost_thread-vc120-mt-gd-1_55.lib)