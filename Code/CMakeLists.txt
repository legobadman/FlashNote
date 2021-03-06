cmake_minimum_required(VERSION 3.5.1)

if (NOT DEFINED CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE Debug)
endif()

SET(BUILD_ROOT ${CMAKE_CURRENT_BINARY_DIR})

if (WIN32)
	if (CMAKE_BUILD_TYPE STREQUAL "Debug")
		list(APPEND CMAKE_PREFIX_PATH "E:/Qt/qtbase/lib/cmake")
		list(APPEND CMAKE_PREFIX_PATH "E:/Qt/qtsvg/lib/cmake")
	else()
		list(APPEND CMAKE_PREFIX_PATH "E:/Qt_bin/lib/cmake")
	endif()
else()
	if (CMAKE_BUILD_TYPE STREQUAL "Debug")
		SET(QT_BASE_PATH "/home/luzh/Qt/install_shared")
		SET(QT_SVG_PATH "/home/luzh/Qt/install_shared")
	else()
		SET(QT_BASE_PATH "/home/luzh/Qt/install_release_shared")
		SET(QT_SVG_PATH "/home/luzh/Qt/install_release_shared")
	endif()
	list(APPEND CMAKE_PREFIX_PATH "${QT_BASE_PATH}/lib/cmake")
	list(APPEND CMAKE_PREFIX_PATH "${QT_SVG_PATH}/lib/cmake")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BUILD_ROOT}/bin)
else()
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BUILD_ROOT}/bin)
endif()

project(FlashNote)

add_subdirectory(guimain)
add_subdirectory(notecore)
add_subdirectory(dbservice)
add_subdirectory(3rdparty)
add_subdirectory(facility)
if (WIN32)
	add_subdirectory(notehook)
endif(WIN32)

if (WIN32)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT guimain)
	set_property(TARGET guimain PROPERTY VS_NO_SOLUTION_DEPLOY OFF)
	set_property(TARGET notecore PROPERTY VS_NO_SOLUTION_DEPLOY OFF)
endif(WIN32)
