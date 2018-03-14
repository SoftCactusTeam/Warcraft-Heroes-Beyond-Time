#include "Application.h"
#include "Scene.h"
#include "ModuleEntitySystem.h"
#include  "ModuleGUI.h"
#include "Label.h"
#include "InputBox.h"
#include "ModuleInput.h"
#include "Button.h"

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
	/*LabelInfo defLabel;
	defLabel.color = Red;
	defLabel.fontName = "Arial16";
	defLabel.text = "Hey bitches im here";

	App->gui->CreateLabel({0,0}, defLabel, nullptr, nullptr);*/


	PlayerEntity* player = App->entities->AddPlayer({ 50,50 }, THRALL);
	App->entities->SetPlayer(player);

	Button* button = (Button*)App->gui->CreateButton({20.0f, 20.0f }, this);

	LabelInfo defLabel;
	defLabel.color = Red;
	defLabel.fontName = "Arial11";
	defLabel.text = "I'm a button";
	App->gui->CreateLabel({5,5}, defLabel, button, this);



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
	switch (UIelem->type)
	{
		case GUIElem::GUIElemType::BUTTON:
		{
			Button* button = (Button*)UIelem;
			switch (_event)
			{
				case UIEvents::MOUSE_ENTER:
				case UIEvents::MOUSE_RIGHT_UP:
				case UIEvents::MOUSE_LEFT_UP:
				{
					button->atlasRect = Button1MouseHover;
					break;
				}
				case UIEvents::MOUSE_LEFT_CLICK:
				{
					button->atlasRect = Button1Pressed;
					break;
				}
				case UIEvents::MOUSE_LEAVE:
				case UIEvents::NO_EVENT:
				{
					button->atlasRect = Button1;
					break;
				}
			}
			break;
		}
	}
}