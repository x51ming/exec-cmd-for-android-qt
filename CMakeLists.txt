cmake_minimum_required(VERSION 3.16)

project(Terminal VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Core Quick QuickControls2)

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

target_link_libraries(appTerminal
    PRIVATE Qt6::Quick Qt6::CorePrivate
            Qt6::QuickControls2 Qt6::Core
)

set_property(TARGET appTerminal APPEND PROPERTY
    QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android
)
qt_add_resources(appTerminal "config"
    PREFIX "/"
    FILES
        "qtquickcontrols2.conf"
)
qt_finalize_executable(appTerminal)
