#project(notecore)

include_directories(
    ../facility
	)

ADD_LIBRARY(notecore STATIC
	framework.h
	dllmain.cpp
	guiddefine.h
	guiddefine.cpp
	notebase.cpp
	notebase.h
	notecontainer.h
	notecore.h
	notecoreinit.cpp
	notecoreinit.h
	notecore_c.c
	notecore_i.c
	notecore_p.c)