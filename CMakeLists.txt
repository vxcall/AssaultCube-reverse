cmake_minimum_required(VERSION 3.16)
project(ACR)

set(CMAKE_CXX_STANDARD 17)

FILE(GLOB_RECURSE MAIN_SOURCE CONFIGURE_DEPENDS ${CMAKE_CURRENT_LIST_DIR}/AssaultCube-reverse/*.cpp)
FILE(GLOB_RECURSE MAIN_HEADER CONFIGURE_DEPENDS ${CMAKE_CURRENT_LIST_DIR}/AssaultCube-reverse/*.h)

include_directories(${CMAKE_CURRENT_LIST_DIR}/MSDetour/include)
link_directories(${CMAKE_CURRENT_LIST_DIR}/MSDetour/binary)

include_directories(${CMAKE_CURRENT_LIST_DIR}/MinHook/include)
link_directories(${CMAKE_CURRENT_LIST_DIR}/MinHook/bin)

add_library(ACR SHARED ${MAIN_SOURCE} ${MAIN_HEADER})

target_link_libraries(${PROJECT_NAME} detours MinHook.x86)