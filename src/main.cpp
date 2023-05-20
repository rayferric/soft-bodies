#include <pch.hpp>

#include <core/world.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <os/os.hpp>

using namespace soft_bodies;

enum class spawn_mode { none, joint, rect, soft_box };

static constexpr float fixed_delta = 1.0F / 144.0F;

bool         paused                     = false;
float        time_counter               = 0.0F;
float        fixed_time_counter         = 0.0F;
float        frame_time                 = -1;
bool         spawning_joint             = false;
core::joint *selected_joint             = nullptr;
bool         left_mouse_button          = false;
bool         right_mouse_button         = false;
bool         left_mouse_button_clicked  = false;
bool         right_mouse_button_clicked = false;
std::chrono::time_point<std::chrono::steady_clock>
    left_mouse_button_click_time = std::chrono::steady_clock::now();
std::chrono::time_point<std::chrono::steady_clock>
    right_mouse_button_click_time = std::chrono::steady_clock::now();
std::unique_ptr<core::joint> spring_joint;
math::fvec2                  spring_joint_last_pos;
float                        new_joint_mass       = 0.01F;
float                        new_joint_radius     = 0.02F;
float                        new_spring_stiffness = 300.0F;
float                        new_spring_damping   = 0.5F;
core::rect                  *selected_rect        = nullptr;
core::rect                  *spawning_rect        = nullptr;
core::rect                  *rotating_rect        = nullptr;
math::fvec2                  rotating_rect_init_selection_pos;
float                        rotating_rect_init_angle;

void reset_world(core::world &world) {
	selected_joint = nullptr;
	spring_joint.reset();
	selected_rect = nullptr;
	spawning_rect = nullptr;
	rotating_rect = nullptr;

	world.joints.clear();
	world.springs.clear();
	world.rects.clear();

	// world.spawn_soft_box(math::fvec2(0, 0), 0.1, math::ivec2(2, 2), 0.01,
	// 0.02, 1000.0, 2);

	world.spawn_rect(math::fvec2(0, -1.1), math::fvec2(1.1, 0.1), 0);
	world.spawn_rect(math::fvec2(0, 1.1), math::fvec2(1.1, 0.1), 0);
	world.spawn_rect(math::fvec2(-1.1, 0), math::fvec2(0.1, 1.1), 0);
	world.spawn_rect(math::fvec2(1.1, 0), math::fvec2(0.1, 1.1), 0);
}

