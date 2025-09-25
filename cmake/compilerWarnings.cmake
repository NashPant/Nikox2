function(set_target_warnings target)
    set(MSVC_WARNINGS
        /W4
        /w14242
        /w14287
        /w14296
        /w14311
        /w14826
        /w44062
        /w44242
        /w14905
        /w14906
        /w14263
        /w44265
        /w14928
        /w4804
        /w5033
    )
    
    set(CLANG_WARNINGS
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wshadow
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wcast-align
        -Wunused
        -Woverloaded-virtual
        -Wformat=2
    )
    
    set(GCC_WARNINGS
        ${CLANG_WARNINGS}
        -Wmisleading-indentation
        -Wduplicated-cond
        -Wlogical-op
    )
    
    if(MSVC)
        target_compile_options(${target} PRIVATE ${MSVC_WARNINGS})
    elseif(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
        target_compile_options(${target} PRIVATE ${CLANG_WARNINGS})
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(${target} PRIVATE ${GCC_WARNINGS})
    endif()
endfunction()