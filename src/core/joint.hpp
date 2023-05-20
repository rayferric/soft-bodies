#pragma once

#include <vector>

#include <math/math.hpp>

namespace soft_bodies::core {

class spring;

class joint {
public:
    std::vector<spring *> springs;
    math::fvec2 position;
    math::fvec2 velocity;
    math::fvec2 force;
    float mass;
    float radius;
    math::fvec3 color;
};

}
