#include "window.h" 
#include <iostream>

Window::Window(const std::string& title, uint w, uint h, bool makeContextCurrent)
	:title(title), width(w), height(h)
{
	if (w > MAX_WIDTH)
		std::cout << "TOO BIG WINDOW WIDTH" << '\n'; 

	if (h > MAX_HEIGHT)
		std::cout << "TOO BIG WINDOW HEIGHT" << '\n';

	window = glfwCreateWindow(w, h, title.c_str(), NULL, NULL);

	if (!window) {
		std::cout << "WINDOW NULL ERROR" << '\n';
		glfwTerminate(); 
	}

	if (makeContextCurrent)
		glfwMakeContextCurrent(window); 

}

Window::~Window() {
	glfwDestroyWindow(window);
}

uint Window::getWidth() const {
	return width; 
}

uint Window::getHeight() const {
	return height;
}

const std::string& Window::getTitle() const {
	return title;
}

GLFWwindow* Window::getWindow() const {
	return window; 
}

bool Window::getWindowShouldClose() const {
	return glfwWindowShouldClose(window);
}

void Window::getCursorPosition(double* x, double* y) {
	glfwGetCursorPos(window, x, y);
}

void Window::swapBuffers() {
	glfwSwapBuffers(window);
}

void Window::setFramebufferSizeCallback(void (*f) (GLFWwindow*, int, int)) {
	glfwSetFramebufferSizeCallback(window, f);
}

void Window::setKeyCallback(void (*f)(GLFWwindow*, int, int, int, int)) {
	glfwSetKeyCallback(window, f);
}

void Window::setMouseButtonCallback(void (*f)(GLFWwindow*, int, int, int)) {
	glfwSetMouseButtonCallback(window, f);
}

void Window::setMouseScrollCallback(void (*f)(GLFWwindow*, double, double)) {
	glfwSetScrollCallback(window, f);
}

void Window::setCursorPosCallback(void (*f)(GLFWwindow*, double, double)) {
	glfwSetCursorPosCallback(window, f);
}

void Window::setInputMode(CursorMode mode) {
	if (mode == disabled) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else if (mode == normal) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}