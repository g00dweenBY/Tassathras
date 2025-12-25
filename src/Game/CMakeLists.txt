file(GLOB_RECURSE GAME_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

add_executable(Game ${GAME_SOURCES})
set_target_properties(Game PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)
foreach(FILE ${GAME_SOURCES})
 get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)
 file(RELATIVE_PATH REL_DIR "${CMAKE_CURRENT_SOURCE_DIR}" "${PARENT_DIR}")
 string(REPLACE "/" "\\" GROUP_NAME "${REL_DIR}")
 source_group("${GROUP_NAME}" FILES "${FILE}")
endforeach()

target_link_libraries(Game PRIVATE Engine)

set_target_properties(Game PROPERTIES FOLDER "Tassathras")

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Game)

add_custom_command(TARGET Game POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${CMAKE_SOURCE_DIR}/assets"
	"$<TARGET_FILE_DIR:Game>/assets"
)