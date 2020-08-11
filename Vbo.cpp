#include "vbo.h"

#include <glad/glad.h>


VertexBufferObject::VertexBufferObject(const void* data, int sizeOf, int enumUsage) {
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	//this->bind();
	glBufferData(GL_ARRAY_BUFFER, sizeOf, data, enumUsage);
}

VertexBufferObject::~VertexBufferObject() {
	glDeleteBuffers(1, &ID);
}

void VertexBufferObject::bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VertexBufferObject::unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

