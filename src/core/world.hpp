#pragma once

#include <memory>
#include <vector>

#include <core/joint.hpp>
#include <core/spring.hpp>
#include <core/rect.hpp>
#include <gl/gl.hpp>

namespace soft_bodies::core {

class world {
public:
    std::vector<std::unique_ptr<joint>> joints;
    std::vector<std::unique_ptr<spring>> springs;
    std::vector<rect> rects;
    float gravity = 9.81F; // N/kg

    world();

    void update(float delta);

    void draw(GLFWwindow* window);

    void spawn_rect(math::fvec2 position, math::fvec2 size, float angle);

    void spawn_soft_box(math::fvec2 position, float grid_size, math::ivec2 radius, float joint_mass, float joint_radius, float spring_stiffness, float spring_damping);

    void spawn_soft_wheel(math::fvec2 position, float radius, size_t vertices);

private:
    gl::shader common_shader;
    gl::square_mesh square;
    gl::circle_mesh circle;
};

}
