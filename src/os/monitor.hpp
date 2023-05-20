#pragma once

#include <common.hpp>

#include <math/math.hpp>

namespace soft_bodies::os {

class monitor {
public:
	GLFWmonitor *_glfw_monitor;

	static std::shared_ptr<monitor> primary();

	static std::vector<std::shared_ptr<monitor>> enumerate();

	math::uvec2 resolution();

	math::uvec3 color_depth();

	uint32_t refresh_rate();

private:
	monitor(GLFWmonitor *handle);

	const GLFWvidmode *glfw_video_mode;
};

}
