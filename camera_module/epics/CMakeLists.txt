
set(EPICS_BASE "/opt/epics/base" CACHE PATH "Epics base installation")

set(EPICS_INCLUDE_DIRS ${EPICS_BASE}/include ${EPICS_BASE}/include/os/Linux ${EPICS_BASE}/include/compiler/gcc/)
set(EPICS_LIBRARIES ${EPICS_BASE}/lib/linux-x86_64/libca.a ${EPICS_BASE}/lib/linux-x86_64/libCom.a pthread dl readline history)

set(TARGET_NAME camera_epics)


add_library(${TARGET_NAME} SHARED
  Channel.cpp
  Channel.h
  Device.cpp
  Device.h
  EpicsImagePoller.cpp
  EpicsImagePoller.h
  EpicsConfigure.ui)


target_include_directories(${TARGET_NAME}
  PUBLIC ${EPICS_INCLUDE_DIRS}
  INTERFACE ./)

target_link_libraries(${TARGET_NAME}
  PUBLIC ${EPICS_LIBRARIES}
  PUBLIC camera_common)

target_compile_options(${TARGET_NAME}
  PUBLIC -std=c++11)
