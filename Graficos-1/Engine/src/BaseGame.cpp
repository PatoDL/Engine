#include "BaseGame.h"
#include <iostream>
#include "Window.h"
#include "Renderer.h"
#include <ctime>
#include "glew.h"
#include "glfw3.h"

void BaseGame::MostrarAlgo()
{
	std::cout << "heh" << std::endl;
}

bool BaseGame::Init()
{
	window = new Window(640, 480, "Ventana");
	window->Init();

	renderer = new Renderer();
	if (!renderer->Init(window))
	{
		cout << "renderer pincho" << endl;
		return false;
	}

	renderer->SetClearColor(0.5f, 0.0f, 0.5f, 1.0f);
	renderer->ClearScreen();
	return true;
}

void BaseGame::Update()
{
	while(!window->GetOpened())
	{

		renderer->ClearScreen();
		

		//glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, nullptr);

		window->SwapBuffers();

		window->PollEvents();
	}
}

void BaseGame::DeInit()
{
	renderer->DeInit();
	window->DeInit();
	delete window;
}