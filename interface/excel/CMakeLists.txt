
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)


# Configurations
set (CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

set(PARENT_DIRECTORY ${CMAKE_SOURCE_DIR}/..)
message (STATUS "EXCEL INTERFACE Current Source Directory: ${CMAKE_CURRENT_SOURCE_DIR}") 
message(STATUS "EXCEL PARENT Directory: ${PARENT_DIRECTORY}")

# Optional: Force 64-bit platform for Visual Studio
if (MSVC)
	set (CMAKE_GENERATOR_PLATFORM X64 CACHE STRING "Platform" FORCE)
endif()


add_library(TPXLInterface SHARED
NewInterface/moduleInfo.cpp
TPExcelInterface.cpp
TPExcelRegistrationFunctions.cpp
TPExcelArgumentArray.cpp
XLOper.cpp
framework.cpp
)


# Determine architecture
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
	# 64-bit build
	set (XLCALL32_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/x64")
	set(XLCALL32_LIB "${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/XLCALL32.lib")
else()
	# 32-bit build
	set(XLCALL32_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib32")
	set(XLCALL32_LIB "${CMAKE_CURRENT_SOURCE_DIR}/lib32/XLCALL32.lib")
endif()
message (STATUS "XLCALL32 Library directory: ${XLCALL32_DIR}") 
message (STATUS "Linking against: ${XLCALL32_LIB}")

target_compile_definitions (TPXLInterface
PRIVATE TPXLINTERFACE_EXPORTS
)


#target_link_libraries (TPXLInterface PUBLIC
#tptools
#tpmaths)

#target_include_directories (TPXLInterface PRIVATE ${CMAKE_SOURCE_DIR}/src/ )

target_link_libraries (TPXLInterface PUBLIC 
"${XLCALL32_LIB}"
tptools
tpmaths)

target_include_directories (TPXLInterface PRIVATE ${CMAKE_SOURCE_DIR}/src/ "${XLCALL32_DIR}" )