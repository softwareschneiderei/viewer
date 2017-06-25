set(TARGET_NAME Viewer)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Tell CMake to create the helloworld executable
add_executable(${TARGET_NAME} WIN32
  Viewer.cpp
  Viewer.h
  Viewer.ui
  Main.cpp
  Main.h
  CameraModuleFactory.cpp
  CameraModuleFactory.h
  FunctionEvent.cpp
  FunctionEvent.h
  DisplayWidget.cpp
  DisplayWidget.h)

# Use the Widgets module from Qt 5.
# qt5_use_modules(${TARGET_NAME} Widgets)

target_compile_definitions(${TARGET_NAME}
  PUBLIC -DVIEWER_VERSION=${VERSION})
target_link_libraries(${TARGET_NAME}
  PUBLIC camera_common
  PUBLIC camera_emulated
  PUBLIC camera_uca
  PUBLIC Qt5::Widgets)

if(${VIEWER_EPICS_PLUGIN})
  target_link_libraries(${TARGET_NAME}
    PUBLIC camera_epics)
endif()