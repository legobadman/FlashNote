cmake_minimum_required(VERSION 3.8.2)

list(APPEND CMAKE_PREFIX_PATH "C:/Qt/Qt-5.15.0/lib/cmake")

project(FlashNote)

add_subdirectory(FlashNote)
add_subdirectory(notecore)
#add_subdirectory(3rdparty)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT FlashNote)