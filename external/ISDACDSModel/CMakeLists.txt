
cmake_minimum_required (VERSION 3.26)

project(ISDACDSModel)

set(CMAKE_CXX_STANDARD 20)
#include (${CMAKE_SOURCE_DIR}/../infra/files/common.cmake) 
#setup()

message (STATUS "ISDACDSModel CURRENT SOURCE DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
if (MSVC)
	set (CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

file (GLOB C_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/isda")
set_source_files_properties(${C_SRC_FILES} PROPERTIES LANGUAGE CXX)

add_library(ISDACDSModel SHARED ${C_SRC_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/src/HolidayWrapper.cpp)

set_target_properties(ISDACDSModel PROPERTIES POSITION_INDEPENDENT_CODE ON LINKER_LANGUAGE CXX)