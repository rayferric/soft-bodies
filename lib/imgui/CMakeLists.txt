cmake_minimum_required(VERSION 3.20)
project(ImGui VERSION 1.0.0 LANGUAGES CXX)

# Discover sources.
file(GLOB_RECURSE TARGET_SOURCES CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

# Declare the library.
add_library(${PROJECT_NAME} STATIC ${TARGET_SOURCES})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# Set some target options.
set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 11
	OUTPUT_NAME "imgui"
)

# Publish the headers.
target_include_directories(${PROJECT_NAME} PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(${PROJECT_NAME} PRIVATE GLFW::GLFW)
