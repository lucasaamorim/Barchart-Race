cmake_minimum_required(VERSION 3.5)
project (BarCharRace VERSION 1.0.0 LANGUAGES CXX )

# Currently CMake (since 2.8.5) supports generation of compilation databases
# for Unix Makefile builds (Ninja builds in the works) with the option
# CMAKE_EXPORT_COMPILE_COMMANDS.
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)
#=== FINDING PACKAGES ===#


#=== SETTING VARIABLES ===#
# Compiling flags
set( GCC_COMPILE_FLAGS "-Wall" )
set( CMAKE_CXX_FLAGS_DEBUG "-Og -g")
set( CMAKE_CXX_FLAGS_RELEASE "-O3" )

#=== Main App ===

# include_directories( "core" "libs" )

add_executable( bcr "main.cpp"
                    "barchart.cpp"
                    "animation.cpp"
                    "file_parser.cpp"
                    "libs/coms.cpp")

target_compile_features( bcr PUBLIC cxx_std_17 )
