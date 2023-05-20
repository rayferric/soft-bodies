cmake_minimum_required(VERSION 3.20)
project(SoftBodies VERSION 1.0.0 LANGUAGES CXX)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/lib/date")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/lib/glad")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/lib/stb")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/lib/termcolor")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

file(GLOB_RECURSE TARGET_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.inl"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(${PROJECT_NAME} ${TARGET_SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    OUTPUT_NAME "soft-bodies"
)

target_include_directories(${PROJECT_NAME} PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(${PROJECT_NAME} Date::Date Glad::Glad GLFW::GLFW Stb::Stb ImGui::ImGui Termcolor::Termcolor)

target_precompile_headers(${PROJECT_NAME} PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/src/pch.hpp"
)
