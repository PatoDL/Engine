#ifndef SHAPE_H
#define SHAPE_H

#include "Entity.h"

class Shape : public Entity
{
protected:
	bool shouldDispose;
	bool shouldDisposeColor;

	int vertexCount;
	int colorVertexCount;
	int idxVtxCount;

	unsigned int colorBufferID;
	unsigned int bufferID;
	unsigned int indexBufferID;
	unsigned int* indxVertex;
	float* vertex;
	float* colorVertex;
public:
	Shape(Renderer* r);
	void Draw();
	void DrawMesh(int typeDraw);
	void SetVertices(float* vertices, int count);
	void SetColorVertex(float* vertices, int count);
	void SetIndexVertices(unsigned int* vertices, int count);
	void Dispose();
	void DisposeColor();
	~Shape();
};

#endif
