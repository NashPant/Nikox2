set(PARENT_DIRECTORY ${CMAKE_SOURCE_DIR}/..)
message(STATUS "Formulae Current Source Directory: ${CMAKE_CURRENT_SOURCE_DIR}")


#set(FRM_SRC_DIRECTORY "${CMAKE_SOURCE_DIR}/src/tpformulae")
#message(STATUS "Formulae Source Directory: ${FRM_SRC_DIRECTORY}")
file(GLOB FRM_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# add_library(tpformulae SHARED ${FRM_SOURCE_FILES})

if(DEFINED TPSTATIC OR BUILD_TPTOOLS_STATIC)
    message(STATUC "tpformulae compiling Static Lib")
    add_library(tpformulae STATIC ${FRM_SOURCE_FILES})
else()
     message(STATUC "tpformulae compiling Shared Lib")
    add_library(tpformulae SHARED ${FRM_SOURCE_FILES})
endif()

# 1. Add include directory for tpformulae private headers
# Or using generator expressions
target_include_directories(tpformulae
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
)

target_link_libraries(tpformulae PUBLIC
tptools)

set_target_properties(tpformulae PROPERTIES LINKER_LANGUAGE CXX)
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET tpformulae PROPERTY CXX_STANDARD 20)
endif()