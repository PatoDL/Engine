#include "BaseGame.h"
#include <iostream>
#include "Window.h"
#include <ctime>

using namespace Basegame;

void BaseGame::MostrarAlgo()
{
	std::cout << "heh" << std::endl;
}

void BaseGame::Init()
{
	window = new Window(640, 480, "Ventana");
	window->Init();
}

void BaseGame::Update()
{
	//glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Queremos OpenGL 3.3
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Para hacer feliz a MacOS ; Aunque no deber�a ser necesaria
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //No queremos el viejo OpenGL 

	glewExperimental = true;
	glewInit();

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	static const GLfloat g_vertex_buffer_data[] = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
	};

	// Identificar el vertex buffer
	GLuint vertexbuffer;
	// Generar un buffer, poner el resultado en el vertexbuffer que acabamos de crear
	glGenBuffers(1, &vertexbuffer);
	// Los siguientes comandos le dar�n caracter�sticas especiales al 'vertexbuffer' 
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Darle nuestros v�rtices a  OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	while(!window->GetOpened())
	{
		//window->SetBackgroundColor(rand() % 2, rand() % 2, rand() % 2, 1);

		window->Clear();
		//1rst attribute buffer : v�rtices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // atributo 0. No hay raz�n particular para el 0, pero debe corresponder en el shader.
			3,                  // tama�o
			GL_FLOAT,           // tipo
			GL_FALSE,           // normalizado?
			0,                    // Paso
			(void*)0            // desfase del buffer
		);
		// Dibujar el tri�ngulo !
		glDrawArrays(GL_TRIANGLES, 0, 3); // Empezar desde el v�rtice 0S; 3 v�rtices en total -> 1 tri�ngulo
		glDisableVertexAttribArray(0);

		window->SwapBuffers();
		window->PollEvents();
	}
}

void BaseGame::DeInit()
{
	window->DeInit();
	delete window;
}