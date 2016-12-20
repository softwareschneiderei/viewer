
set(TARGET_NAME shared)


add_library(${TARGET_NAME}
  Channel.cpp
  Channel.h
  Device.cpp
  Device.h)


target_include_directories(${TARGET_NAME}
  PUBLIC ${EPICS_INCLUDE_DIRS}
  INTERFACE ./)

target_link_libraries(${TARGET_NAME}
  PUBLIC ${EPICS_LIBRARIES})

target_compile_options(${TARGET_NAME}
  PUBLIC -std=c++11)
