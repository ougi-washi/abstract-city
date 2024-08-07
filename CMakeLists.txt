# STONKENGINE - Jed Fakhfekh - https://github.com/ougi-washi

## General
cmake_minimum_required(VERSION 3.22.1)

project(STONKENGINE LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_GENERATOR)
    find_program(NINJA_EXECUTABLE ninja)
    if(NINJA_EXECUTABLE)
        set(CMAKE_GENERATOR "Ninja" CACHE INTERNAL "Ninja")
        message(STATUS "Ninja build system found. Using Ninja generator.")
    else()
        message(STATUS "Ninja build system not found. Using default generator.")
    endif()
endif()

# Directory setup
set(LIB_DIR "lib")
set(INC_DIR "include")
set(SRC_DIR "src")
set(BIN_DIR "bin")

# setup library macro
macro(setup_library arg_lib_name arg_source_path arg_include_path)
	message(STATUS "Adding ${arg_lib_name}")
	file(GLOB CURRENT_SRC_FILES	"${arg_source_path}/*.c" 
								"${arg_source_path}/*.cpp"
								"${arg_source_path}/**/*.c" 
								"${arg_source_path}/**/*.cpp")
	add_library(${arg_lib_name} STATIC	${CURRENT_SRC_FILES})
	target_include_directories(${arg_lib_name} PUBLIC ${arg_include_path})
endmacro()

find_package(Threads REQUIRED)

# RAYLIB
message(STATUS "Adding Raylib library")
add_subdirectory(${LIB_DIR}/raylib)
message(STATUS "Adding Raygui library")
add_subdirectory(${LIB_DIR}/raygui/projects/CMake)

# Main module
set(MAIN_MODULE_INCLUDES ${PROJECT_SOURCE_DIR}/${INC_DIR} ${PROJECT_SOURCE_DIR}/${LIB_DIR})
setup_library(main_module ${SRC_DIR} "${MAIN_MODULE_INCLUDES}")
target_link_libraries(main_module PUBLIC raylib raygui)

# Executable
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC main_module)
set_target_properties(${PROJECT_NAME} PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/${BIN_DIR})

## Definitions
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()
