#ifndef BSPPLANE_H
#define BSPPLANE_H

#include "glm.hpp"

#define DLLEXPORT __declspec(dllexport)

class DLLEXPORT BSPPlane
{
private:
	glm::vec4 plane;
	float cameraValue;
public:
	BSPPlane(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3);
	bool CheckPositionWithPlane(glm::vec3 pos, bool isCamera);
	bool CheckAABBWithPlane(struct Bounds b);
};

#endif
