cmake_minimum_required(VERSION 3.5)

project(Fangorn VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

#force C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#-------------------
# Configure BT.CPP
#-------------------
set(buildBtManually true) #TODO evaluate this based on presence of ros2 btcpp pkg
if(buildBtManually)
    message(STATUS "No BehaviorTree.CPP found on system. It will build manually.")
    add_subdirectory(btcpp)
    set(fangorn_include_dirs ${fangorn_include_dirs} btcpp/include)
endif()

#-----------------------
# Configure node editor
#-----------------------
add_subdirectory(nodeeditor)
set(fangorn_include_dirs ${fangorn_include_dirs} nodeeditor/include)

#--------------------
# Configure Fangorn
#--------------------

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Xml REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Xml REQUIRED)

file(GLOB view_sources src/fangorn/view/*.cpp)
file(GLOB view_uis src/fangorn/view/*.ui)
file(GLOB view_includes include/fangorn/view/*.h)
file(GLOB control_sources src/fangorn/control/*.cpp)
file(GLOB model_sources src/fangorn/model/*.cpp)
file(GLOB util_sources src/fangorn/util/*.cpp)

set(PROJECT_SOURCES
    src/fangorn/fangorn.cpp
    ${view_sources}
    ${view_uis}
    ${view_includes}
    ${control_sources}
    ${model_sources}
    ${util_sources}
)

#
# create library for executables
#
add_library(fangorn_lib SHARED
    ${PROJECT_SOURCES})

#
# point to our include files
#
set(fangorn_include_dirs ${fangorn_include_dirs} ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(fangorn_lib PUBLIC
    ${fangorn_include_dirs})

#
# link widgets to library
#
target_link_libraries(fangorn_lib PUBLIC Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Xml)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Fangorn
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

#
# FANGORN EXECUTABLE
#

# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Fangorn APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    add_executable(Fangorn
        src/fangorn/main.cpp
    )
endif()

#link library to executable
target_link_libraries(Fangorn PUBLIC fangorn_lib behaviortree_cpp QtNodes)

set_target_properties(Fangorn PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Fangorn)
endif()

file(GLOB icon_files src/fangorn/view/icons/*.svg)
file(COPY ${icon_files}
    DESTINATION icons)

#
# TESTING EXECUTABLE
#
if(FANGRON_ENABLE_TESTS)
    message(STATUS "Fangorn tests are enabled.")
    find_package(GTest REQUIRED)
    file(GLOB model_tests test/fangorn/model/*.cpp)
    file(GLOB control_tests test/fangorn/control/*.cpp)
    file(GLOB util_tests test/fangorn/util/*.cpp)

    add_executable(FangornRunTests
        ${model_tests}
        ${control_tests}
        ${util_tests}
        test/fangorn/FangornRunTests.cpp)
    
    target_link_libraries(FangornRunTests PUBLIC fangorn_lib GTest::gtest)
endif()

