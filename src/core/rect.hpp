#pragma once

#include <math/math.hpp>
#include <gl/square_mesh.hpp>

namespace soft_bodies::core {

class rect {
public:
    math::fvec2 position;
    math::fvec2 radius;
    float angle;
    math::fvec3 color;

    bool contains(math::fvec2 point) {
        math::fmat2 rot = math::fmat2{
            math::cos(angle), math::sin(angle),
            -math::sin(angle), math::cos(angle)
        };
        point = position + math::inverse(rot) * (point - position);

        // If the joint is inside the collision rectangle...
        math::fvec2 min = position - radius;
        math::fvec2 max = position + radius;

        return point.x > min.x && point.x < max.x && point.y > min.y && point.y < max.y;
    }
};

}
