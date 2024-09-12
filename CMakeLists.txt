cmake_minimum_required(VERSION 3.5)

project(donut)

set(lista_archivos
  main.cpp
)

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  MESSAGE("Clang")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  MESSAGE("GNU")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  MESSAGE("Intel")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  MESSAGE("MSVC")
endif()

#Debug/Release

set(CMAKE_BUILD_TYPE Debug)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -Wall -Wextra")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -pedantic -Wall -W -Wconversion -Wshadow -Wcast-qual -Wwrite-strings -O0 -ggdb3 -DDEBUG -std=gnu++17")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -std=gnu++17")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} /W3 /std:c++17")
else()
    message(WARNING "Compiler no reconocido. No se establecen directrices.")
endif()

add_executable(donut ${lista_archivos})

install(TARGETS donut RUNTIME DESTINATION bin)
