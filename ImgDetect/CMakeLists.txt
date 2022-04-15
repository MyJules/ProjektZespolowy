cmake_minimum_required(VERSION 3.5)

project(ImgDetect VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if(ANDROID)
    set(OpenCV_STATIC ON)
    set(OPENCV_ANDROID_PATH vendor/opencvAndroid)
    set(OpenCV_DIR ${OPENCV_ANDROID_PATH}/sdk/native/jni)
    set(ANDROID_ARCHITECTURE arm64-v8a)

    include_directories(${OPENCV_ANDROID_PATH}/sdk/native/jni/include)

    link_directories(${OPENCV_ANDROID_PATH}/sdk/native/libs/${ANDROID_ARCHITECTURE})
    link_directories(${OPENCV_ANDROID_PATH}/sdk/native/3rdparty/libs/${ANDROID_ARCHITECTURE})

    find_package(OpenCV REQUIRED)
else()
    include_directories(${OpenCV_INCLUDE_DIRS})
    find_package(OpenCV REQUIRED)
endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Multimedia REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Multimedia REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwidget.cpp
        mainwidget.h
        videocapture.cpp
        videocapture.h
        cvutils.cpp
        cvutils.h
        mainwidget.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ImgDetect
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ImgDetect APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(ImgDetect SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(ImgDetect
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(ImgDetect PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Multimedia)
target_link_libraries(ImgDetect PRIVATE ${OpenCV_LIBS})

if(ANDROID)
    set(ANDROID_EXTRA_LIBS
        ${OPENCV_ANDROID_PATH}/sdk/native/libs/${ANDROID_ARCHITECTURE}/libopencv_java4.so
    CACHE INTERNAL "")
endif()

set_target_properties(ImgDetect PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ImgDetect)
endif()
