set(TARGET_NAME Viewer)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)

# Tell CMake to create the helloworld executable
add_executable(${TARGET_NAME} WIN32
  Viewer.cpp
  Viewer.h
  Viewer.ui
  Main.cpp
  Main.h
  FunctionEvent.cpp
  FunctionEvent.h
  DisplayWidget.cpp
  DisplayWidget.h
  ImagePoller.cpp
  ImagePoller.h)

# Use the Widgets module from Qt 5.
qt5_use_modules(${TARGET_NAME} Widgets)

target_link_libraries(${TARGET_NAME}
  shared)
