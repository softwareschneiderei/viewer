


add_subdirectory(common)
add_subdirectory(emulated)
if(${VIEWER_EPICS_PLUGIN})
  add_subdirectory(epics)
endif()
add_subdirectory(uca)