cmake_minimum_required(VERSION 2.8)

project (latino-json)

add_library(latino-json SHARED src/latino-json.c)
#optional install in /usr/lib
install(FILES liblatino-json.so DESTINATION /usr/lib)
