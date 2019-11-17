#include "Sprite.h"


Sprite::Sprite(Renderer * render, int columns, int rows) : Shape(render) {
	onCollision = false;
	vertex = new float[12]
	{
		-1.0f, -1.0f, 0.f,
		-1.0f,  1.0f, 0.f,
		1.0f, -1.0f, 0.f,
		1.0f,  1.0f, 0.f
	};
	SetVertices(vertex, 4);

	uvArray = new float[8]
	{
		0.0f,0.0f,
		0.0f,1.0f,
		1.0f,0.0f,
		1.0f,1.0f
	};

	SetTextureVertex(uvArray, 4);
}

void Sprite::SetTextureVertex(float * vertices, int count) {
	DisposeTexture();

	uvVtxCount = count;
	shouldDisposeTexture = true;
	uvBufferID = render->GenBuffer(vertices, sizeof(float)* count * 2);
}

void Sprite::UpdAnim(float deltaTime) {
	//uvArray = anim->UpdateAnimation(deltaTime);
	SetTextureVertex(uvArray, 4);
}

void Sprite::SetAnim(int initF, int finishF, float timePerF) {
	//anim->SetAnimation(initF, finishF, timePerF);
}

void Sprite::setCollision()
{
	onCollision = true;
}

bool Sprite::getCollision()
{
	return onCollision;
}

void Sprite::LoadMaterial(const char * texPath) {
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(texPath, &width, &height, &nrChannels, 0);
	textureID = render->ChargeTexture(width, height, data);
	stbi_image_free(data);
	material->BindTexture("fragmentTexCoords");
}

void Sprite::DisposeTexture() {
	if (shouldDisposeTexture) {
		render->DestroyBuffer(uvBufferID);
		shouldDisposeTexture = false;
	}
}

void Sprite::DrawMeshWithTexture(int typeDraw) {
	render->LoadIMatrix();
	render->SetWMatrix(WorldMatrix);

	if (material != NULL) {
		material->BindProgram();
		material->Bind("WVP");
		material->SetMatrixProperty(render->GetWVP());
	}

	render->BindTexture(textureID, uvBufferID);
	render->BeginDraw(0);
	render->BindBuffer(0, bufferId, 3);
	render->BeginDraw(1);
	render->BindBuffer(1, uvBufferID, 2);
	render->DrawBuffer(vertexCount, typeDraw);
	render->EndDraw(0);
	render->EndDraw(1);
}

void Sprite::Draw() {
	DrawMeshWithTexture(GL_TRIANGLE_STRIP);
}

Sprite::~Sprite() {
	delete[] vertex;
	delete[] uvArray;
}
