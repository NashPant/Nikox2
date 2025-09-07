message("VCKPG ROOT is $ENV{VCKPG_ROOT}")

add_library(Nikopolis TPNikopolisRoot.cpp
 "tpproducts/curveinstruments/instruments.h" "tpproducts/curveinstruments/rate_futures.h")

add_subdirectory(tptools)
add_subdirectory(tpdatetime)
add_subdirectory(tpformulae)
add_subdirectory(tpmaths)


target_link_libraries(Nikopolis PUBLIC
	tptools
	tpformulae
	tpmaths
	tpdatetime
	fmt::fmt
)
target_include_directories(Nikopolis
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
)
#