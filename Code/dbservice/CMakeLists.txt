INCLUDE_DIRECTORIES(
	../3rdparty
	../3rdparty/boost_1_55_0
	../server/gen-cpp
	../notecore
	../facility
	../include
	../3rdparty/sqlite
)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

ADD_LIBRARY(dbservice STATIC
	../server/gen-cpp/Hello.cpp
	../server/gen-cpp/UserInfo.cpp
	../server/gen-cpp/NoteInfo.cpp
	../server/gen-cpp/notestructure_constants.cpp
	../server/gen-cpp/notestructure_types.cpp
	dbservice.cpp
	dbservice.h
	sql_const.h
	)

target_link_libraries(dbservice Qt5::Widgets sqlite)
