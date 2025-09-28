
set (GSL_ROOT $ENV{GSL_ROOT})
set (GSL_INCLUDE_DIR "${GSL_ROOT}")
set(GSL_LIBRARY "${GSL_ROOT}/gsl.lib")
set(GSL_CBLAS_LIBRARY "${GSL_ROOT}/gslcblas.lib")
message ("USing GSL Libs in directory: ${GSL_ROOT}")

if (NOT EXISTS "${GSL_INCLUDE_DIR}")
	message (FATAL_ERROR "GSL include directory not found: ${GSL_INCLUDE_DIR}")
endif()
if (NOT EXISTS "${GSL_LIBRARY}")
	message (FATAL_ERROR "GSL library not found: ${GSL_LIBRARY}")
endif()
if (NOT EXISTS "${GSL_CBLAS_LIBRARY}")
	message (FATAL_ERROR "GSL CBLAS library not found: ${GSL_CBLAS_LIBRARY}") 
endif()
include_directories(${GSL_INCLUDE_DIR})

set(EXTERNAL_DIRECTORY ${CMAKE_SOURCE_DIR}/external)

file(GLOB_RECURSE MARKET_SOURCE_FILES
"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
 "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
 "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")


 # Find specific Boost components
# find_package(Boost REQUIRED COMPONENTS core system)

 add_library(tpmarket STATIC ${MARKET_SOURCE_FILES})

  target_include_directories(tpmarket PUBLIC
    ${EXTERNAL_DIRECTORY}
)

 target_link_libraries(tpmarket PRIVATE 
 tptools
 tpdatetime
 fmt::fmt
 ${GSL_LIBRARY} 
${GSL_CBLAS_LIBRARY})
