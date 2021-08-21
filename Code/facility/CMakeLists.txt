find_package(Qt5 REQUIRED COMPONENTS Widgets)

ADD_LIBRARY(facility STATIC
	com_sptr.cpp
	com_sptr.h
	common_define.h
	iunknown.h)

target_link_libraries(facility Qt5::Widgets)
