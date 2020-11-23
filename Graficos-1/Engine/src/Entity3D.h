#ifndef ENTITY3D
#define ENTITY3D

#include <list>
#include <glm.hpp>
#include <vector>

#include "Shader.h"
#include "CollisionBox.h"

#ifndef DLLEXPORT
#define DLLEXPORT __declspec(dllexport)
#endif // !DLLEXPORT

using namespace glm;
using namespace std;

enum EntityType
{
	unassigned,
	mesh,
	model,
	entity
};

class DLLEXPORT Entity3D
{
protected:
	Entity3D* parent;
	string name;
	string tag = "untagged";

	vec3 position;
	vec3 rotation;
	vec3 scale;
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	
	CollisionBox* collisionBox;
	
	vector<vec3> verticesVector;
	
public:
	Entity3D(Entity3D* newParent = nullptr);
	Entity3D(string newName);
	~Entity3D();
	void SetParent(Entity3D* p);
	Entity3D* GetParent() { return parent; }
	void SetChild(Entity3D* c);
	Entity3D* GetChild(string childName);
	list<Entity3D*> childs;
	void UnsetChild(Entity3D* c);
	void SetPos(vec3 pos);
	vec3 GetPos(){ return position; };
	void SetRot(float angle, vec3 axis);
	vec3 GetRot() { return rotation; };
	void SetScale(vec3 sc);
	vec3 GetScale() { return scale; };
	void Draw(Shader shader);
	void UpdateModelMatrix();
	//////////////////////////
	string GetName() const { return name; }
	void SetName(string newName);
	string GetTag() const { return tag; }
	void SetTag(string newTag);
	glm::mat4 GetModelMatrix() const { return modelMatrix; }
	void SetModelMatrix(glm::mat4 newModelMatrix);
	////////////////////////////
	EntityType entityType = unassigned;
	Bounds bounds;
	CollisionBox* GetCollisionBox() const { return collisionBox; }
	Bounds UpdateModelMatAndBoundingBox();
	void CalculateBounds(Bounds otherBounds);
	Bounds CalculateBounds(Bounds b1, Bounds b2);
	Bounds GenerateBounds(vec3 v[], mat4 mat);
	Bounds GenerateBoundsByVertex(vec3 v[]);
	Bounds GenerateBoundsByVertex(vector<vec3> v);
	void CalculateBoundsWithChilds();
	CollisionBox* AABB;
	bool shallDraw = true;
	////////////////////////////
	void GetAllChildsNames();
	void GetAllChildsTypes();

	glm::mat4 worldModel = glm::mat4(1.0f);
};

#endif // !ENTITY3D

