cmake_minimum_required(VERSION 3.5.1)

if (NOT DEFINED ${CMAKE_BUILD_TYPE})
	SET(CMAKE_BUILD_TYPE Debug)
endif()

SET(BUILD_ROOT ../)

if (WIN32)
	list(APPEND CMAKE_PREFIX_PATH "E:/Qt/qtbase/lib/cmake")
else()
	list(APPEND CMAKE_PREFIX_PATH "/home/luzh/Qt/install_shared/lib/cmake")
endif(WIN32)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BUILD_ROOT}/bin)


project(FlashNote)

add_subdirectory(guimain)
add_subdirectory(notecore)
add_subdirectory(dbservice)
add_subdirectory(3rdparty)
add_subdirectory(facility)

if (WIN32)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT guimain)
	set_property(TARGET guimain PROPERTY VS_NO_SOLUTION_DEPLOY OFF)
	set_property(TARGET notecore PROPERTY VS_NO_SOLUTION_DEPLOY OFF)
endif(WIN32)
