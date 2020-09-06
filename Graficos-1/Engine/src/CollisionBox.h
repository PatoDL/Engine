#ifndef COLLISION_BOX_H
#define COLLISION_BOX_H

#include <vector>

#include "glm.hpp"

#define DLLEXPORT _declspec(dllexport)

using namespace glm;

#define TOTALVERTICES 8

class Shader;


struct Bounds
{
	float minX = INT32_MAX;
	float minY = INT32_MAX;
	float minZ = INT32_MAX;
	float maxX = INT32_MIN;
	float maxY = INT32_MIN;
	float maxZ = INT32_MIN;
};


class DLLEXPORT CollisionBox
{
private:
	
	unsigned int vao, vbo, ebo;
	std::vector<unsigned int> indicesVector;

	vec3 mins;
	vec3 maxs;

public:
	CollisionBox();
	~CollisionBox();
	void GenerateBoundingBox(Bounds b, mat4 modelMat);
	void GenerateBoundingBox(Bounds b);
	void SetBounds(int boundIndex, vec3 newBoundValue);
	void GenerateBuffers();
	void DrawCollisionBox(mat4 worldMatrix);
	static Shader* wireframeShader;
	void Setup();
	vec3 vertices[TOTALVERTICES];
	vec3 GetVec3Max();
	vec3 GetVec3Min();
};

#endif
