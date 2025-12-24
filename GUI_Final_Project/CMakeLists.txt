cmake_minimum_required(VERSION 3.16)

project(XML VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(XML
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        comp_decomp.h error_check.h errorFixing.h mainwindow.h minfiy.h Mutual.h Post.h prettify.h SearchTopic.h SearchWord.h SocialNetwork.h Suggest.h Token.h Tree.h User.h XMLNode.h XMLParser.h XMLToJSONConverter.h XMLTokenizer.h XMLTreeBuilder.h
        comp_decomp.cpp error_check.cpp errorFixing.cpp main.cpp mainwindow.cpp minfiy.cpp Mutual.cpp Post.cpp prettify.cpp SearchTopic.cpp SearchWord.cpp SocialNetwork.cpp Suggest.cpp User.cpp XMLNode.cpp XMLParser.cpp XMLToJSONConverter.cpp XMLTokenizer.cpp XMLTreeBuilder.cpp
        LauncherWindow.cpp
        LauncherWindow.h
        main.cpp
        launcherwindow.ui
        ActiveUser.h comp_decomp.h error_check.h errorFixing.h GraphVisualizer.h Influencer.h LauncherWindow.h mainwindow.h minfiy.h Mutual.h Post.h prettify.h SearchTopic.h SearchWord.h SocialNetwork.h Suggest.h Token.h Tree.h User.h XMLNode.h XMLParser.h XMLToJSONConverter.h XMLTokenizer.h XMLTreeBuilder.h XMLTrreeBuilder.h
        ActiveUser.cpp comp_decomp.cpp error_check.cpp errorFixing.cpp GraphVisualizer.cpp Influencer.cpp LauncherWindow.cpp main.cpp mainwindow.cpp minfiy.cpp Mutual.cpp Post.cpp prettify.cpp SearchTopic.cpp SearchWord.cpp SocialNetwork.cpp Suggest.cpp User.cpp XMLNode.cpp XMLParser.cpp XMLToJSONConverter.cpp XMLTokenizer.cpp XMLTreeBuilder.cpp
        Level2Window.ui
        Level2Window.h
        level2window.h
        level2window.cpp
        Level2Window.cpp
        ActiveUser.h comp_decomp.h error_check.h errorFixing.h GraphVisualizer.h Influencer.h LauncherWindow.h Level2Window.h mainwindow.h minfiy.h Mutual.h Post.h prettify.h SearchTopic.h SearchWord.h SocialNetwork.h Suggest.h Token.h Tree.h User.h XMLNode.h XMLParser.h XMLToJSONConverter.h XMLTokenizer.h XMLTreeBuilder.h XMLTrreeBuilder.h
        ActiveUser.cpp comp_decomp.cpp error_check.cpp errorFixing.cpp GraphVisualizer.cpp Influencer.cpp LauncherWindow.cpp Level2Window.cpp main.cpp mainwindow.cpp minfiy.cpp Mutual.cpp Post.cpp prettify.cpp SearchTopic.cpp SearchWord.cpp SocialNetwork.cpp Suggest.cpp User.cpp XMLNode.cpp XMLParser.cpp XMLToJSONConverter.cpp XMLTokenizer.cpp XMLTreeBuilder.cpp
        error_check.cpp errorFixing.cpp mainwindow.cpp
        error_check.h errorFixing.h mainwindow.h
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET XML APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(XML SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(XML
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(XML PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.XML)
endif()
set_target_properties(XML PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS XML
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(XML)
endif()
