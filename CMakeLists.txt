cmake_minimum_required(VERSION 3.13)
project(speed_charm_event C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)


add_executable(
	"speed_charm_event"
	"app_1.c"
	"sce_lexer.c"
	"sce_parser.c"
	"sce_ir.c"
	"sce_run.c"
	"sce_dynamic_link.c"
	"sce_cmp_operator.c"
	"sce_array_list.c"
	"sce_run_vm.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/std_ext/ext_alloc.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/std_ext/ext_strs.c"
)

add_library("sce_math" SHARED 
	"sce_math.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/std_ext/ext_alloc.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/std_ext/ext_strs.c"
)

target_include_directories(sce_math
	PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/std_ext"
)

target_include_directories(speed_charm_event
	PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/std_ext"
)
