// THIS IS OLD AND BADLY WRITTEN CODE; NOT FOR REFERENCE

#pragma once

#include <cstdint>

namespace soft_bodies::gl {

class mesh {
public:
    virtual ~mesh();

    void bind() const;

    uint32_t get_index_count() const;

protected:
    uint32_t vao;
    uint32_t vbos[2];
    uint32_t index_count;

    mesh();
};

}
