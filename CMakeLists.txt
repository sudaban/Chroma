cmake_minimum_required(VERSION 3.15)
project(ChromaServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(ENET_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/enet")

add_subdirectory(${ENET_DIR} enet_build)

add_executable(ChromaServer
    Chroma/Chroma.cpp
    Chroma/EnetServer.cpp
    Chroma/Log.cpp
    Chroma/inc/EnetServer.h
    Chroma/inc/Log.h
    Chroma/inc/GameUpdatePacket.h
)

target_include_directories(ChromaServer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Chroma/inc
    ${ENET_DIR}/include
)

target_link_libraries(ChromaServer PRIVATE enet)

set_target_properties(ChromaServer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/x64/Release"
)
