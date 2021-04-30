cmake_minimum_required(VERSION 3.8.2)

project(FlashNote)

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# There may be a way to tell up front if Qt5 is going to be found, but I haven't found
# a foolproof way to do it yet, so settle for the default error message for now.
#if(NOT CMAKE_PREFIX_PATH AND NOT Qt5Widgets_DIR)
#    message(WARNING "CMAKE_PREFIX_PATH is not defined, so find_package may not work. Set the CMAKE_PREFIX_PATH "
#            "environment variable to the install prefix of Qt 5, either on the command line as "
#            "-DCMAKE_PREFIX_PATH=\"path/to/Qt5/lib/cmake\" or with set(CMAKE_PREFIX_PATH path/to/Qt5/lib/cmake)")
#endif(NOT CMAKE_PREFIX_PATH AND NOT Qt5Widgets_DIR)

# Add a compiler flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Make this a GUI application on Windows
if(WIN32)
  set(CMAKE_WIN32_EXECUTABLE ON)
endif()

include_directories(
    ./comctrl/
    ./style/
    ./richeditor/
    ./style/
    ../notecore)

# Find the QtWidgets library
find_package(Qt5 REQUIRED COMPONENTS Widgets)

# Tell CMake to create the helloworld executable
add_executable(FlashNote 
    ./alignmentcombobox.cpp
    ./alignmentcombobox.h
    ./colorgallery.cpp
    ./colorgallery.h
    ./coloritemdelegate.cpp
    ./coloritemdelegate.h
    ./colortableview.cpp
    ./colortableview.h
    ./comctrl/menubutton.cpp
    ./comctrl/menubutton.h
    ./comctrl/toolbutton.cpp
    ./comctrl/toolbutton.h
    ./DrawerFunc.cpp
    ./DrawerFunc.h
    ./FlashNote.cpp
    ./FontColorComboBox.cpp
    ./FontColorComboBox.h
    ./fontsize_combobox.cpp
    ./fontsize_combobox.h
    ./font_combobox.cpp
    ./font_combobox.h
    ./FramelessLineEdit.cpp
    ./FramelessLineEdit.h
    ./LeftSideItemDelegate.cpp
    ./LeftSideItemDelegate.h
    ./listpane.cpp
    ./listpane.h
    ./NoteMainWindow.cpp
    ./NoteMainWindow.h
    ./NoteMainWindow.ui
    ./note_editwindow.cpp
    ./note_editwindow.h
    ./note_editwindow.ui
    ./popupwidget.cpp
    ./popupwidget.h
    ./res/icons.qrc
    ./richeditor/mrichtextedit.cpp
    ./richeditor/mrichtextedit.h
    ./richeditor/mrichtextedit.ui
    ./richeditor/mtextedit.cpp
    ./richeditor/mtextedit.h
    ./richeditor/richtexteditor.cpp
    ./richeditor/richtexteditor.h
    ./SeperateLine.cpp
    ./SeperateLine.h
    ./stdafx.cpp
    ./stdafx.h
    ./style/MyStyle.cpp
    ./style/MyStyle.h
    ./style/ToolButtonStyle.cpp
    ./style/ToolButtonStyle.h
    ./style/toolbuttonstyleoption.cpp
    ./style/toolbuttonstyleoption.h
)

# Add the Qt5 Widgets for linking
target_link_libraries(FlashNote Qt5::Widgets)
