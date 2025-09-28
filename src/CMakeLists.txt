message("VCKPG ROOT is $ENV{VCKPG_ROOT}")

add_library(Nikopolis TPNikopolisRoot.cpp
 "tpproducts/curveinstruments/instruments.h" "tpproducts/curveinstruments/rate_futures.h" "tpmarketdata/rates_curve_data.h" "tpmarketdata/marketcurves/discount_factors.h" "tpmodels/curves/discount_curve.h" "tpmodels/curves/curve_utils.h" "tpmodels/curves/discount_curve.cpp")

add_subdirectory(tptools)
add_subdirectory(tpdatetime)
add_subdirectory(tpformulae)
add_subdirectory(tpmaths)
add_subdirectory(tpmarketdata)
add_subdirectory(tpproducts)


target_link_libraries(Nikopolis PUBLIC
	tptools
	tpformulae
	tpmaths
	tpdatetime
	tpmarketdata
	fmt::fmt
)
target_include_directories(Nikopolis
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
)
#