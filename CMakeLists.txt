cmake_minimum_required(VERSION 3.4)
project(Viewer)

set(VERSION 0.1)

option(VIEWER_EPICS_PLUGIN "Enable epics plugin"
  FALSE)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)

add_subdirectory(viewer)
add_subdirectory(camera_module)
