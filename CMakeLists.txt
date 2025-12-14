cmake_minimum_required(VERSION 3.20)

project(Tassathras)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



# Engine sources
file(GLOB_RECURSE ENGINE_SOURCES
    src/Engine/*.cpp
    src/Engine/**/*.cpp

)
add_library(Engine STATIC ${ENGINE_SOURCES})   

# Game sources
file(GLOB_RECURSE GAME_SOURCES
    src/Game/*.cpp
    src/Game/**/*.cpp
)

# Include dirs
target_include_directories(Engine PUBLIC
    src/Engine
	${PROJECT_SOURCE_DIR}/external
	)

# Executable
add_executable(TreaxGame ${GAME_SOURCES})
target_compile_definitions(Engine PUBLIC GLM_ENABLE_EXPERIMENTAL)
target_compile_definitions(TreaxGame PRIVATE GLM_ENABLE_EXPERIMENTAL)

find_package(EnTT CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

# Link libraries
target_link_libraries(Engine PUBLIC
	
	EnTT::EnTT
	glm::glm
	spdlog::spdlog
	glad::glad
	glfw
	opengl32
	)
target_link_libraries(TreaxGame PRIVATE Engine)
# Copy assets
set(ASSETS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/assets")
set(ASSETS_BINARY_DIR "${CMAKE_BINARY_DIR}/assets")

add_custom_command(TARGET TreaxGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${ASSETS_SOURCE_DIR}"
    "${ASSETS_BINARY_DIR}"
)


add_custom_command(
    TARGET TreaxGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:TreaxGame>/assets" # Копируем в D:/Work/Tassathras/out/Debug/assets
    COMMENT "Copying assets to runtime directory"
)