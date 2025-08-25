cmake_minimum_required(VERSION 3.26)
macro(setup)
	# Language standards
	set (CMAKE_CXX_STANDARD 20) set(CMAKE_CXX_STANDARD_REQUIRED YES) set(CMAKE_CXX_EXTENSIONS OFF)
	set (CMAKE_C_STANDARD 11)
	set (CMAKE_C_STANDARD_REQUIRED YES)
	set(CMAKE_C_EXTENSIONS OFF)

	option (CMAKE_COMPILE_WARNING_AS_ERROR "Warnings -> errors" ON)

	message("MSVC is: ${MSVC}")
	message("CMAKE_CXX_COMPILER_ID is: ${CMAKE_CXX_COMPILER_ID}") 

	if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		add_compile_options(/W4 /wd4018 /wd4389)
	else()
		add_compile_options (-Wall -Wextra -Wno-sign-compare)
	endif()

	# Warnings to max + turn off signed/unsigned warnings
	# Warnings are errors by default
	set (CMAKE_COMPILE_WARNING_AS_ERROR ON)
	# Windows has excellent stl debugging by default; turn on assertions for stdlibc++ if (NOT WIN32)
	add_compile_definitions($<$<CONFIG: Debug>:_GLIBCXX_ASSERTIONS>)
	endif()
	# Standard build tweaks
	if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC") 
		add_compile_options(/bigobj /Zc:__cplusplus) 
		add_link_options(/MANIFEST: NO)
	endif()

	set (CMAKE_POSITION_INDEPENDENT_CODE ON)

	if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	endif()
	# On Linux, use $ORIGIN to mean "the directory of the executable"
	if(UNIX AND NOT APPLE)
		set (CMAKE_INSTALL_RPATH "$ORIGIN")
	# Optionally, if you want your build tree to use the install RPATH as well:
	# set (CMAKE_BUILD_WITH_INSTALL_RPATH ON)
	endif()

	# On macOS, you might use @loader_path instead:
	if (APPLE)
		set (CMAKE_INSTALL_RPATH "@loader_path")
		# set (CMAKE_BUILD_WITH_INSTALL_RPATH ON) endif()
endmacro()

function(setup_sanitizer)
	# Sanitizers (gcc only)
	option (MY_SANITIZE "Enable address, leak, undefined sanitizers (gcc only)")
	if(MY_SANITIZE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU")
		set (SANITIZE_OPTIONS -fno-omit-frame-pointer -fsanitize=address -fsanitize=leak -fsanitize=undefined)
		add_compile_options(${SANITIZE_OPTIONS})
		add_link_options(${SANITIZE_OPTIONS})
		add_compile_options(-Wno-array-bounds -Wno-stringop-overread) # gcc13
	endif()
endfunction()

function (setup_code_coverage)
	option (MY_CODE_COVERAGE "Code coverage")
	if (MY_CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
		add_compile_options(--coverage -fprofile-abs-path)
		add_link_options(--coverage)
	endif()
endfunction()
