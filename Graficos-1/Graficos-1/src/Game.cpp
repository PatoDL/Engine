#include "Game.h"
#include "Shape.h"
#include "Input.h"
#include "Sprite.h"
#include "CollisionManager.h"
#include "Model.h"
#include "Shader.h"
#include "Entity3D.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Frustum.h"
Sprite* sq;
Sprite* spr;
Shader* shad;

DirectionalLight* directionalLight;
PointLight* pointLight;
SpotLight* spotLight;

list<Light*>* lightsList;
list<Light*>* lightsToShowList;

vec3 plpos = { 0.f,0.f,0.f };



bool Game::OnStart()
{	
	render->setClearScreenColor(1.f, 0.f, 1.f,1.f);
	
	/*sq = new Sprite(render, 1, 1, 1);
	Material* sqmat = new Material();
	sqmat->LoadShaders("src/TextureVertexShader.txt", "src/TextureFragmentShader.txt");
	sq->SetMaterial(sqmat);
	sq->LoadMaterial("res/alien.jpg", false);
	sq->SetPos(5.0f, 0.0f, -10.0f);

	spr = new Sprite(render,2,1,2);
	Material* sprmat = new Material();
	sprmat->LoadShaders("src/TextureVertexShader.txt", "src/TextureFragmentShader.txt");
	spr->SetMaterial(sprmat);
	spr->LoadMaterial("res/megaman.png",true);
	spr->SetPos(-10.0f, 0.0f, -10.0f);*/

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	
	shad = new Shader("src/3DVertexShader.txt", "src/3DFragmentShader.txt");

	shad->setVec3("viewPosition", cam->GetCameraPosition());
	shad->setVec3("viewDirection", cam->GetCameraDirection());

	glm::vec3 dir = { 0.f, 0.f, -1.f };
	glm::vec3 ambient = { 0.1f, 0.1f, 0.1f };
	glm::vec3 diffuse = { 0.5f, 0.5f, 0.5f };
	glm::vec3 specular = { 1.f,1.f,1.f };

	cam->SetCameraSpeed(2.5f);
	
	m = new Model("res/Planes/PlanesZneg2.fbx", false);

	m2 = new Model("res/backpack/backpack.obj", true);

	m3 = new Model("res/backpack/backpack.obj", true);

	m4 = new Model("res/backpack/backpack.obj", true);

	movableEntity = new Model("res/backpack/backpack.obj", true);

	//m->SetRot(90, vec3(0.f, 1.f, 0.f));

	m2->SetPos(vec3(-15.f, 0.f, 0.f));

	m3->SetPos(vec3(15.f, 0.f, 0.f));

	m4->SetPos(vec3(0.f, 0.f, -15.f));

	//m3->SetPos(vec3(0.f, 0.f, -10.f));

	/*m3 = new Model("res/nave/E 45 Aircraft_obj.obj", false);

	m4 = new Model("res/nave2/Intergalactic_Spaceship-(Wavefront).obj", false);*/

	lightsList = new list<Light*>();
	
	vec3 lightPos = { 0.f,0.f,0.f };

	directionalLight = new DirectionalLight(lightPos, dir, shad, true);
	pointLight = new PointLight(plpos, dir, shad, true);
	spotLight = new SpotLight(cam->GetCameraPosition(), cam->GetCameraDirection(), shad, false);

	shad->setInt("lightsAmount", PointLight::GetPointLightCount());
	
	lightsList->push_front(directionalLight);
	lightsList->push_front(pointLight);
	lightsList->push_front(spotLight);

	for(list<Light*>::iterator iB = lightsList->begin(); iB != lightsList->end(); ++iB)
	{
		(*iB)->SetAmbient(ambient);
		(*iB)->SetDiffuse(diffuse);
		(*iB)->SetSpecular(specular);
	}
	
	return true;
}

vec2 prevPos;

float yRot = 0.0f;

float yRot2 = 0.f;

float at = 1.0f;

vec3 newscale = { 1.f,1.f,1.f };

