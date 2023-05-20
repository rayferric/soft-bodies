cmake_minimum_required(VERSION 3.20)
project(Termcolor LANGUAGES CXX)

add_library(Termcolor INTERFACE)
add_library(Termcolor::Termcolor ALIAS Termcolor)

target_include_directories(Termcolor INTERFACE
	"${CMAKE_CURRENT_SOURCE_DIR}/include/"
)

set_target_properties(Termcolor PROPERTIES
    CXX_STANDARD 11
)
