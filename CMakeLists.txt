cmake_minimum_required(VERSION 3.20)

project(Tassathras)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Engine sources
file(GLOB_RECURSE ENGINE_SOURCES
    src/TreaxEngine/*.cpp
    src/TreaxEngine/**/*.cpp
    src/TreaxEngine/Graphics/stb_image.cpp
)
add_library(TreaxEngine STATIC ${ENGINE_SOURCES})

# Game sources
file(GLOB_RECURSE GAME_SOURCES
    src/Game/*.cpp
    src/Game/**/*.cpp
)

# GLFW
add_subdirectory(external/glfw)

# Glad
add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# Include dirs
target_include_directories(TreaxEngine PUBLIC
    src/TreaxEngine
    external/glad/include
    external/glfw/include
    external/glm
    external/stb
)

# Executable
add_executable(TreaxGame ${GAME_SOURCES})

# Link libraries
target_link_libraries(TreaxGame PRIVATE TreaxEngine glfw glad opengl32)

# Copy assets
set(ASSETS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/assets")
set(ASSETS_BINARY_DIR "${CMAKE_BINARY_DIR}/assets")

add_custom_command(TARGET TreaxGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${ASSETS_SOURCE_DIR}"
    "${ASSETS_BINARY_DIR}"
)
