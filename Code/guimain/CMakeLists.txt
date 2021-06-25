cmake_minimum_required(VERSION 3.8.2)

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
    ../notecore
    ../facility
    ../server/gen-cpp
    ../rpcservice
    ../include
    ../3rdparty
	../3rdparty/boost_1_55_0
    ../3rdparty/rapidxml
    )

# Find the QtWidgets library
find_package(Qt5 REQUIRED COMPONENTS Widgets)

link_directories(../3rdparty/boost_1_55_0/stage/lib)

# Tell CMake to create the helloworld executable
add_executable(guimain
    ./addbookdlg.cpp
    ./addbookdlg.h
    ./addbookdlg.ui
    ./alignmentcombobox.cpp
    ./alignmentcombobox.h
    ./booklistview.cpp
    ./booklistview.h
    ./booklistview.ui
    ./bookviewmodel.cpp
    ./bookviewmodel.h
    ./bookmenubutton.cpp
    ./bookmenubutton.h
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
    ./common_types.h
    ./DrawerFunc.cpp
    ./DrawerFunc.h
    ./guimain.cpp
    ./guihelper.cpp
    ./guihelper.h
    ./fontitemdelegate.cpp
    ./fontitemdelegate.h
    ./FontColorComboBox.cpp
    ./FontColorComboBox.h
    ./fontsize_combobox.cpp
    ./fontsize_combobox.h
    ./font_combobox.cpp
    ./font_combobox.h
    ./FramelessLineEdit.cpp
    ./FramelessLineEdit.h
    ./highlighter/cppsyntaxhighlighter.cpp
    ./highlighter/cppsyntaxhighlighter.h
    ./LeftSideItemDelegate.cpp
    ./LeftSideItemDelegate.h
    ./listpane.cpp
    ./listpane.h
    ./mindmap/mindmapview.cpp
    ./mindmap/mindmapview.h
    ./mindmap/mindmapwidget.cpp
    ./mindmap/mindmapwidget.h
    ./mindmap/mindnode.cpp
    ./mindmap/mindnode.h
    ./mindmap/mindmapscene.cpp
    ./mindmap/mindmapscene.h
    ./mindmap/mindnodebutton.cpp
    ./mindmap/mindnodebutton.h
    ./mindmap/mindprogressnode.cpp
    ./mindmap/mindprogressnode.h
    ./model/notebookmodel.cpp
    ./model/notebookmodel.h
    ./newitemdelegate.cpp
    ./newitemdelegate.h
    ./newnotewindow.cpp
    ./newnotewindow.h
    ./newnotewindow.ui
    ./NoteMainWindow.cpp
    ./NoteMainWindow.h
    ./NoteMainWindow.ui
    ./note_editwindow.cpp
    ./note_editwindow.h
    ./note_editwindow.ui
    ./notelistview.cpp
    ./notelistview.h
    ./noteitemdelegate.cpp
    ./noteitemdelegate.h
    ./noteseditview.cpp
    ./noteseditview.h
    ./notetrayicon.cpp
    ./notetrayicon.h
    ./notewinservice.cpp
    ./notewinservice.h
    ./normal_editor.cpp
    ./normal_editor.h
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
    ./searcheditor.cpp
    ./searcheditor.h
    ./selectnotebookdlg.cpp
    ./selectnotebookdlg.h
    ./selectnotebookdlg.ui
    ./selectnotebookpanel.cpp
    ./selectnotebookpanel.h
    ./stdafx.cpp
    ./stdafx.h
    ./style/MyStyle.cpp
    ./style/MyStyle.h
    ./style/ToolButtonStyle.cpp
    ./style/ToolButtonStyle.h
    ./style/toolbuttonstyleoption.cpp
    ./style/toolbuttonstyleoption.h
    ./typography/customtextdocument.cpp
    ./typography/customtextdocument.h
    ./typography/codetextframe.cpp
    ./typography/codetextframe.h

    #qt试验代码
    ./qtexamples/MltpDlgs1.cpp
    ./qtexamples/MltpDlgs1.h
    ./qtexamples/MltpDlgs2.cpp
    ./qtexamples/MltpDlgs2.h

    #thrift生成的类型
	#../server/gen-cpp/UserInfo.cpp
	#../server/gen-cpp/NoteInfo.cpp
	#../server/gen-cpp/notestructure_constants.cpp
	#../server/gen-cpp/notestructure_types.cpp
)

target_precompile_headers(guimain PRIVATE ./stdafx.h)

# Add the Qt5 Widgets for linking
target_link_libraries(guimain notecore Qt5::Widgets rpcservice)

set_target_properties(guimain PROPERTIES OUTPUT_NAME "flashnote")

SET(ASSETS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/assets)
file(MAKE_DIRECTORY ${ASSETS})
add_custom_target(build-time-make-directory ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${ASSETS})
