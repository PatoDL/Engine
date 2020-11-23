#include "Entity3D.h"

#include "BaseGame.h"
#include "gtx/quaternion.hpp"
#include "Renderer.h"
#include "Camera.h"
#include "Model.h"

Entity3D::Entity3D(Entity3D* newParent)
{
	parent = nullptr;
	position = vec3(0.f);
	rotation = vec3(0.f);
	scale = vec3(1.f);
	
	if(newParent)
	{
		SetParent(newParent);
	}
	else if(name != "root" || this != BaseGame::GetRootEntity())
	{	
		SetParent(BaseGame::GetRootEntity());
	}

	entityType = entity;

	collisionBox = new CollisionBox();
	AABB = new CollisionBox();
}

Entity3D::Entity3D(string newName)
{
	entityType = entity;
	
	name = newName;

	collisionBox = new CollisionBox();
	AABB = new CollisionBox();
	
	/*BaseGame::GetRootEntity()->CalculateBoundsWithChilds();
	BaseGame::GetRootEntity()->UpdateModelMatAndBoundingBox();*/
}

Entity3D::~Entity3D()
{
	
}

void Entity3D::SetParent(Entity3D* p)
{
	if (parent)
	{
		parent->UnsetChild(this);
	}
	if (p)
	{
		parent = p;
		modelMatrix = glm::inverse(parent->worldModel) * worldModel;
		parent->SetChild(this);
	}
}

void Entity3D::SetChild(Entity3D* c)
{
	childs.push_front(c);
}

Entity3D* Entity3D::GetChild(string childName)
{
	Entity3D* toGet = nullptr;
	if (name == childName)
	{
		return this;
	}
	else
	{
		for (list<Entity3D*>::iterator itBeg = childs.begin(); itBeg != childs.end(); ++itBeg)
		{
			toGet = (*itBeg)->GetChild(childName);
			if (toGet != nullptr)
				break;
		}
	}
	return toGet;
}

void Entity3D::UnsetChild(Entity3D* c)
{
	for (list<Entity3D*>::iterator itBeg = childs.begin(); itBeg != childs.end(); ++itBeg)
	{
		if (*itBeg == c)
		{
			childs.remove(*itBeg);
			break;
		}
	}
}

void Entity3D::SetPos(vec3 pos)
{
	position = pos;
	modelMatrix = glm::translate(modelMatrix, position);
	BaseGame::GetRootEntity()->UpdateModelMatAndBoundingBox();
}

void Entity3D::SetRot(float angle, vec3 axis)
{
	rotation.x += angle * axis.x;
	rotation.y += angle * axis.y;
	rotation.z += angle * axis.z;
	
	modelMatrix = glm::rotate(modelMatrix, radians(angle), axis);
	BaseGame::GetRootEntity()->UpdateModelMatAndBoundingBox();
}

void Entity3D::SetScale(vec3 sc)
{
	scale = sc;
	modelMatrix = glm::scale(modelMatrix, scale);
	BaseGame::GetRootEntity()->UpdateModelMatAndBoundingBox();
}

void Entity3D::Draw(Shader shader)
{
	for (list<Entity3D*>::iterator itBeg = childs.begin(); itBeg != childs.end(); ++itBeg)
	{
		Entity3D* ent = (*itBeg);
		if (ent->shallDraw)
		{
			if (ent->entityType == mesh)
			{
				Mesh* m = static_cast<Mesh*> (ent);
				Renderer::renderer->DrawMesh(shader, &bounds, worldModel, m);
			}
			ent->Draw(shader);
		}
		ent->shallDraw = true;
	}
	if(shallDraw)
		AABB->DrawCollisionBox(worldModel);

	shallDraw = true;
}

void Entity3D::UpdateModelMatrix()
{
	if(parent != nullptr)
	{
		worldModel = parent->worldModel * modelMatrix;
	}

	for (list<Entity3D*>::iterator itBeg = childs.begin(); itBeg != childs.end(); ++itBeg)
	{
		(*itBeg)->UpdateModelMatrix();
	}
}

void Entity3D::SetName(string newName)
{
	name = newName;
}

void Entity3D::SetTag(string newTag)
{
	tag = newTag;
}

void Entity3D::SetModelMatrix(glm::mat4 newModelMatrix)
{
	modelMatrix = newModelMatrix;
}

void Entity3D::GetAllChildsNames()
{
	cout << name << endl;

	for (list<Entity3D*>::iterator itBeg = childs.begin(); itBeg != childs.end(); ++itBeg)
	{
		(*itBeg)->GetAllChildsNames();
	}
}

void Entity3D::GetAllChildsTypes()
{
	cout << entityType << endl;

	for (list<Entity3D*>::iterator itBeg = childs.begin(); itBeg != childs.end(); ++itBeg)
	{
		(*itBeg)->GetAllChildsTypes();
	}
}

