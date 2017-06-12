

option(VIEWER_EPICS_PLUGIN "Enable epics plugin"
  FALSE)

add_subdirectory(common)
add_subdirectory(emulated)
if(${VIEWER_EPICS_PLUGIN})
  add_subdirectory(epics)
endif()
add_subdirectory(uca)