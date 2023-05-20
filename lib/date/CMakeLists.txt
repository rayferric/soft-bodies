cmake_minimum_required(VERSION 3.20)
project(Date LANGUAGES CXX)

add_library(Date INTERFACE)
add_library(Date::Date ALIAS Date)

target_include_directories(Date INTERFACE
	"${CMAKE_CURRENT_SOURCE_DIR}/include/"
)

set_target_properties(Date PROPERTIES
    CXX_STANDARD 11
)
