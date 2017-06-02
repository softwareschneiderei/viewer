
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)

set(TARGET_NAME camera_common)

add_library(${TARGET_NAME}
  AbstractImagePoller.cpp
  AbstractImagePoller.h)

target_include_directories(${TARGET_NAME}
  INTERFACE ./)

target_compile_options(${TARGET_NAME}
  PUBLIC -std=c++11)

target_link_libraries(${TARGET_NAME}
  PUBLIC Qt5::Widgets)
