
cmake_minimum_required(VERSION 3.15)
# Project metadata
project (TPXLInterface LANGUAGES CXX)

# Force C++ 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Configurations
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

set(PARENT_DIRECTORY ${CMAKE_SOURCE_DIR}/..)
message(STATUS "XLNIKO INTERFACE Current Source Directory: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "XLNIKO PARENT Directory: ${PARENT_DIRECTORY}")


# Optional: Force 64-bit platform for Visual Studio
if (MSVC)
	set (CMAKE_GENERATOR_PLATFORM X64 CACHE STRING "Platform" FORCE)
endif()


add_library(TPXLNIKO SHARED
TPXLNikoMain.cpp
API/TPXLInfo.cpp
TPXLNiko.def)

# Force Linker to use .def file (MSVC)
if (MSVC)
	target_link_options (TPXLNIKO PRIVATE "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/TPXLNiko.def") # Optionally disable LTCG
	set_target_properties (TPXLNIKO PROPERTIES LINK_FLAGS "/DEBUG /INCREMENTAL")
endif()

set_target_properties (TPXLNIKO PROPERTIES SUFFIX ".xll")

target_compile_definitions (TPXLNIKO
	PRIVATE TPXLNIKO_EXPORTS
)

target_link_libraries (TPXLNIKO PUBLIC
TPXLInterface
tptools)

target_include_directories(TPXLNIKO PRIVATE ${CMAKE_SOURCE_DIR}/src/ ${CMAKE_SOURCE_DIR}/interface/ )