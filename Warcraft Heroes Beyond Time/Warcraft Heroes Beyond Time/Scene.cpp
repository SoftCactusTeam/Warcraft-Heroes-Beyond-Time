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

	Button* button = (Button*)App->gui->CreateButton({300, 50.0f }, this);

	LabelInfo defLabel;
	defLabel.color = Red;
	defLabel.fontName = "Arial11";
	defLabel.text = "I'm a button";
	App->gui->CreateLabel({85,40}, defLabel, button, this);

	Button* button2 = (Button*)App->gui->CreateButton({ 300, 150.0f }, this);

	LabelInfo defLabel2;
	defLabel2.color = Red;
	defLabel2.fontName = "Arial11";
	defLabel2.text = "I'm a button too";
	App->gui->CreateLabel({ 80,40 }, defLabel2, button2, this);

	Button* button3 = (Button*)App->gui->CreateButton({ 300, 250.0f }, this);

	LabelInfo defLabel3;
	defLabel3.color = Red;
	defLabel3.fontName = "Arial11";
	defLabel3.text = "Fuck u I'm the best :(";
	App->gui->CreateLabel({ 65,40 }, defLabel3, button3, this);


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
				{
					button->atlasRect = Button1MouseHover;
					break;
				}
				case UIEvents::MOUSE_LEFT_CLICK:
				{
					button->atlasRect = Button1Pressed;
					button->MoveChilds({0.0f, 4.0f});
					break;
				}
				case UIEvents::MOUSE_LEAVE:
				case UIEvents::NO_EVENT:
				{
					button->atlasRect = Button1;
					break;
				}
				case UIEvents::MOUSE_LEFT_UP:
				{
					button->atlasRect = Button1MouseHover;
					button->MoveChilds({ 0.0f, -4.0f });
					break;
				}
			}
			break;
		}
	}
}