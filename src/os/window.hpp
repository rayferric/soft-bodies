#pragma once

#include <common.hpp>

#include <math/math.hpp>
#include <os/monitor.hpp>

namespace soft_bodies::os {

class window {
public:
	GLFWwindow *_glfw_window;

	window(std::string &&title, math::uvec2 size);

	virtual ~window();

	void process();

	bool is_closing();

	void set_closing(bool closing);

	std::string get_title();

	void set_title(std::string title);

	math::uvec2 get_pos();

	void set_pos(math::uvec2 pos) const;

	math::uvec2 get_size();

	void set_size(math::uvec2 size) const;

	bool is_maximized() const;

	void set_maximized(bool maximized);

	bool is_minimized() const;

	void set_minimized(bool minimized);

	bool is_fullscreen() const;

	void set_fullscreen(bool fullscreen);

	std::shared_ptr<monitor> get_monitor();

	void set_monitor(std::shared_ptr<monitor> monitor);

	bool is_visible() const;

	void set_visible(bool visible);

	bool focused() const;

	void focus();

	math::uvec2 get_frame_size();

	bool has_surface() const;

	void _construct_swapchain();

	void _destruct_swapchain();

protected:
	std::string title;
	math::uvec2 windowed_pos, windowed_size;
	// Maximized and minimized trackers are required, because
	// GLFW gives wrong answer once the window goes fullscreen 
	bool maximized = false, minimized = false, fullscreen = false;
	std::shared_ptr<monitor> monitor = monitor::primary();
	math::uvec2 frame_size;
	bool surface_constructed = false;

private:
	static void key_callback(GLFWwindow *glfw_window, int32_t key, int32_t scancode, int32_t action, int32_t mods);

	static void char_callback(GLFWwindow *glfw_window, uint32_t codepoint);

	static void mouse_button_callback(GLFWwindow *glfw_window, int32_t button, int32_t action, int32_t mods);

	static void scroll_callback(GLFWwindow *glfw_window, double x, double y);

	static void pos_callback(GLFWwindow *glfw_window, int32_t x, int32_t y);

	static void size_callback(GLFWwindow *glfw_window, int32_t width, int32_t height);

	static void iconify_callback(GLFWwindow *glfw_window, int32_t iconified);

	static void maximize_callback(GLFWwindow *glfw_window, int32_t maximized);

	static void framebuffer_size_callback(GLFWwindow *glfw_window, int32_t width, int32_t height);

	void apply_mode();
};

}