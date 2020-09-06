#include "CollisionBox.h"

#include "Camera.h"
#include "glew.h"
#include "Renderer.h"
#include "Shader.h"
#include "Entity3D.h"
Shader* CollisionBox::wireframeShader = nullptr;

CollisionBox::CollisionBox()
{
	unsigned int cubeIndices[] = {
		0,1,2,
		0,3,2,
		0,4,6,
		0,3,6,
		3,2,6,
		6,7,2,
		1,5,7,
		1,2,7,
		0,1,4,
		1,4,5,
		6,5,4,
		6,5,7
	};

	for(int i=0;i<36;i++)
	{
		indicesVector.push_back(cubeIndices[i]);
	}
	
	GenerateBuffers();
	Setup();
}

CollisionBox::~CollisionBox()
{
	
}

void CollisionBox::GenerateBoundingBox(Bounds b, mat4 modelMat)
{
	SetBounds(0, vec3(b.minX, b.minY, b.minZ));
	SetBounds(1, vec3(b.maxX, b.minY, b.minZ));
	SetBounds(2, vec3(b.maxX, b.maxY, b.minZ));
	SetBounds(3, vec3(b.minX, b.maxY, b.minZ));
	SetBounds(4, vec3(b.minX, b.minY, b.maxZ));
	SetBounds(5, vec3(b.maxX, b.minY, b.maxZ));
	SetBounds(6, vec3(b.minX, b.maxY, b.maxZ));
	SetBounds(7, vec3(b.maxX, b.maxY, b.maxZ));

	for (int i = 0; i < TOTALVERTICES; i++)
		vertices[i] = modelMat * glm::vec4(vertices[i], 1.f);
	
	Setup();
}

void CollisionBox::GenerateBoundingBox(Bounds b)
{
	SetBounds(0, vec3(b.minX, b.minY, b.minZ));
	SetBounds(1, vec3(b.maxX, b.minY, b.minZ));
	SetBounds(2, vec3(b.maxX, b.maxY, b.minZ));
	SetBounds(3, vec3(b.minX, b.maxY, b.minZ));
	SetBounds(4, vec3(b.minX, b.minY, b.maxZ));
	SetBounds(5, vec3(b.maxX, b.minY, b.maxZ));
	SetBounds(6, vec3(b.minX, b.maxY, b.maxZ));
	SetBounds(7, vec3(b.maxX, b.maxY, b.maxZ));

	mins.x = b.minX;
	mins.y = b.minY;
	mins.z = b.minZ;

	maxs.x = b.maxX;
	maxs.y = b.maxY;
	maxs.z = b.maxZ;

	Setup();
}

void CollisionBox::SetBounds(int boundIndex, vec3 newBoundValue)
{
	vertices[boundIndex] = newBoundValue;
}

void CollisionBox::GenerateBuffers()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
}

void CollisionBox::DrawCollisionBox(mat4 worldMatrix)
{
	glm::mat4 projection = Renderer::renderer->GetProjMatrix();
	glm::mat4 view = Camera::thisCam->GetViewMatrix();
	
	wireframeShader->use();
	wireframeShader->setMat4("projection", projection);
	wireframeShader->setMat4("view", view);
	wireframeShader->setMat4("model", mat4(1.f));
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(vao);
	// Draw the box
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void CollisionBox::Setup()
{
	glBindVertexArray(vao);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesVector.size() * sizeof(unsigned int), &indicesVector[0], GL_DYNAMIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	glBindVertexArray(0);
}

vec3 CollisionBox::GetVec3Max()
{
	return maxs;
}

vec3 CollisionBox::GetVec3Min()
{
	return mins;
}
