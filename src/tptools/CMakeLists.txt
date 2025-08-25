set(PARENT_DIRECTORY ${CMAKE_SOURCE_DIR}/..)
message(STATUS "TOOLS Current Source Directory: ${CMAKE_CURRENT_SOURCE_DIR}")


#set(FRM_SRC_DIRECTORY "${CMAKE_SOURCE_DIR}/src/tpformulae")
#message(STATUS "Formulae Source Directory: ${FRM_SRC_DIRECTORY}")
file(GLOB_RECURSE TOOLS_SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
     "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
#file(GLOB TOOLS_CONT_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Containers/*.cpp")

message(STATUS "TOOLS_SOURCE_FILES = ${TOOLS_SOURCE_FILES}")
#add_library(tptools SHARED ${FRM_SOURCE_FILES} ${TOOLS_CONT_SOURCE_FILES})
add_library(tptools SHARED ${TOOLS_SOURCE_FILES})

# 1. Add include directory for tpformulae private headers
# Or using generator expressions
target_include_directories(tptools
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
)


set_target_properties(tptools PROPERTIES LINKER_LANGUAGE CXX)
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET tptools PROPERTY CXX_STANDARD 20)
endif()