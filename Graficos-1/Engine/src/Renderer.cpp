#include "Renderer.h"
#include <glew.h>
#include <glfw3.h>
#include "Entity3D.h"


using namespace std;

Renderer* Renderer::renderer = nullptr;

vector<BSPPlane*> Renderer::planes;

bool Renderer::Start(Window* wnd) {
	cout << "Renderer::Start()" << endl;
	win = wnd;
	glfwMakeContextCurrent((GLFWwindow*)wnd->GetGLFWWindowPtr());

	glEnable(GL_BLEND);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	if (glewInit() != GLEW_OK) {
		cout << "Falló al inicializar GLEW\n" << endl;
		return -1;
	}

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	//ProjectionMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.f);

	ProjectionMatrix = glm::perspectiveFov(glm::radians(45.0f), 640.0f, 480.0f, 1.0f, 100.0f);

	cam = new Camera(win, glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0, 1, 0));

	WorldMatrix = glm::mat4(1.0f);

	UpdateWVP();

	renderer = this;
	
	f = new Frustum(cam->GetViewMatrix() * GetProjMatrix());

	for (int i = 0; i < planes.size(); i++)
	{
		planes[i]->CheckPositionWithPlane(Camera::thisCam->GetCameraPosition(), true);
	}

	return true;
}

bool Renderer::Stop() {
	cout << "Renderer::Stop()" << endl;
	return true;
}

void Renderer::setClearScreenColor(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
}

unsigned int Renderer::GenBuffer(float * buffer, int size)
{
	unsigned int vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW);
	return vertexBuffer;
}


void Renderer::BeginDraw(unsigned int atribID) {
	glEnableVertexAttribArray(atribID);
}

void Renderer::EndDraw(unsigned int atribID) {
	glDisableVertexAttribArray(atribID);
}

void Renderer::BindBuffer(unsigned int atribID, unsigned int vtxBuffer, unsigned int size) {
	glBindBuffer(GL_ARRAY_BUFFER, vtxBuffer);
	glVertexAttribPointer(
		atribID,            // debe corresponder en el shader.
		size,                  // tamaño
		GL_FLOAT,           // tipo
		GL_FALSE,           // normalizado?
		0,                  // Paso
		(void*)0            // desfase del buffer
	);
}

unsigned int Renderer::ChargeTexture(unsigned int width, unsigned int height, unsigned char * data, bool alpha)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data)
	{
		if(!alpha)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		cout << "cargo tex" << endl;
	}
	else
		cout << "fallo tex" << endl;

	return textureID;
}

void Renderer::BindTexture(unsigned int texture, unsigned int textureID)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(textureID, 0);
}

void Renderer::DrawBuffer(int size, int typeDraw)
{
	glDrawArrays(typeDraw, 0, size);
}

void Renderer::DestroyBuffer(unsigned int buffer)
{
	glDeleteBuffers(1, &buffer);
}

void Renderer::ClearScreen() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::SwapBuffer() {
	glfwSwapBuffers((GLFWwindow*)win->GetGLFWWindowPtr());
}

void Renderer::UpdateWVP()
{
	WVP = ProjectionMatrix * cam->GetViewMatrix() * WorldMatrix;
}

glm::mat4 & Renderer::GetWVP()
{
	return WVP;
}

void Renderer::LoadWorldMatrix()
{
	WorldMatrix = glm::mat4(1.0f);
}

void Renderer::SetWorldMatrix(glm::mat4 matrix)
{
	WorldMatrix = matrix;
	UpdateWVP();
}

void Renderer::MultiplyWorldMatrix(glm::mat4 matrix)
{
	WorldMatrix *= matrix;
	UpdateWVP();
}

Camera* Renderer::GetCam()
{
	return cam;
}

glm::mat4 Renderer::GetProjMatrix()
{
	return ProjectionMatrix;
}

void Renderer::CheckListedAndAddIfNot(string name)
{
	bool found = false;
	for (list<string>::iterator itBeg = CulledEntities.begin(); itBeg != CulledEntities.end(); itBeg++)
	{
		if (name == *itBeg)
		{
			found = true;
		}
	}

	if (!found)
	{
		CulledEntities.push_front(name);
		//cout << name.c_str() << endl;
	}
}

void Renderer::CheckListedAndRemoveIfIs(string name)
{
	bool found = false;
	for (list<string>::iterator itBeg = CulledEntities.begin(); itBeg != CulledEntities.end(); itBeg++)
	{
		if (name == *itBeg)
		{
			found = true;
		}
	}

	if (found)
	{
		CulledEntities.remove(name);
	}
}

Renderer::Renderer() {
}


Renderer::~Renderer() {
}