Bounds Entity3D::UpdateModelMatAndBoundingBox()
{
	if (parent != nullptr)
		worldModel = parent->worldModel * modelMatrix;

	Bounds childBounds;
	
	for (list<Entity3D*>::iterator itBeg = childs.begin(); itBeg != childs.end(); ++itBeg)
	{
		childBounds = CalculateBounds(childBounds, (*itBeg)->UpdateModelMatAndBoundingBox());
	}

	bounds = GenerateBounds(collisionBox->vertices, worldModel);
	
	CalculateBounds(childBounds);
	AABB->GenerateBoundingBox(bounds);
	
	return bounds;
}

void Entity3D::CalculateBoundsWithChilds()
{
	if (!childs.empty())
	{
		for (list<Entity3D*>::iterator itBeg = childs.begin(); itBeg != childs.end(); ++itBeg)
		{
			(*itBeg)->CalculateBoundsWithChilds();
			bounds = CalculateBounds(bounds, (*itBeg)->bounds);
		}
	}
	
	collisionBox->GenerateBoundingBox(bounds);
}

void Entity3D::CalculateBounds(Bounds otherBounds)
{
	bounds.minX = bounds.minX < otherBounds.minX ? bounds.minX : otherBounds.minX;
	bounds.minY = bounds.minY < otherBounds.minY ? bounds.minY : otherBounds.minY;
	bounds.minZ = bounds.minZ < otherBounds.minZ ? bounds.minZ : otherBounds.minZ;
	bounds.maxX = bounds.maxX > otherBounds.maxX ? bounds.maxX : otherBounds.maxX;
	bounds.maxY = bounds.maxY > otherBounds.maxY ? bounds.maxY : otherBounds.maxY;
	bounds.maxZ = bounds.maxZ > otherBounds.maxZ ? bounds.maxZ : otherBounds.maxZ;
}

Bounds Entity3D::CalculateBounds(Bounds b1, Bounds b2)
{
	Bounds returnBounds;
	returnBounds.minX = b1.minX < b2.minX ? b1.minX : b2.minX;
	returnBounds.minY = b1.minY < b2.minY ? b1.minY : b2.minY;
	returnBounds.minZ = b1.minZ < b2.minZ ? b1.minZ : b2.minZ;
	returnBounds.maxX = b1.maxX > b2.maxX ? b1.maxX : b2.maxX;
	returnBounds.maxY = b1.maxY > b2.maxY ? b1.maxY : b2.maxY;
	returnBounds.maxZ = b1.maxZ > b2.maxZ ? b1.maxZ : b2.maxZ;
	return returnBounds;
}

Bounds Entity3D::GenerateBounds(vec3 v[], mat4 mat)
{
	vec3 v2[TOTALVERTICES];
	for(int i=0;i<TOTALVERTICES;i++)
	{
		v2[i] = mat * vec4(v[i],1.f);
	}

	return GenerateBoundsByVertex(v2);
}

Bounds Entity3D::GenerateBoundsByVertex(vec3 v[])
{
	Bounds returnBounds;
	for(int i=0;i<TOTALVERTICES;i++)
	{
		returnBounds.minX = v[i].x < returnBounds.minX ? v[i].x : returnBounds.minX;
		returnBounds.minY = v[i].y < returnBounds.minY ? v[i].y : returnBounds.minY;
		returnBounds.minZ = v[i].z < returnBounds.minZ ? v[i].z : returnBounds.minZ;
		returnBounds.maxX = v[i].x > returnBounds.maxX ? v[i].x : returnBounds.maxX;
		returnBounds.maxY = v[i].y > returnBounds.maxY ? v[i].y : returnBounds.maxY;
		returnBounds.maxZ = v[i].z > returnBounds.maxZ ? v[i].z : returnBounds.maxZ;
	}
	return returnBounds;
}

Bounds Entity3D::GenerateBoundsByVertex(vector<vec3> v)
{
	Bounds returnBounds;
	for (int i = 0; i < v.size(); i++)
	{
		returnBounds.minX = v[i].x < returnBounds.minX ? v[i].x : returnBounds.minX;
		returnBounds.minY = v[i].y < returnBounds.minY ? v[i].y : returnBounds.minY;
		returnBounds.minZ = v[i].z < returnBounds.minZ ? v[i].z : returnBounds.minZ;
		returnBounds.maxX = v[i].x > returnBounds.maxX ? v[i].x : returnBounds.maxX;
		returnBounds.maxY = v[i].y > returnBounds.maxY ? v[i].y : returnBounds.maxY;
		returnBounds.maxZ = v[i].z > returnBounds.maxZ ? v[i].z : returnBounds.maxZ;
	}
	return returnBounds;
}
