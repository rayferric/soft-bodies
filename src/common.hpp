#pragma once

#ifdef _WIN32
	#define SOFT_BODIES_PLATFORM_WINDOWS
#elif __linux__
	#define SOFT_BODIES_PLATFORM_LINUX
#else
	#error "Unsupported OS."
#endif

#ifdef SOFT_BODIES_PLATFORM_LINUX
	#define GLFW_EXPOSE_NATIVE_X11
	#define GLFW_EXPOSE_NATIVE_WAYLAND
#elif defined(SOFT_BODIES_PLATFORM_WINDOWS)
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX

	#define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(SOFT_BODIES_PLATFORM_MACOS)
	#define GLFW_EXPOSE_NATIVE_COCOA
#endif

#define GLFW_INCLUDE_NONE

#include <pch.hpp>
