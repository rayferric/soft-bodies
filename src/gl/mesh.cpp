// THIS IS OLD AND BADLY WRITTEN CODE; NOT FOR REFERENCE

#include <pch.hpp>

#include <gl/mesh.hpp>

namespace soft_bodies::gl {

mesh::~mesh() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(2, vbos);
}

void mesh::bind() const {
	glBindVertexArray(vao);
}

uint32_t mesh::get_index_count() const {
	return index_count;
}

mesh::mesh() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(2, vbos);
	glBindVertexArray(vao);
}

} // namespace soft_bodies::gl
