#include "BSPPlane.h"

BSPPlane::BSPPlane(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3)
{
	glm::vec3 vecA = point1 - point2;
	glm::vec3 vecB = point3 - point2;

	glm::vec3 normal = glm::cross(vecA, vecB);
	float d = -1.f * (normal.x * point2.x + normal.y * point2.y + normal.z * point2.z);

	plane = glm::vec4(normal, d);
}

bool BSPPlane::CheckPositionWithPlane(glm::vec3 pos, bool isCamera)
{
	float b = plane.x * pos.x + plane.y * pos.y + plane.z * pos.z + plane.w;

	if (isCamera)
	{
		cameraValue = b;
		return false;
	}

	if ((cameraValue > 0 && b > 0) || (cameraValue < 0 && b < 0) || (cameraValue == 0 || b == 0))
		return true;

	return false;
}

bool BSPPlane::CheckAABBWithPlane(CollisionBox * aabb)
{
	return false;
}
