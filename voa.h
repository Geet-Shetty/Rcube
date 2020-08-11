#pragma once 

using uint = unsigned int;
class VertexArrayObject {
private:
	uint ID;
	uint attribCount;
public:
	VertexArrayObject(int attribCount);
	~VertexArrayObject();

	void setAttribPointer(int index, int sizeNums, int enumData, bool normalized, int stride, void* dataPointer);

	void bind() const;
	void unbind() const;
};