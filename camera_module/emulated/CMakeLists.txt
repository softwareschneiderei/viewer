

set(TARGET_NAME camera_emulated)

add_library(${TARGET_NAME}
  EmulatedImagePoller.cpp
  EmulatedImagePoller.h
  EmulatedConfigure.ui)

target_include_directories(${TARGET_NAME}
  INTERFACE ./)

target_link_libraries(${TARGET_NAME}
  PUBLIC camera_common)
