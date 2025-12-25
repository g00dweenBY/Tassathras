cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(Tassathras LANGUAGES C CXX VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

if(MSVC)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded<$<CONFIG:Debug>:Debug>")
add_compile_options(/MP)
add_compile_options(/W3)
endif()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external)
add_subdirectory(src/Engine)
add_subdirectory(src/Game)

if(TARGET Game)
add_custom_command(TARGET Game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:Game>/assets"
        COMMENT "Copying assets to output directory..."
	)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)