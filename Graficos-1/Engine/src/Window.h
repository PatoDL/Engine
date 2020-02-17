#ifndef WINDOW_H
#define WINDOW_H

#include <stdio.h>
#include <stdlib.h>
#include "glew.h"
#include <glfw3.h>
#include <glm.hpp>
#include "Exports.h"
//#define DLLEXPORT __declspec(dllexport)

class ENGINE_API Window
{
private:
	void * window;
public:
	Window();
	~Window();
	bool Start(int width, int height, const char* name);
	bool Stop();
	const void* GetWindowPtr() { return (const void*)window; }
	bool ShouldClose();
	void PollEvents();
	void* GetWindow();
};
#endif

