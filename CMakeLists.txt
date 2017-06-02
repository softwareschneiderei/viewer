cmake_minimum_required(VERSION 3.4)
project(epics_viewer)

set(EPICS_BASE "/opt/epics/base" CACHE PATH "Epics base installation")

set(EPICS_INCLUDE_DIRS ${EPICS_BASE}/include ${EPICS_BASE}/include/os/Linux ${EPICS_BASE}/include/compiler/gcc/)
set(EPICS_LIBRARIES ${EPICS_BASE}/lib/linux-x86_64/libca.a ${EPICS_BASE}/lib/linux-x86_64/libCom.a pthread dl readline history)

add_subdirectory(shared)
add_subdirectory(demos)
add_subdirectory(viewer)
add_subdirectory(camera_module)
