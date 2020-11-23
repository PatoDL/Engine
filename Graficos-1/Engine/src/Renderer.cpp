#include "Renderer.h"
#include <glew.h>
#include <glfw3.h>
#include "Entity3D.h"
#include "CollisionBox.h"
#include "Mesh.h"
#include "BaseGame.h"

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

void Renderer::CheckBSPVisibility(Entity3D* e, int plane)
{
	if (!e->shallDraw)
		return;

	if (bspEnabled)
	{
		if (e->GetTag() != "BSP")
		{
			if (!planes[plane]->CheckAABBWithPlane(e->bounds))
			{
				e->shallDraw = false;
			}
		}
	}
		
	if (e->shallDraw)
	{
		if (e->GetTag() != "BSP" && plane == 2)
		{
			culledEntitiesAmount++;
			//cout << e->GetName().c_str()<<endl;
		}
	
		for (list<Entity3D*>::iterator iterB = e->childs.begin(); iterB != e->childs.end(); ++iterB)
		{
			CheckBSPVisibility((*iterB), plane);
		}
	}
}

void Renderer::CheckPlanes()
{
	for (int i = 0; i < planes.size(); i++)
	{
		CheckBSPVisibility(BaseGame::GetRootEntity(), i);
	}
}

void Renderer::CheckFrustumCulling(Entity3D * e)
{
	if (!f->IsBoxVisible(e->AABB->GetVec3Min(), e->AABB->GetVec3Max()) && frustumCullingEnabled)
	{
		e->shallDraw = false;
	}
	else
	{
		//if (e->GetTag() != "BSP")
		//{
		//	culledEntitiesAmount++;
		//}

		for (list<Entity3D*>::iterator iterB = e->childs.begin(); iterB != e->childs.end(); ++iterB)
		{
			CheckFrustumCulling((*iterB));
		}
	}
}

void Renderer::DrawMesh(Shader shader, Bounds* b, mat4 worldModel, Mesh* m)
{
	shader.use();

	glm::mat4 projection = Renderer::renderer->GetProjMatrix();
	glm::mat4 view = Camera::thisCam->GetViewMatrix();

	shader.setMat4("proj", projection);
	shader.setMat4("view", view);
	shader.setMat4("model", worldModel);

	// bind appropriate textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < m->textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		string number;
		string name = m->textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++); // transfer unsigned int to stream
		else if (name == "texture_normal")
			number = std::to_string(normalNr++); // transfer unsigned int to stream
		else if (name == "texture_height")
			number = std::to_string(heightNr++); // transfer unsigned int to stream

		// now set the sampler to the correct texture unit
		shader.setInt((name + number).c_str(), i);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, m->textures[i].id);
	}

	// draw mesh
	glBindVertexArray(m->VAO);
	glDrawElements(GL_TRIANGLES, m->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}

Renderer::Renderer() {
}

Renderer::~Renderer() {
}