################################################################################
# Para mas información de este archivo visite la pagina de cmake:
# https://cmake.org/documentation/
################################################################################

#Establece la versión mínima requerida de cmake para un proyecto.
cmake_minimum_required(VERSION 3.0.2)

#Establece un nombre para el proyecto
project (latino-json)

#Agrega un subdirectorio a la compilación.
add_subdirectory(jansson)

#Añade directorios de inclusión a la compilación
include_directories(jansson/include)
include_directories(jansson/src)
include_directories(include)

#Agrega una biblioteca al proyecto utilizando los
#archivos de origen especificados.
add_library(latino-json SHARED src/latino-json.c)

#Vincula un objetivo a bibliotecas determinadas.
target_link_libraries(latino-json jansson latino)

#Especifica las reglas que se ejecutarán en el momento de la instalación.
install(FILES liblatino-json.so DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
