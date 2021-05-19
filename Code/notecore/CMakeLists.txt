#project(notecore)

include_directories(
    ../facility
	)

ADD_LIBRARY(notecore STATIC
	framework.h
	notebase.cpp
	notebase.h
	notecore.cpp
	notecore.h)