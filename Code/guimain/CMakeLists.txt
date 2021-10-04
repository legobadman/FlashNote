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
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

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
    ../3rdparty/sqlite
    ../dbservice
    ../notehook
    )

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

link_directories(../3rdparty/boost_1_55_0/stage/lib)

IF (WIN32)
    SET(SHORTCUT_SRCS "./shortcut/gui/qxtwindowsystem.cpp" "./shortcut/gui/qxtwindowsystem.h" "./shortcut/gui/qxtglobalshortcut_win.cpp" "./shortcut/gui/qxtwindowsystem_win.cpp")
    find_package(Qt5 REQUIRED COMPONENTS Widgets Svg)
ENDIF (WIN32)

IF (UNIX)
    find_package(X11)
    find_package(Qt5 REQUIRED COMPONENTS Widgets X11Extras Svg)
    IF (APPLE)
        SET(SHORTCUT_SRCS "./shortcut/gui/qxtglobalshortcut_mac.cpp" "./shortcut/gui/qxtwindowsystem_mac.cpp" "./shortcut/gui/qxtwindowsystem_mac.h") 
    ELSE (APPLE)
        SET(SHORTCUT_SRCS "./shortcut/gui/qxtwindowsystem_x11.cpp" "./shortcut/gui/qxtglobalshortcut_x11.cpp" "./shortcut/gui/x11info.cpp" "./shortcut/gui/x11info.h")
    ENDIF(APPLE)
ENDIF (UNIX)


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
    ./bookmenubutton.cpp
    ./bookmenubutton.h
    ./bookviewmodel.cpp
    ./bookviewmodel.h
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
    ./globalsearcheditor.cpp
    ./globalsearcheditor.h
    ./floatingmenubutton.cpp
    ./floatingmenubutton.h
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
    ./labelbutton.cpp
    ./labelbutton.h
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
    ./mindmap/mindtransaction.cpp
    ./mindmap/mindtransaction.h
    ./mindmap/linknoteitem.cpp
    ./mindmap/linknoteitem.h
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
    ./notecompleter.cpp
    ./notecompleter.h
    ./normal_editor.cpp
    ./normal_editor.h
    ./pathservice.cpp
    ./pathservice.h
    ./popupwidget.cpp
    ./popupwidget.h
    ./res/icons.qrc
    ./res/font.qrc
    ./richeditor/richtexteditor.cpp
    ./richeditor/richtexteditor.h
    ./SeperateLine.cpp
    ./SeperateLine.h
    ./searcheditor.cpp
    ./searcheditor.h
    ./searchitemdelegate.cpp
    ./searchitemdelegate.h
    ./selectnotebookdlg.cpp
    ./selectnotebookdlg.h
    ./selectnotebookdlg.ui
    ./selectnotebookpanel.cpp
    ./selectnotebookpanel.h
    ./screenshot/screenshotwindow.cpp
    ./screenshot/screenshotwindow.h
    ./screenshot/screenwidget.cpp
    ./screenshot/screenwidget.h
    ./screenshot/screentoolbar.cpp
    ./screenshot/screentoolbar.h
    ./shortcut/gui/qxtglobalshortcut.cpp
    ./shortcut/gui/qxtglobalshortcut.h
    ./shortcut/gui/qxtglobalshortcut_p.h
    ${SHORTCUT_SRCS}
    ./shortcut/core/qxtglobal.cpp
    ./shortcut/core/qxtglobal.h
    ./stdafx.cpp
    ./stdafx.h
    ./style/MyStyle.cpp
    ./style/MyStyle.h
    ./style/ToolButtonStyle.cpp
    ./style/ToolButtonStyle.h
    ./style/toolbuttonstyleoption.cpp
    ./style/toolbuttonstyleoption.h
    ./transaction/transaction.cpp
    ./transaction/transaction.h
    ./uiapplication.cpp
    ./uiapplication.h
)

target_precompile_headers(guimain PRIVATE ./stdafx.h)

# Add the Qt5 Widgets for linking
if (WIN32)
	target_link_libraries(guimain notecore Qt5::Widgets Qt5::Svg dbservice sqlite Ws2_32 notehook)
    add_custom_command(TARGET guimain POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory ${BUILD_ROOT}/bin/translations/)
	add_custom_command(TARGET guimain POST_BUILD
		COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/ts/lrelease.exe ${CMAKE_CURRENT_SOURCE_DIR}/ts/uimain.ts -qm ${BUILD_ROOT}/bin/translations/uimain.qm)
else()
	target_link_libraries(guimain notecore Qt5::Widgets Qt5::Svg Qt5::X11Extras dbservice sqlite Threads::Threads dl X11)
	add_custom_command(TARGET guimain POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory ${BUILD_ROOT}/bin/translations/)
    add_custom_command(TARGET guimain POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E env LD_LIBRARY_PATH="${QT_BASE_PATH}/lib" ${QT_BASE_PATH}/bin/lrelease ${CMAKE_CURRENT_SOURCE_DIR}/ts/uimain.ts -qm ${BUILD_ROOT}/bin/translations/uimain.qm)
endif(WIN32)

set_target_properties(guimain PROPERTIES OUTPUT_NAME "flashnote")
