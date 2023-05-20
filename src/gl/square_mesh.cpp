// THIS IS OLD AND BADLY WRITTEN CODE; NOT FOR REFERENCE

#include <pch.hpp>

#include <gl/square_mesh.hpp>

#include <math/math.hpp>

namespace soft_bodies::gl {

square_mesh::square_mesh() {
	unsigned int vertex_count = 4;
	float       *vertex_data =
	    new float[vertex_count * 2]{-1, -1, 1, -1, 1, 1, -1, 1};

	index_count           = 6;
	unsigned int *indices = new unsigned int[index_count]{0, 1, 2, 2, 3, 0};

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	glBufferData(
	    GL_ARRAY_BUFFER,
	    vertex_count * sizeof(float) * 2,
	    vertex_data,
	    GL_STATIC_DRAW
	);
	glVertexAttribPointer(
	    0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void *)0
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
	glBufferData(
	    GL_ELEMENT_ARRAY_BUFFER,
	    index_count * sizeof(unsigned int),
	    indices,
	    GL_STATIC_DRAW
	);

	delete[] vertex_data;
	delete[] indices;
}

} // namespace soft_bodies::gl