bool Game::OnUpdate()
{
	shad->use();
	shad->setVec3("viewPosition", cam->GetCameraPosition());
	shad->setVec3("viewDirection", glm::normalize(cam->GetCameraDirection()));
	

	glm::vec3 objColor = { 1.0f,1.0f,1.0f };

	pointLight->SetAttenuation(at);
	
	//m->SetPos(plpos);

	spotLight->SetPosition(cam->GetCameraPosition());
	spotLight->SetDirection(cam->GetCameraDirection());
	shad->use();
	shad->setInt("lightsAmount", PointLight::GetPointLightCount());
	
	shad->setVec3("objectColor", objColor);

	pointLight->SetPosition(plpos);
	pointLight->SetDiffuse(vec3(1.f, 0.f, 0.f));

	for (list<Light*>::iterator iB = lightsList->begin(); iB != lightsList->end(); ++iB)
	{
		(*iB)->Update();
	}
	
	cam->UpdateCamera();
	
	//point directionalLight input
	if(Input::GetKeyPressed(GLFW_KEY_I))
	{
		plpos.z += BaseGame::GetDeltaTime() * 100.0f;
	}
	if (Input::GetKeyPressed(GLFW_KEY_K))
	{
		plpos.z -= BaseGame::GetDeltaTime() * 100.0f;
	}
	if (Input::GetKeyPressed(GLFW_KEY_J))
	{
		plpos.x -= BaseGame::GetDeltaTime() * 100.0f;
	}
	if (Input::GetKeyPressed(GLFW_KEY_L))
	{
		plpos.x += BaseGame::GetDeltaTime() * 100.0f;
	}
	
	if(Input::GetKeyPressed(GLFW_KEY_SPACE))
	{
		newscale = vec3(1.f, 1.f, 1.f) + vec3(10.f) * BaseGame::GetDeltaTime();
		m->SetScale(newscale);
	}
	
	if (Input::GetKeyPressed(GLFW_KEY_C))
	{
		newscale = vec3(1.f, 1.f, 1.f) - vec3(10.f) * BaseGame::GetDeltaTime();
		m->SetScale(newscale);
	}

	if(Input::GetKeyReleased(GLFW_KEY_F))
	{
		spotLight->SetActive(!spotLight->GetActive());
	}
	
	if (Input::GetKeyPressed(GLFW_KEY_LEFT))
	{
		yRot2 = 100.0f * BaseGame::GetDeltaTime();
		movableEntity->SetPos(vec3(yRot2, 0.f, 0.f));
	}

	if (Input::GetKeyPressed(GLFW_KEY_RIGHT))
	{
		yRot2 = -100.0f * BaseGame::GetDeltaTime();
		movableEntity->SetPos(vec3(yRot2, 0.f, 0.f));
	}

	if (Input::GetKeyPressed(GLFW_KEY_UP))
	{
		yRot2 = 100.0f * BaseGame::GetDeltaTime();
		movableEntity->SetPos(vec3(0.f, 0.f, yRot2));
	}

	if (Input::GetKeyPressed(GLFW_KEY_DOWN))
	{
		yRot2 = -100.0f * BaseGame::GetDeltaTime();
		movableEntity->SetPos(vec3(0.f, 0.f, yRot2));
	}

	if (Input::GetKeyPressedOnce(GLFW_KEY_0))
	{
		render->SetBSPEnabled(!render->GetBSPEnabled());
	}

	if (Input::GetKeyPressedOnce(GLFW_KEY_9))
	{
		render->SetFCEnabled(!render->GetFCEnabled());
	}

	if (Input::GetKeyPressed(GLFW_KEY_ESCAPE))
	{
		return false;
	}

	return true;
}

void Game::OnDraw()
{
	BaseGame::GetRootEntity()->Draw(*shad);
	cout << render->culledEntitiesAmount << endl;
	render->culledEntitiesAmount = 0;
}

bool Game::OnStop()
{
	delete spr;
	delete sq;

	delete lightsList;
	
	return true;
}