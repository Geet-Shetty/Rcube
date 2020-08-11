#pragma once 

#include <GLFW/glfw3.h>
#include <string>

using uint = unsigned int; 

constexpr uint MAX_WIDTH{ 1920 };
constexpr uint MAX_HEIGHT{ 1080 };

enum CursorMode { disabled, normal };

class Window {

private: 
	GLFWwindow* window; 
	const std::string title; 
	uint width;
	uint height; 

public:
	Window(const std::string& title, uint w = 800, uint h = 600, bool makeContextCurrent = true);
	~Window();

	// getters 
	uint getWidth() const; 

	uint getHeight() const;

	GLFWwindow* getWindow() const;

	const std::string& getTitle() const; 

	bool getWindowShouldClose() const; 

	void getCursorPosition(double* x, double* y);

	void swapBuffers(); 
	
	// setters
	void setFramebufferSizeCallback(void (*f) (GLFWwindow*, int, int)); // render box resize 

	void setMouseScrollCallback(void (*f)(GLFWwindow*, double, double)); // scroll wheel movement 

	void setCursorPosCallback(void (*f)(GLFWwindow*, double, double)); // cursor movement 

	void setKeyCallback(void (*f)(GLFWwindow*, int, int, int, int)); // key presses

	void setMouseButtonCallback(void (*f)(GLFWwindow*, int, int, int)); // mouse presses

	void setInputMode(CursorMode mode); 

};