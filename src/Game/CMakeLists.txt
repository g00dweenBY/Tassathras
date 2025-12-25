file(GLOB_RECURSE ENGINE_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

add_executable(Game ${GAME_SOURCES})

foreach(FILE ${GAME_SOURCES})
 get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)
 file(RELATIVE_PATH REL_DIR "${CMAKE_CURRENT_SOURCE_DIR}" "${PARENT_DIR}"
 string(REPLACE "/" "\\" GROUP_NAME "${REL_DIR}")
 source_group("${GROUP_NAME}" FILES "${FILE})
endforeach();

target_link_libraries(Game PRIVATE Engine)

set_target_properties(Game PROPERTIES FOLDER "Tassathras")

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Game)