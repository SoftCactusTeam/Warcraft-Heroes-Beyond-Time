#include "Application.h"
#include "Scene.h"
#include "ModuleEntitySystem.h"
#include  "ModuleGUI.h"
#include "Label.h"
#include "InputBox.h"
#include "ModuleInput.h"

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
	LabelInfo defLabel;
	defLabel.color = Red;
	defLabel.fontName = "Arial16";
	defLabel.text = "Hey bitches im here";
	
	Application->gui->CreateLabel({0,0}, defLabel, nullptr, nullptr);
	Application->entities->AddPlayer({0,0}, THRALL);
	//LabelInfo defLabel;
	//defLabel.color = Red;
	//defLabel.fontName = "Arial16";
	//defLabel.text = "Hey bitches im here";
	//
	//Application->gui->CreateLabel({0,0}, defLabel, nullptr, nullptr);



	//InputBoxInfo defInputBox;
	//defInputBox.color = Green;
	//defInputBox.fontName = "Arial16";

	//InputBox* box = Application->gui->CreateInputBox({ 0, 200 }, defInputBox, nullptr, nullptr);
	//box->EnableInput();

	



	return true;
}

bool Scene::PreUpdate()
{
	return true;
}

bool Scene::Update(float dt)
{
	//TESTING SAVES
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		App->Save();

	}

	//TESTING LOAD
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		App->Load();
	}

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