void draw_imgui(core::world &world) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	ImGui::NewFrame();

	{
		ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
		ImGui::SetNextWindowBgAlpha(0.5F);
		// create a window and append into it
		ImGui::Begin("Toolbox", NULL, ImGuiWindowFlags_NoResize);

		if (ImGui::Button(paused ? "Resume" : "Pause")) {
			paused = !paused;
		}

		ImGui::SameLine();
		ImGui::Text("Time: %.2f", time_counter);
		ImGui::SameLine();
		ImGui::Text("FPS: %d", static_cast<int>(1.0F / frame_time));

		ImGui::SameLine();

		if (ImGui::Button("Reset")) {
			reset_world(world);
		}

		ImGui::Text("Gravity: ");
		ImGui::SliderFloat("##gravity", &world.gravity, 0, 20.0F);

		ImGui::Separator();

		ImGui::Text("LMB over air: spawn joint");
		ImGui::Text("LMB over joint: spawn spring");
		ImGui::Text("LMB while spawning spring: attach spring or abort");

		ImGui::Dummy(ImVec2(0, 10));

		ImGui::Text("RMB over air: spawn rectangle");
		ImGui::Text("RMB while spawning rectangle: apply size or angle");

		ImGui::Dummy(ImVec2(0, 10));

		ImGui::Text("RMB over joint: delete joint");
		ImGui::Text("RMB over rectangle: delete rectangle");

		ImGui::Separator();

		ImGui::Text("New Joint Mass: ");
		ImGui::SliderFloat("##new_joint_mass", &new_joint_mass, 0.01F, 1.0F);

		ImGui::Text("New Joint Radius: ");
		ImGui::SliderFloat(
		    "##new_joint_radius", &new_joint_radius, 0.01F, 0.2F
		);

		ImGui::Text("New Spring Stiffness: ");
		ImGui::SliderFloat(
		    "##new_spring_stiffness", &new_spring_stiffness, 0.0F, 1000.0F
		);

		ImGui::Text("New Spring Damping: ");
		ImGui::SliderFloat(
		    "##new_spring_damping", &new_spring_damping, 0.0F, 20.0F
		);

		ImGui::Separator();

		if (ImGui::Button("Spawn Soft Box")) {
			world.spawn_soft_box(
			    math::fvec2(0, 0), 0.1, math::ivec2(2, 2), 0.01, 0.02, 1000.0, 2
			);
		}

		if (ImGui::Button("Spawn Wheel")) {
			world.spawn_soft_wheel(math::fvec2(0, 0), 0.1, 16);
		}

		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main() {
	glfwInit();

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);

	GLFWwindow *window =
	    glfwCreateWindow(900, 900, "Soft Bodies", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	gladLoadGL(glfwGetProcAddress);

	glfwSetFramebufferSizeCallback(
	    window,
	    [](GLFWwindow *window, int width, int height) {
		    glViewport(0, 0, width, height);
	    }
	);

	glfwSetMouseButtonCallback(
	    window,
	    [](GLFWwindow *window, int button, int action, int mods) {
		    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
				    return;
			    }

			    left_mouse_button            = true;
			    left_mouse_button_click_time = std::chrono::steady_clock::now();
		    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
			    left_mouse_button = false;
			    if (std::chrono::duration_cast<std::chrono::milliseconds>(
			            std::chrono::steady_clock::now() -
			            left_mouse_button_click_time
			        )
			            .count() < 200) {
				    left_mouse_button_clicked = true;
			    }
		    } else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
			    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
				    return;
			    }

			    right_mouse_button = true;
			    right_mouse_button_click_time =
			        std::chrono::steady_clock::now();
		    } else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
			    right_mouse_button = false;
			    if (std::chrono::duration_cast<std::chrono::milliseconds>(
			            std::chrono::steady_clock::now() -
			            right_mouse_button_click_time
			        )
			            .count() < 200) {
				    right_mouse_button_clicked = true;
			    }
		    }
	    }
	);

	// BEGIN IMGUI INIT

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// setup platform/renderer bindings
	if (!ImGui_ImplGlfw_InitForOpenGL(window, true)) {
		return 1;
	}
	if (!ImGui_ImplOpenGL3_Init()) {
		return 1;
	}

	/// END IMGUI INIT

	core::world world;
	world.gravity = 5.0;

	reset_world(world);
	world.spawn_soft_box(
	    math::fvec2(0, 0), 0.1, math::ivec2(2, 2), 0.01, 0.02, 1000.0, 2
	);

	std::chrono::time_point<std::chrono::steady_clock> last_time =
	    std::chrono::steady_clock::now();

	while (!glfwWindowShouldClose(window)) {
		// Reset clicked state before possibly setting it.
		left_mouse_button_clicked  = false;
		right_mouse_button_clicked = false;
		glfwPollEvents();

		// Find mouse position in world space.
		math::fvec2 world_mouse_pos;
		{
			math::dvec2 mouse_pos;
			glfwGetCursorPos(window, &mouse_pos.x, &mouse_pos.y);
			math::ivec2 window_size;
			glfwGetWindowSize(window, &window_size.x, &window_size.y);
			world_mouse_pos =
			    math::fvec2(mouse_pos) / math::fvec2(window_size) * 2 -
			    math::fvec2(1);
			world_mouse_pos.y *= -1;
		}

		float delta = std::chrono::duration<float>(
		                  std::chrono::steady_clock::now() - last_time
		)
		                  .count();
		last_time = std::chrono::steady_clock::now();

		if (frame_time < 0) {
			frame_time = delta;
		} else {
			frame_time = math::lerp(frame_time, delta, 0.1F);
		}

		if (!paused) {
			time_counter += delta;
			while (time_counter > fixed_time_counter) {
				world.update(fixed_delta);
				fixed_time_counter += fixed_delta;
			}
		}

		{
			// Find the closest joint to the mouse.
			float closest_joint_distance = 1000000;
			for (auto &joint_ptr : world.joints) {
				core::joint &joint = *joint_ptr;

				float distance =
				    math::distance(world_mouse_pos, joint.position);
				if (distance < closest_joint_distance) {
					selected_joint         = &joint;
					closest_joint_distance = distance;
				}
			}

			if (selected_joint &&
			    closest_joint_distance > selected_joint->radius) {
				selected_joint = nullptr;
			}

			// Find rect under the mouse.
			selected_rect = nullptr;
			for (auto &rect : world.rects) {
				if (rect.contains(world_mouse_pos) && &rect != spawning_rect &&
				    &rect != rotating_rect) {
					selected_rect = &rect;
					break;
				}
			}
		}

		{
			// Create a spring and an invisible joint if another joint was
			// clicked.
			if (selected_joint && !rotating_rect && left_mouse_button_clicked &&
			    !spring_joint && !spawning_rect) {
				spring_joint           = std::make_unique<core::joint>();
				spring_joint->position = selected_joint->position;
				spring_joint->mass     = 0;
				spring_joint->radius   = 0;
				spring_joint_last_pos  = spring_joint->position;

				auto spring = std::make_unique<core::spring>(
				    selected_joint,
				    spring_joint.get(),
				    0,
				    new_spring_stiffness,
				    new_spring_damping
				);
				world.springs.push_back(std::move(spring));
				spring_joint->springs.push_back(world.springs.back().get());
				selected_joint->springs.push_back(world.springs.back().get());
			}

            // Destroy the spring and the invisible joint if LMB was clicked without a different joint under the mouse.
            else if (left_mouse_button_clicked && !rotating_rect && !spawning_rect && spring_joint && (!selected_joint || selected_joint == spring_joint->springs.front()->start)) {
				auto *spring = spring_joint->springs.front();
				spring->start->springs.erase(std::find(
				    spring->start->springs.begin(),
				    spring->start->springs.end(),
				    spring
				));
				world.springs.erase(std::find_if(
				    world.springs.begin(),
				    world.springs.end(),
				    [&](std::unique_ptr<core::spring> &s) {
					    return s.get() == spring;
				    }
				));
				spring_joint.reset();
			}

            // Connect the spring to another joint.
            else if (left_mouse_button_clicked && !rotating_rect && spring_joint && !spawning_rect && selected_joint && selected_joint != spring_joint->springs.front()->start) {
				auto &spring = *spring_joint->springs.front();

				// Abort if the joint at the start of the created spring already
				// has a spring connected to the closest joint.
				if (std::find_if(
				        spring.start->springs.begin(),
				        spring.start->springs.end(),
				        [&](core::spring *s) {
					        return s->start == selected_joint ||
					               s->end == selected_joint;
				        }
				    ) == spring.start->springs.end()) {
					spring.end         = selected_joint;
					spring.rest_length = math::distance(
					    spring.start->position, spring.end->position
					);
					selected_joint->springs.push_back(&spring);
					spring_joint.reset();
				} else {
					// Destroy
					auto *spring = spring_joint->springs.front();
					spring->start->springs.erase(std::find(
					    spring->start->springs.begin(),
					    spring->start->springs.end(),
					    spring
					));
					world.springs.erase(std::find_if(
					    world.springs.begin(),
					    world.springs.end(),
					    [&](std::unique_ptr<core::spring> &s) {
						    return s.get() == spring;
					    }
					));
					spring_joint.reset();
				}
			}

			// Remove joint if clicked with RMB.
			else if (right_mouse_button_clicked && !rotating_rect && selected_joint && !spring_joint && !spawning_rect) {
				auto it = std::find_if(
				    world.joints.begin(),
				    world.joints.end(),
				    [&](std::unique_ptr<core::joint> &j) {
					    return j.get() == selected_joint;
				    }
				);
				if (it != world.joints.end()) {
					// Erase all springs connected to the joint.
					for (auto *spring : selected_joint->springs) {
						auto other_joint = spring->start == selected_joint
						                     ? spring->end
						                     : spring->start;

						// Remove spring from the other joint.
						other_joint->springs.erase(std::find_if(
						    other_joint->springs.begin(),
						    other_joint->springs.end(),
						    [&](core::spring *s) {
							    return s == spring;
						    }
						));

						// Remove spring from the world.
						world.springs.erase(std::find_if(
						    world.springs.begin(),
						    world.springs.end(),
						    [&](std::unique_ptr<core::spring> &s) {
							    return s.get() == spring;
						    }
						));
					}

					// Erase joint from the world.
					world.joints.erase(it);
				}
			}

			// Spawn a disconnected joint.
			else if (left_mouse_button_clicked && !spring_joint && !selected_joint && !selected_rect && !spawning_rect && !rotating_rect) {
				auto joint      = std::make_unique<core::joint>();
				joint->position = world_mouse_pos;
				joint->mass     = new_joint_mass;
				joint->radius   = new_joint_radius;
				world.joints.push_back(std::move(joint));
			}

			// Remove a rect.
			else if (right_mouse_button_clicked && selected_rect && !spring_joint && !spawning_rect && !rotating_rect) {
				world.rects.erase(std::find_if(
				    world.rects.begin(),
				    world.rects.end(),
				    [](core::rect &r) {
					    return &r == selected_rect;
				    }
				));
			}

			// Start spawning a rect.
			else if (right_mouse_button_clicked && !spring_joint && !selected_joint && !selected_rect && !spawning_rect && !rotating_rect) {
				world.rects.emplace_back();
				spawning_rect           = &world.rects.back();
				spawning_rect->position = world_mouse_pos;
				spawning_rect->radius   = math::fvec2(0);
			}

			// Finish spawning a rect and start rotating it.
			else if (right_mouse_button_clicked && !spring_joint && !rotating_rect && (spawning_rect || selected_rect)) {
				rotating_rect = spawning_rect ? spawning_rect : selected_rect;
				spawning_rect = nullptr;
				rotating_rect_init_selection_pos = world_mouse_pos;
				rotating_rect_init_angle         = rotating_rect->angle;
			}

			// Finish rotating a rect.
			else if (right_mouse_button_clicked && rotating_rect) {
				rotating_rect = nullptr;
			}
		}

		{
			// Move the spring joint to the mouse.
			if (spring_joint) {
				spring_joint->position = world_mouse_pos;
				spring_joint->velocity =
				    (spring_joint->position - spring_joint_last_pos) / delta;
				spring_joint_last_pos = spring_joint->position;
			}

			// Resize the spawning rect.
			if (spawning_rect) {
				spawning_rect->radius =
				    math::abs(world_mouse_pos - spawning_rect->position);
			}

			// Rotate the rotating rect.
			if (rotating_rect) {
				math::fvec2 center_to_mouse =
				    math::normalize(world_mouse_pos - rotating_rect->position);
				math::fvec2 center_to_old_mouse = math::normalize(
				    rotating_rect_init_selection_pos - rotating_rect->position
				);
				float angle_delta =
				    math::atan2(center_to_mouse.y, center_to_mouse.x) -
				    math::atan2(center_to_old_mouse.y, center_to_old_mouse.x);
				rotating_rect->angle = rotating_rect_init_angle + angle_delta;
				rotating_rect->angle =
				    math::mod(rotating_rect->angle, math::pi * 2);
			}
		}

		{
			// Color joints.
			for (auto &joint_ptr : world.joints) {
				core::joint &joint = *joint_ptr;

				if (spring_joint &&
				    spring_joint->springs.front()->start == &joint) {
					joint.color = math::fvec3(0.2, 0.9, 0.2);
				} else if (&joint == selected_joint) {
					joint.color = math::fvec3(0.9, 0.2, 0.2);
				} else {
					joint.color = math::fvec3(0.0);
				}
			}

			// Color rects
			for (auto &rect : world.rects) {
				if (&rect == selected_rect) {
					rect.color = math::fvec3(0.9, 0.2, 0.2);
				} else {
					rect.color = math::fvec3(0.0);
				}
			}
		}

		// Drawing

		glClearColor(0.2f, 0.3f, 0.3f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		world.draw(window);

		draw_imgui(world);

		glfwSwapBuffers(window);
	}

	return 0;
}
