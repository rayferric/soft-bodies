#include <core/joint.hpp>
#include <core/spring.hpp>

namespace soft_bodies::core {

spring::spring(
    joint *start, joint *end, float rest_length, float stiffness, float damping
)
    : start(start), end(end), rest_length(rest_length), stiffness(stiffness),
      damping(damping) {}

float spring::force() const {
	math::fvec2 dir          = end->position - start->position;
	float       length       = math::length(dir);
	float       length_delta = length - rest_length;

	float stiffness_force = length_delta * stiffness;

	dir                   /= length;
	math::fvec2 vel_delta = end->velocity - start->velocity;

	float damping_force = math::dot(vel_delta, dir) * damping;

	return stiffness_force + damping_force;
}

math::fvec2 spring::center() const {
	return (start->position + end->position) * 0.5F;
}

float spring::length() const {
	return math::distance(start->position, end->position);
}

} // namespace soft_bodies::core
