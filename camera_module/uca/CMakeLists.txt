
find_package(PkgConfig)
pkg_check_modules(GLIB2 glib-2.0>=2.24 REQUIRED)
pkg_check_modules(GOBJECT2 gobject-2.0>=2.24 REQUIRED)
pkg_check_modules(UCA libuca>=1.3.0 REQUIRED)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

link_directories(
  ${UCA_LIBRARY_DIRS}
)

set(TARGET_NAME camera_uca)


add_library(${TARGET_NAME} SHARED
  UcaImagePoller.cpp
  UcaImagePoller.h
  UcaConfigure.cpp
  UcaConfigure.h
  UcaState.cpp
  UcaState.h
  UcaTableModel.cpp
  UcaTableModel.h
  UcaConfigure.ui)


target_include_directories(${TARGET_NAME}
  PUBLIC ${GOBJECT2_INCLUDE_DIRS}
  PUBLIC ${GLIB2_INCLUDE_DIRS}
  PUBLIC ${UCA_INCLUDE_DIRS}
  INTERFACE ./)

target_link_libraries(${TARGET_NAME}
  PUBLIC ${EPICS_LIBRARIES}
  PUBLIC camera_common
  PUBLIC ${UCA_LIBRARIES}
  PUBLIC ${GLIB2_LIBRARIES}
  PUBLIC ${GOBJECT2_LIBRARIES})

target_compile_options(${TARGET_NAME}
  PUBLIC -std=c++11)
