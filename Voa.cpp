#include "voa.h"

#include <glad/glad.h>
#include <iostream>



VertexArrayObject::VertexArrayObject(int attribCount) : attribCount(attribCount) {
	glGenVertexArrays(1, &ID);
	//glBindVertexArray(ID);
	this->bind();
}

VertexArrayObject::~VertexArrayObject() {
	glDeleteVertexArrays(1, &ID);
}

void VertexArrayObject::setAttribPointer(int index, int sizeNums, int enumData, bool normalized, int stride, void* dataPointer) {
	if (index > attribCount-1) {
		std::cout << "VOA INDEX DOESN'T EXIST" << '\n'; 
		return;
	}
	glVertexAttribPointer(index, sizeNums, enumData, normalized, stride, dataPointer);
	glEnableVertexAttribArray(index);
}

void VertexArrayObject::bind() const {
	glBindVertexArray(ID);
}

void VertexArrayObject::unbind() const {
	glBindVertexArray(0);
}


