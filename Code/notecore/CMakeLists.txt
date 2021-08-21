#project(notecore)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

include_directories(
    ../facility
	)

ADD_LIBRARY(notecore STATIC
	framework.h
	#dllmain.cpp
	guiddefine.h
	guiddefine.cpp
	guidconst.h
	guidconst.cpp
	notebase.cpp
	notebase.h
	notecontainer.h
	#notecore.h
	notecore2.h
	notecoreinit.cpp
	notecoreinit.h)
	#notecore_c.c
	#notecore_i.c
	#notecore_p.c)

target_link_libraries(notecore Qt5::Widgets)