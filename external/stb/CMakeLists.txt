add_library(stb STATIC
	"${CMAKE_CURREND_LIST_DIR}/include/stb_image.h"
	"${CMAKE_CURREND_LIST_DIR}/include/stb_image_write.h"
	"${CMAKE_CURREND_LIST_DIR}/src/stb_impl.c")
	
target_compile_definitions(stb PRIVATE _CRT_SECURE_NO_WARNING)
target_include_directories(stb PULBIC "${CMAKE_CURREND_LIST_DIR}/include")
set_target_properties(stb PROPERTIES FOLDER Dependencies)	