cmake_minimum_required(VERSION 2.8)

project (latino-json)

add_subdirectory(jansson)
include_directories(jansson/include)

add_library(latino-json SHARED src/latino-json.c)

link_libraries(jansson latino)

target_link_libraries(latino-json jansson latino)
#optional install in /usr/lib
install(FILES liblatino-json.so DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
