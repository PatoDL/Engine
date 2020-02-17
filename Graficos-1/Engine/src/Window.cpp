#include "Window.h"

using namespace glm;

Window::Window()
{
}

Window::~Window()
{
}
bool Window::Start(int width, int height, const char* name)
{
	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(width, height, name, NULL, NULL);

	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return false;
	}

	return true;
}

bool Window::Stop()
{
	if (window != NULL)
		glfwDestroyWindow((GLFWwindow*)window);
	window = NULL;
	glfwTerminate();
	return true;
}

bool Window::ShouldClose()
{
	if (window)
		return glfwWindowShouldClose((GLFWwindow*)window);

}

void Window::PollEvents()
{
	glfwPollEvents();
}

void* Window::GetWindow() {
	return window;
}