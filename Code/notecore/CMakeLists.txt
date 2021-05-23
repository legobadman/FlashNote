#project(notecore)

include_directories(
    ../facility
	)

ADD_LIBRARY(notecore STATIC
	framework.h
	dllmain.cpp
	notebase.cpp
	notebase.h
	notecore.h
	notecoreinit.cpp
	notecoreinit.h)