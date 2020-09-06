#include "Camera.h"
#include <iostream>
#include "Input.h"
#include "BaseGame.h"
using namespace std;

Camera* Camera::thisCam = nullptr;
double Camera::xposs = 0.0f;
double Camera::yposs = 0.0f;

float Camera::yaw = 0.0f;
float Camera::pitch = 0.0f;

bool  Camera::firstMouse = true;
float Camera::lastX = 0.0f;
float Camera::lastY = 0.0f;

Camera::Camera(Window* w, glm::vec3 p, glm::vec3 t, glm::vec3 f, glm::vec3 u)
{
	actualWindow = w;
	glfwSetCursorPosCallback(w->GetGLFWWindowPtr(), &Camera::mouse_callback);
	thisCam = this;
	cameraPos = p; up = u; cameraFront = f;
	cameraTarget = t;
	LookAt();
}

void Camera::LookAt()
{
	cameraDirection = glm::normalize(cameraTarget - cameraPos);
	cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	cameraUp = glm::cross(cameraDirection, cameraRight);
	viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

glm::mat4 Camera::GetViewMatrix()
{
	return viewMatrix;
}

void Camera::UpdateCamera()
{
	bool moved = false;
	if(Input::GetKeyPressed(GLFW_KEY_W))
	{
		cameraPos += cameraSpeed * BaseGame::GetDeltaTime() * cameraFront;
		moved = true;
	}
	if (Input::GetKeyPressed(GLFW_KEY_S))
	{
		cameraPos -= cameraSpeed * BaseGame::GetDeltaTime() * cameraFront;
		moved = true;
	}
	if (Input::GetKeyPressed(GLFW_KEY_A))
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, up)) * BaseGame::GetDeltaTime() * cameraSpeed;
		moved = true;
	}
	if (Input::GetKeyPressed(GLFW_KEY_D))
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, up)) * BaseGame::GetDeltaTime() * cameraSpeed;
		moved = true;
	}

	if (moved)
	{
		LookAt();
		for (int i = 0; i < Renderer::planes.size(); i++)
		{
			Renderer::planes[i]->CheckPositionWithPlane(GetCameraPosition(), true);
		}
	}
		
}

glm::vec3 Camera::GetCameraPosition()
{
	return cameraPos;
}

glm::vec3 Camera::GetCameraDirection()
{
	return cameraFront;
}

void Camera::SetCameraSpeed(float s)
{
	cameraSpeed = s;
}

void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	Camera::thisCam->cameraFront = glm::normalize(direction);
	Camera::thisCam->LookAt();
}