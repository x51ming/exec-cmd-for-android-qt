cmake_minimum_required(VERSION 3.16)

project(Terminal VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)

qt_standard_project_setup()

file(GLOB_RECURSE JAVA_FILES
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    android/*.java
    android/*.xml)

qt_add_executable(appTerminal
    main.cpp
    ${JAVA_FILES}
    MANUAL_FINALIZATION
)

qt_add_qml_module(appTerminal
    URI Terminal
    VERSION 1.0
    QML_FILES
        Main.qml
        Commands.qml
        ScrollablePage.qml
        QTerm.qml
    SOURCES qlog.h qlog.cpp
    SOURCES jni_utils.cpp
    SOURCES jni_utils.h
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appTerminal PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appTerminal
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appTerminal
    PRIVATE Qt6::Quick Qt6::CorePrivate
)

include(GNUInstallDirs)
install(TARGETS appTerminal
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

set_property(TARGET appTerminal APPEND PROPERTY
    QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android
)
qt_finalize_executable(appTerminal)
