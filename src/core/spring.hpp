#pragma once

#include <math/math.hpp>

namespace soft_bodies::core {

class joint;

class spring {
public:
    joint *start, *end;
    float rest_length;
    float stiffness;
    float damping;

    spring(joint *start, joint *end, float rest_length, float stiffness, float damping);

    // + = force towards the spring
    // - = force away from the spring
    float force() const;

    math::fvec2 center() const;

    float length() const;
};

}
