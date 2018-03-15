#include "Application.h"
#include "Scene.h"
#include "ModuleEntitySystem.h"
#include  "ModuleGUI.h"
#include "Label.h"
#include "InputBox.h"
#include "ModuleInput.h"
#include "Button.h"
#include "Console.h"
#include "Slider.h"
#include "ModuleAudio.h"
#include "ModuleMapGenerator.h"
#include "ModuleRender.h"


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
	
	App->gui->CreateLabel({0,0}, defLabel, nullptr, nullptr);
	App->entities->AddPlayer({0,0}, THRALL);

	App->map->GenerateGrid(50,50);

	//LabelInfo defLabel;
	//defLabel.color = Red;
	//defLabel.fontName = "Arial16";
	//defLabel.text = "Hey bitches im here";
	//
	//Application->gui->CreateLabel({0,0}, defLabel, nullptr, nullptr);


	App->gui->CreateLabel({0,0}, defLabel, nullptr, nullptr);*/

	App->gui->Activate();
	switch (actual_scene)
	{
		case Stages::MAIN_MENU:
		{
			Button* button = (Button*)App->gui->CreateButton({ 300, 50.0f }, BType::PLAY, this);

			LabelInfo defLabel;
			defLabel.color = Red;
			defLabel.fontName = "Arial11";
			defLabel.text = "PLAY";
			App->gui->CreateLabel({ 100,40 }, defLabel, button, this);

			Button* button2 = (Button*)App->gui->CreateButton({ 300, 150.0f }, BType::SETTINGS, this);

			LabelInfo defLabel2;
			defLabel2.color = Red;
			defLabel2.fontName = "Arial11";
			defLabel2.text = "Settings";
			App->gui->CreateLabel({ 90,40 }, defLabel2, button2, this);

			Button* button3 = (Button*)App->gui->CreateButton({ 300, 250.0f }, BType::EXIT_GAME, this);

			LabelInfo defLabel3;
			defLabel3.color = Red;
			defLabel3.fontName = "Arial11";
			defLabel3.text = "Fuck u go fucking out of here ;(";
			App->gui->CreateLabel({ 40,40 }, defLabel3, button3, this);

			break;
		}
		case Stages::SETTINGS:
		{
			SliderInfo sinfo;
			sinfo.type = Slider::SliderType::MUSIC_VOLUME;
			Slider* slider = (Slider*)App->gui->CreateSlider({ 250, 190 }, sinfo, this, nullptr);

			LabelInfo defLabel3;
			defLabel3.color = White;
			defLabel3.fontName = "Arial11";
			std::string temp = (char*)std::to_string(App->audio->MusicVolumePercent).data();
			defLabel3.text = (char*)temp.data();
			App->gui->CreateLabel({ 380,7 }, defLabel3, slider, this);


			Button* button3 = (Button*)App->gui->CreateButton({ 300, 250.0f }, BType::GO_MMENU, this);

			LabelInfo defLabel2;
			defLabel2.color = Red;
			defLabel2.fontName = "Arial11";
			defLabel2.text = "Go Back ;)";
			App->gui->CreateLabel({ 40,40 }, defLabel2, button3, this);

			break;
		}
		case Stages::INGAME:
		{
			App->entities->Activate();
			App->console->Activate();
			PlayerEntity* player = App->entities->AddPlayer({ 50,50 }, THRALL);
			App->entities->SetPlayer(player);
			break;
		}
			
	}
	

	
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


	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		App->render->camera.y += 20;
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		App->render->camera.x += 20;
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		App->render->camera.y -= 20;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		App->render->camera.x -= 20;
	}

	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
	{
		App->map->CleanMap();
		App->map->GenerateGrid(50, 50);
	}

	return true;
}

bool Scene::PostUpdate()
{
	if (restart)
	{
		restart = false;
		this->DeActivate();
		this->Activate();
	}
	return true;
}

bool Scene::CleanUp()
{
	App->gui->DeActivate();
	App->entities->DeActivate();
	App->console->DeActivate();
	return true;
}

//-----------------------------------
bool Scene::OnUIEvent(GUIElem* UIelem, UIEvents _event)
{
	bool ret = true;
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
					switch (button->btype)
					{
					case BType::PLAY:
						actual_scene = Stages::INGAME;
						restart = true;
						break;
					case BType::EXIT_GAME:
						return false;
						break;
					case BType::SETTINGS:
						actual_scene = Stages::SETTINGS;
						restart = true;
						break;
					case BType::GO_MMENU:
						actual_scene = Stages::MAIN_MENU;
						restart = true;
						break;

					}
					break;
				}
			}
			break;
		}
	}
	return ret;
}