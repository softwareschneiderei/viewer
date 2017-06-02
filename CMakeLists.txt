cmake_minimum_required(VERSION 3.4)
project(epics_viewer)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)

add_subdirectory(viewer)
add_subdirectory(camera_module)
