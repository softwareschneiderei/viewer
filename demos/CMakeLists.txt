
set(TARGET_NAME Demo)

add_executable(${TARGET_NAME} Demo.cpp)

target_link_libraries(${TARGET_NAME}
  PUBLIC shared)
