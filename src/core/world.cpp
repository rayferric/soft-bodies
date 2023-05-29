#include <core/world.hpp>

static std::string vert = R"(
#version 330 core

layout (location = 0) in vec2 position;

uniform vec2 translation;
uniform mat2 basis;

void main() {
    gl_Position = vec4(basis * position + translation, 0.0, 1.0);
}
)";

static std::string frag = R"(
#version 330 core

out vec4 frag_color;

uniform vec3 color;

void main() {
    frag_color = vec4(color, 1.0);
}
)";

namespace soft_bodies::core {

world::world() : common_shader(vert, frag) {}

void world::update(float delta) {
	// Update forces.
	for (auto &joint_ptr : joints) {
		core::joint &joint = *joint_ptr;

		joint.force = math::fvec2::zero;

		// Apply gravity.
		joint.force.y -= joint.mass * gravity;

		// Apply spring forces.
		for (auto *spring_ptr : joint.springs) {
			core::spring &spring = *spring_ptr;

			math::fvec2 to_spring = spring.center() - joint.position;
			float       force     = spring.force();

			joint.force += to_spring * force;
		}
	}

	// Update velocities and positions.
	for (auto &joint_ptr : joints) {
		core::joint &joint = *joint_ptr;

		joint.velocity += joint.force * (delta / joint.mass);
		if (math::length(joint.velocity) > 10.0F) {
			joint.velocity = math::normalize(joint.velocity) * 10.0F;
		}
		joint.position += joint.velocity * delta;
	}

	// Solve rect collisions.
	for (auto &joint_ptr : joints) {
		core::joint &joint = *joint_ptr;

		for (auto &rect : rects) {
			// Transform the joint position into the rectangle's local space.
			// ..by rotating the joint position around the rectangle's center.

			math::fmat2 rot = math::fmat2{
			    math::cos(-rect.angle),
			    -math::sin(-rect.angle),
			    math::sin(-rect.angle),
			    math::cos(-rect.angle)};
			joint.position =
			    rect.position +
			    math::inverse(rot) * (joint.position - rect.position);

			// If the joint is inside the collision rectangle...
			math::fvec2 min =
			    rect.position - rect.radius - math::fvec2(joint.radius);
			math::fvec2 max =
			    rect.position + rect.radius + math::fvec2(joint.radius);
			if (joint.position.x > min.x && joint.position.x < max.x &&
			    joint.position.y > min.y && joint.position.y < max.y) {
				// Find closest point on the surface of the rectangle and move
				// the joint there.

				float distances_to_edges[4]{
				    joint.position.x - min.x,
				    joint.position.y - min.y,
				    max.x - joint.position.x,
				    max.y - joint.position.y};

				float  min_distance = distances_to_edges[0];
				size_t min_index    = 0;

				for (size_t i = 1; i < 4; i++) {
					if (distances_to_edges[i] < min_distance) {
						min_distance = distances_to_edges[i];
						min_index    = i;
					}
				}

				math::fvec2 normal;

				switch (min_index) {
				case 0:
					joint.position.x = min.x;
					normal           = math::fvec2(-1.0, 0.0);
					break;
				case 1:
					joint.position.y = min.y;
					normal           = math::fvec2(0.0, -1.0);
					break;
				case 2:
					joint.position.x = max.x;
					normal           = math::fvec2(1.0, 0.0);
					break;
				case 3:
					joint.position.y = max.y;
					normal           = math::fvec2(0.0, 1.0);
					break;
				default:
					break;
				}

				// Transform the normal back into the world space.
				normal = rot * normal;

				// Reflect the velocity.
				joint.velocity = math::reflect(joint.velocity, normal) * 0.9F;
			}

			// Transform the joint back into the world space.
			joint.position =
			    rect.position + rot * (joint.position - rect.position);
		}
	}

	// Solve joint collisions.
	for (auto &joint_ptr : joints) {
		core::joint &joint = *joint_ptr;

		// If the joint collides with any other neighboring joints...
		// for (auto &spring_ptr : joint.springs) {
		//     core::joint &other = &spring_ptr->start == &joint ?
		//     spring_ptr->end : spring_ptr->start;
		for (auto &other_ptr : joints) {
			if (&joint == &*other_ptr) {
				continue;
			}
			core::joint &other = *other_ptr;

			// Here order of joint traversal might have some effect, but it's
			// probably not that important.

			float distance   = math::distance(joint.position, other.position);
			float radius_sum = joint.radius + other.radius;

			if (distance < radius_sum - math::epsilon) {
				float distance_delta = radius_sum - distance;

				// Move the two joints apart.
				math::fvec2 to_other = other.position - joint.position;
				to_other             = math::normalize(to_other);

				joint.position -= to_other * distance_delta * 0.5F;
				other.position += to_other * distance_delta * 0.5F;

				joint.velocity =
				    math::reflect(joint.velocity, -to_other) * 0.9F;
				other.velocity = math::reflect(other.velocity, to_other) * 0.9F;
			}
		}
	}
}

void world::draw(GLFWwindow *window) {
	common_shader.bind();

	// Draw collision rects.
	square.bind();

	for (auto &rect : rects) {
		math::fvec2 translation = rect.position;
		math::fmat2 basis = math::fmat2{rect.radius.x, 0.0, 0.0, rect.radius.y};
		basis =
		    math::fmat2{
		        math::cos(-rect.angle),
		        -math::sin(-rect.angle),
		        math::sin(-rect.angle),
		        math::cos(-rect.angle)} *
		    basis;
		gl::shader::set_uniform(
		    common_shader.get_uniform_location("translation"), translation
		);
		gl::shader::set_uniform(
		    common_shader.get_uniform_location("basis"), basis
		);
		gl::shader::set_uniform(
		    common_shader.get_uniform_location("color"), rect.color
		);

		glDrawElements(
		    GL_TRIANGLES, square.get_index_count(), GL_UNSIGNED_INT, nullptr
		);
	}

	// Draw springs.
	square.bind();
	gl::shader::set_uniform(
	    common_shader.get_uniform_location("color"), math::fvec3(0.9)
	);

	for (auto &spring_ptr : springs) {
		core::spring &spring = *spring_ptr;

		math::fvec2 translation = spring.center();
		math::fmat2 basis =
		    math::fmat2{spring.length() * 0.5F, 0.0F, 0.0F, 0.005F};
		math::fvec2 dir = spring.end->position -
		                  spring.start->position; // or start - end, whatever
		float angle = -math::atan2(dir.y, dir.x);
		basis =
		    math::fmat2{
		        math::cos(angle),
		        -math::sin(angle),
		        math::sin(angle),
		        math::cos(angle)} *
		    basis;

		gl::shader::set_uniform(
		    common_shader.get_uniform_location("translation"), translation
		);
		gl::shader::set_uniform(
		    common_shader.get_uniform_location("basis"), basis
		);

		glDrawElements(
		    GL_TRIANGLES, square.get_index_count(), GL_UNSIGNED_INT, nullptr
		);
	}

	// Draw joints.
	circle.bind();

	for (auto &joint_ptr : joints) {
		core::joint &joint = *joint_ptr;

		math::fvec2 translation = joint.position;
		// math::fmat2 basis = math::fmat2::identity * 0.012;
		math::fmat2 basis = math::fmat2::identity * joint.radius;
		gl::shader::set_uniform(
		    common_shader.get_uniform_location("translation"), translation
		);
		gl::shader::set_uniform(
		    common_shader.get_uniform_location("basis"), basis
		);
		gl::shader::set_uniform(
		    common_shader.get_uniform_location("color"), joint.color
		);

		glDrawElements(
		    GL_TRIANGLES, circle.get_index_count(), GL_UNSIGNED_INT, nullptr
		);
	}
}

void world::spawn_rect(math::fvec2 position, math::fvec2 radius, float angle) {
	core::rect rect;
	rect.position = position;
	rect.radius   = radius;
	rect.angle    = angle;
	this->rects.push_back(std::move(rect));
}

void world::spawn_soft_box(
    math::fvec2 position,
    float       grid_size,
    math::ivec2 radius,
    float       joint_mass,
    float       joint_radius,
    float       spring_stiffness,
    float       spring_damping
) {
	// Spawn joints.
	std::vector<std::unique_ptr<core::joint>> joints;
	for (int32_t x = -radius.x; x <= radius.x; x++) {
		for (int32_t y = -radius.y; y <= radius.y; y++) {
			auto joint_ptr      = std::make_unique<core::joint>();
			joint_ptr->position = position + math::fvec2(x, y) * grid_size;
			joint_ptr->mass     = joint_mass;
			joint_ptr->radius   = joint_radius;
			joints.push_back(std::move(joint_ptr));
		}
	}

	// Interconnect joints. Pattern:
	// ---
	// |X|
	// ---
	std::vector<std::unique_ptr<core::spring>> springs;

	// horizontal
	for (int32_t x = -radius.x; x < radius.x; x++) {
		for (int32_t y = -radius.y; y <= radius.y; y++) {
			int32_t start_index =
			    (x + radius.x) * (radius.y * 2 + 1) + (y + radius.y);
			int32_t end_index =
			    (x + radius.x + 1) * (radius.y * 2 + 1) + (y + radius.y);

			core::joint &start = *joints[start_index];
			core::joint &end   = *joints[end_index];

			auto spring_ptr = std::make_unique<core::spring>(
			    &start, &end, grid_size, spring_stiffness, spring_damping
			);

			start.springs.push_back(spring_ptr.get());
			end.springs.push_back(spring_ptr.get());

			springs.push_back(std::move(spring_ptr));
		}
	}

	// vertical
	for (int32_t x = -radius.x; x <= radius.x; x++) {
		for (int32_t y = -radius.y; y < radius.y; y++) {
			int32_t start_index =
			    (x + radius.x) * (radius.y * 2 + 1) + (y + radius.y);
			int32_t end_index =
			    (x + radius.x) * (radius.y * 2 + 1) + (y + radius.y + 1);

			core::joint &start = *joints[start_index];
			core::joint &end   = *joints[end_index];

			auto spring_ptr = std::make_unique<core::spring>(
			    &start, &end, grid_size, spring_stiffness, spring_damping
			);

			start.springs.push_back(spring_ptr.get());
			end.springs.push_back(spring_ptr.get());

			springs.push_back(std::move(spring_ptr));
		}
	}

	// lower-right diagonal
	for (int32_t x = -radius.x; x < radius.x; x++) {
		for (int32_t y = -radius.y; y < radius.y; y++) {
			int32_t start_index =
			    (x + radius.x) * (radius.y * 2 + 1) + (y + radius.y);
			int32_t end_index =
			    (x + radius.x + 1) * (radius.y * 2 + 1) + (y + radius.y + 1);

			core::joint &start = *joints[start_index];
			core::joint &end   = *joints[end_index];

			auto spring_ptr = std::make_unique<core::spring>(
			    &start,
			    &end,
			    grid_size * math::sqrt2,
			    spring_stiffness,
			    spring_damping
			);

			start.springs.push_back(spring_ptr.get());
			end.springs.push_back(spring_ptr.get());

			springs.push_back(std::move(spring_ptr));
		}
	}

	// lower-left diagonal
	for (int32_t x = -radius.x; x < radius.x; x++) {
		for (int32_t y = -radius.y; y < radius.y; y++) {
			int32_t start_index =
			    (x + radius.x) * (radius.y * 2 + 1) + (y + radius.y + 1);
			int32_t end_index =
			    (x + radius.x + 1) * (radius.y * 2 + 1) + (y + radius.y);

			core::joint &start = *joints[start_index];
			core::joint &end   = *joints[end_index];

			auto spring_ptr = std::make_unique<core::spring>(
			    &start,
			    &end,
			    grid_size * math::sqrt2,
			    spring_stiffness,
			    spring_damping
			);

			start.springs.push_back(spring_ptr.get());
			end.springs.push_back(spring_ptr.get());

			springs.push_back(std::move(spring_ptr));
		}
	}

	// Add soft body to the world.
	this->joints.insert(
	    this->joints.end(),
	    std::make_move_iterator(joints.begin()),
	    std::make_move_iterator(joints.end())
	);
	this->springs.insert(
	    this->springs.end(),
	    std::make_move_iterator(springs.begin()),
	    std::make_move_iterator(springs.end())
	);
}

void world::spawn_soft_wheel(
    math::fvec2 position, float radius, size_t vertices
) {
	// Spawn joints.
	std::vector<std::unique_ptr<core::joint>> joints;

	auto root      = std::make_unique<core::joint>();
	root->position = position;
	root->mass     = 0.01;
	root->radius   = 0.02;
	joints.push_back(std::move(root));

	for (size_t i = 0; i < vertices; i++) {
		auto joint_ptr = std::make_unique<core::joint>();
		joint_ptr->position =
		    position + math::fvec2(
		                   math::cos(i * 2 * math::pi / vertices),
		                   math::sin(i * 2 * math::pi / vertices)
		               ) * radius;
		joint_ptr->mass   = 0.01;
		joint_ptr->radius = 0;
		joints.push_back(std::move(joint_ptr));
	}

	// Spawn springs between all joints.
	std::vector<std::unique_ptr<core::spring>> springs;

	for (size_t i = 0; i < joints.size(); i++) {
		for (size_t j = i + 1; j < joints.size(); j++) {
			auto spring_ptr = std::make_unique<core::spring>(
			    joints[i].get(),
			    joints[j].get(),
			    math::distance(joints[i]->position, joints[j]->position),
			    300,
			    1
			);
			joints[i]->springs.push_back(spring_ptr.get());
			joints[j]->springs.push_back(spring_ptr.get());
			springs.push_back(std::move(spring_ptr));
		}
	}

	// Add objects to the world.
	this->joints.insert(
	    this->joints.end(),
	    std::make_move_iterator(joints.begin()),
	    std::make_move_iterator(joints.end())
	);
	this->springs.insert(
	    this->springs.end(),
	    std::make_move_iterator(springs.begin()),
	    std::make_move_iterator(springs.end())
	);
}

} // namespace soft_bodies::core
