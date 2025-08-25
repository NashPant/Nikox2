message("VCKPG ROOT is $ENV{VCKPG_ROOT}")

add_library(Nikopolis TPNikopolisRoot.cpp
)

add_subdirectory(tptools)
add_subdirectory(tpformulae)
#