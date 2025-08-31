
file(GLOB_RECURSE DATETIME_SOURCE_FILES
"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
 "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
 "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")


 # Find specific Boost components
# find_package(Boost REQUIRED COMPONENTS core system)

 add_library(tpdatetime STATIC ${DATETIME_SOURCE_FILES})

 target_link_libraries(tpdatetime PRIVATE 
 tptools
 fmt::fmt)
