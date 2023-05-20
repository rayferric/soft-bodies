// THIS IS OLD AND BADLY WRITTEN CODE; NOT FOR REFERENCE

#include <pch.hpp>

#include <gl/circle_mesh.hpp>

#include <math/math.hpp>

namespace soft_bodies::gl {

circle_mesh::circle_mesh() {
	unsigned int             vertex_count = 64 + 1;
	std::vector<math::fvec2> vertices;
	vertices.reserve(vertex_count);
	vertices.push_back(math::fvec2(0));
	for (unsigned int i = 0; i < vertex_count - 1; i++) {
		float angle = (float)i / (float)(vertex_count - 1) * math::pi * 2;
		vertices.push_back(math::fvec2(math::cos(angle), math::sin(angle)));
	}

	std::vector<uint32_t> indices;
	indices.reserve((vertex_count - 1) * 3);
	for (unsigned int i = 0; i < vertex_count; i++) {
		indices.push_back(0);
		indices.push_back(i + 1);
		indices.push_back((i + 1) % (vertex_count - 1) + 1);
	}
	index_count = indices.size();

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	glBufferData(
	    GL_ARRAY_BUFFER,
	    vertices.size() * sizeof(math::fvec2),
	    reinterpret_cast<float *>(vertices.data()),
	    GL_STATIC_DRAW
	);
	glVertexAttribPointer(
	    0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void *)0
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
	glBufferData(
	    GL_ELEMENT_ARRAY_BUFFER,
	    index_count * sizeof(uint32_t),
	    indices.data(),
	    GL_STATIC_DRAW
	);
}

} // namespace soft_bodies::gl
