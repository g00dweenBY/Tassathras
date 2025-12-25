file(GLOB_RECURSE ENGINE_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

add_library(Engine STATIC ${ENGINE_SOURCES})

foreach(FILE ${ENGINE_SOURCES})
 get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)
 file(RELATIVE_PATH REL_DIR "${CMAKE_CURRENT_SOURCE_DIR}" "${PARENT_DIR}"
 string(REPLACE "/" "\\" GROUP_NAME "${REL_DIR}")
 source_group("${GROUP_NAME}" FILES "${FILE})
endforeach();

target_include_directories(Engine PUBLIC "${CMAKE_SOURCE_DIR}/src")

target_link_libraries(Engine PUBLIC
	glad
	glfw
	glm
	entt
	spdlog
	imgui
	imgui-glfw
	imgui-opengl3
	stb
)

set_target_properties(Engine PROPERTIES FOLDER "Tassathras")