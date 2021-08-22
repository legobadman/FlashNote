INCLUDE_DIRECTORIES(
	../3rdparty
	../3rdparty/boost_1_55_0
	../notecore
	../facility
	../include
	../3rdparty/sqlite
)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

ADD_LIBRARY(dbservice STATIC
	dbservice.cpp
	dbservice.h
	sql_const.h
	)

target_link_libraries(dbservice Qt5::Widgets sqlite)
