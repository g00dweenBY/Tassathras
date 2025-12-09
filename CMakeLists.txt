cmake_minimum_required(VERSION 3.20)

project(Tassathras)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Engine sources
file(GLOB_RECURSE ENGINE_SOURCES
    src/Engine/*.cpp
    src/Engine/**/*.cpp
    src/Engine/Graphics/stb_image.cpp
)
add_library(Engine STATIC ${ENGINE_SOURCES} "src/Engine/Graphics/VertexArray.h" "src/Engine/Graphics/VertexArray.cpp")

# Game sources
file(GLOB_RECURSE GAME_SOURCES
    src/Game/*.cpp
    src/Game/**/*.cpp
)

# GLFW
add_subdirectory(external/glfw)

# Glad
add_library(glad STATIC external/glad/src/glad.c "src/Engine/Graphics/VertexArray.h" "src/Engine/Graphics/VertexArray.cpp")
target_include_directories(glad PUBLIC external/glad/include)

# Include dirs
target_include_directories(Engine PUBLIC
    src/Engine
    external/glad/include
    external/glfw/include
    external/glm
    external/stb
)

# Executable
add_executable(TreaxGame ${GAME_SOURCES} "src/Engine/Graphics/VertexArray.h" "src/Engine/Graphics/VertexArray.cpp")

# Link libraries
target_link_libraries(TreaxGame PRIVATE Engine glfw glad opengl32)

# Copy assets
set(ASSETS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/assets")
set(ASSETS_BINARY_DIR "${CMAKE_BINARY_DIR}/assets")

add_custom_command(TARGET TreaxGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${ASSETS_SOURCE_DIR}"
    "${ASSETS_BINARY_DIR}"
)
