cmake_minimum_required(VERSION 3.8.2)

if (NOT DEFINED ${CMAKE_BUILD_TYPE})
	SET(CMAKE_BUILD_TYPE Debug)
endif()

SET(BUILD_ROOT ../)

list(APPEND CMAKE_PREFIX_PATH "C:/Qt/Qt-5.15.0/lib/cmake")

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BUILD_ROOT}/bin)

project(FlashNote)

add_subdirectory(guimain)
add_subdirectory(notecore)
#add_subdirectory(3rdparty)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT guimain)
set_property(TARGET guimain PROPERTY VS_NO_SOLUTION_DEPLOY OFF)
set_property(TARGET notecore PROPERTY VS_NO_SOLUTION_DEPLOY OFF)