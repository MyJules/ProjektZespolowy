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
    set(OpenCV_DIR vendor/opencvAndroid/sdk/native/jni)
    set(OPENCV_ANDROID_PATH /home/andrew/MY/PWR/ProjektZespolowy/ProjektZespolowy/ImgDetect/vendor/opencvAndroid)

    include_directories(${OPENCV_ANDROID_PATH}/sdk/native/jni/include)

    link_directories(${OPENCV_ANDROID_PATH}/sdk/native/libs/arm64-v8a)
    link_directories(${OPENCV_ANDROID_PATH}/sdk/native/3rdparty/libs/arm64-v8a)

    find_package(OpenCV REQUIRED)
else()
    include_directories(${OpenCV_INCLUDE_DIRS})
    find_package(OpenCV REQUIRED)
endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwidget.cpp
        mainwidget.h
        videocapture.cpp
        videocapture.h
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

target_link_libraries(ImgDetect PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(ImgDetect PRIVATE ${OpenCV_LIBS})

if(ANDROID)
    set(ANDROID_EXTRA_LIBS
        ${OPENCV_ANDROID_PATH}/sdk/native/libs/arm64-v8a/libopencv_java4.so
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
