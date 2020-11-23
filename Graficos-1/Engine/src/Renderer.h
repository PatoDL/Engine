#ifndef RENDERER_H
#define RENDERER_H

#include "Window.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <glfw3.h>
#include <iostream>
#include "Camera.h"
#include "Frustum.h"
#include "BSPPlane.h"
#include <vector>
#include <list>

using namespace std;
using namespace glm;

class Shader;

class DLLEXPORT Renderer
{
private:
	Window* win;
	Camera* cam;
	unsigned int VertexArrayID;
	unsigned int ColorVertexArrayID;
	glm::mat4 WorldMatrix;
	glm::mat4 ProjectionMatrix;
	glm::mat4 WVP;
	bool bspEnabled = true;
	bool frustumCullingEnabled = true;
public:
	bool Start(Window* wnd);
	bool Stop();
	void setClearScreenColor(float r, float g, float b, float a);
	unsigned int GenBuffer(float* buffer, int size);
	void BeginDraw(unsigned int atribID);
	void EndDraw(unsigned int atribID);
	void BindBuffer(unsigned int atribID, unsigned int vtxBuffer, unsigned int size);
	unsigned int ChargeTexture(unsigned int width, unsigned int height, unsigned char * data, bool alpha);
	void BindTexture(unsigned int texture, unsigned int textureID);
	void DrawBuffer(int size, int typeDraw);
	void DestroyBuffer(unsigned int buffer);
	void ClearScreen();
	void SwapBuffer();
	void UpdateWVP();
	glm::mat4& GetWVP();
	void SetWorldMatrix(glm::mat4 matrix);
	void LoadWorldMatrix();
	void MultiplyWorldMatrix(glm::mat4 matrix);
	Camera* GetCam();
	glm::mat4 GetProjMatrix();

	void DrawMesh(Shader shader, struct Bounds* b, mat4 worldModel, class Mesh* m);

	Frustum* f;
	
	void SetBSPEnabled(bool e) { bspEnabled = e; }

	bool GetBSPEnabled() { return bspEnabled; }

	void SetFCEnabled(bool e) { frustumCullingEnabled = e; }

	bool GetFCEnabled() { return frustumCullingEnabled; }

	int culledEntitiesAmount = 0;

	static Renderer* renderer;

	void CheckBSPVisibility(class Entity3D* e, int plane);
	void CheckPlanes();
	void CheckFrustumCulling(class Entity3D* e);
	
	static vector<BSPPlane*> planes;

	Renderer();
	~Renderer();
};

#endif