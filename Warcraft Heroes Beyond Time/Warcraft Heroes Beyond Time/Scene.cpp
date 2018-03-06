#include "App.h"
#include "Scene.h"

Scene::Scene()
{
	name = "scene";
}
Scene::~Scene(){}

bool Scene::Awake()
{
	return true;
}

bool Scene::Start()
{
	return true;
}

bool Scene::PreUpdate()
{
	return true;
}

bool Scene::Update(float dt)
{
	return true;
}

bool Scene::PostUpdate()
{
	return true;
}

bool Scene::CleanUp()
{
	return true;
}

//-----------------------------------
void Scene::OnUIEvent(GUIElem* UIelem, UIEvents _event)
{

}