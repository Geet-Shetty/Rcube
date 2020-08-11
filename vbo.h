#pragma once 

using uint = unsigned int;

class VertexBufferObject {
private:
	uint ID;
public:
	VertexBufferObject(const void* data, int sizeOf, int enumUsage);
	~VertexBufferObject();

	void bind() const;
	void unbind() const;
};