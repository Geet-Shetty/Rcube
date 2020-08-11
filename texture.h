#pragma once 

using uint = unsigned int; 

class Texture {
private:
	uint ID;
	int enumType; 

public: 
	Texture(); 
	~Texture(); 

	void bind(int enumType); 
	static void vFlip(bool setVFlip);
	void generateTexture(const char* file, int rgbType);
};

