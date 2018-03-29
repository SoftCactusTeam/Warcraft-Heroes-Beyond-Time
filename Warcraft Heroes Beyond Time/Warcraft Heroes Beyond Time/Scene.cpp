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
	App->gui->Activate();
	switch (actual_scene)
	{
		case Stages::MAIN_MENU:
		{
			Button* button = (Button*)App->gui->CreateButton({ 250, 50.0f }, BType::PLAY, this);

			LabelInfo defLabel;
			defLabel.color = Red;
			defLabel.fontName = "Arial11";
			defLabel.text = "PLAY";
			App->gui->CreateLabel({ 65,25 }, defLabel, button, this);

			Button* button2 = (Button*)App->gui->CreateButton({ 250, 150.0f }, BType::SETTINGS, this);

			LabelInfo defLabel2;
			defLabel2.color = Red;
			defLabel2.fontName = "Arial11";
			defLabel2.text = "Settings";
			App->gui->CreateLabel({ 60,25 }, defLabel2, button2, this);

			Button* button3 = (Button*)App->gui->CreateButton({ 250, 250.0f }, BType::EXIT_GAME, this);

			LabelInfo defLabel3;
			defLabel3.color = Red;
			defLabel3.fontName = "Arial9";
			defLabel3.text = "Fuck u go fucking out of here ;(";
			App->gui->CreateLabel({ 13,25 }, defLabel3, button3, this);

			break;
		}
		case Stages::SETTINGS:
		{
			SliderInfo sinfo;
			sinfo.type = Slider::SliderType::MUSIC_VOLUME;
			Slider* slider = (Slider*)App->gui->CreateSlider({ 200, 190 }, sinfo, this, nullptr);

			LabelInfo defLabel3;
			defLabel3.color = White;
			defLabel3.fontName = "Arial11";
			std::string temp = (char*)std::to_string(App->audio->MusicVolumePercent).data();
			defLabel3.text = (char*)temp.data();
			App->gui->CreateLabel({ 270,3 }, defLabel3, slider, this);


			Button* button3 = (Button*)App->gui->CreateButton({ 250, 250.0f }, BType::GO_MMENU, this);

			LabelInfo defLabel2;
			defLabel2.color = Red;
			defLabel2.fontName = "Arial11";
			defLabel2.text = "Go Back ;)";
			App->gui->CreateLabel({ 50,25 }, defLabel2, button3, this);

			break;
		}
		case Stages::INGAME:
		{
			App->entities->Activate();
			App->console->Activate();

			MapData mapInfo;
			mapInfo.sizeX = 50;
			mapInfo.sizeY = 50;
			mapInfo.iterations = 600;
			mapInfo.tilesetPath = "Tiles.png";
			lvlIndex = 1;

			App->map->GenerateMap(mapInfo);
			player = App->entities->AddPlayer({ 25*48,25*48 }, THRALL);
			App->entities->AddEnemy({ 80,80 }, FOOTMAN);

			iPoint chestPos = App->map->GetRandomValidPoint();
			lvlChest = App->entities->AddChest({ (float)chestPos.x * 48,(float)chestPos.y * 48 }, MID_CHEST);
			portal = (PortalEntity*)App->entities->AddStaticEntity({ 25 * 48,25 * 48 }, PORTAL);
			break;
		}
			
	}
	
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

	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN && actual_scene == Stages::INGAME && lvlIndex != 8)
	{
		App->map->CleanUp();

		MapData mapInfo;
		mapInfo.sizeX = 50;
		mapInfo.sizeY = 50;
		mapInfo.iterations = 600;
		mapInfo.tilesetPath = "Tiles.png";

		App->map->GenerateMap(mapInfo);

		App->entities->ClearEntitiesList();
		player = App->entities->AddPlayer({ 25 * 48,25 * 48 }, THRALL);
		iPoint chestPos = App->map->GetRandomValidPoint();
		lvlChest = App->entities->AddChest({ (float)chestPos.x * 48,(float)chestPos.y * 48 }, MID_CHEST);
		portal = (PortalEntity*)App->entities->AddStaticEntity({ 25 * 48,25 * 48 }, PORTAL);
		
		lvlIndex++;
	}
	else if(App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
	{
		App->entities->ClearEntitiesList();
		App->map->CleanUp();
		lvlIndex = 0;
		// RESTART THIS MODULE AND THE ENTIRE GAME // GO TO MAIN MENU
	}

	if (App->input->GetPadButtonDown(SDL_CONTROLLER_BUTTON_A) == KEY_DOWN)
	{
		App->input->PlayJoyRumble(0.75f, 100);
	}

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		App->render->camera.y += 10;
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		App->render->camera.x += 10;
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		App->render->camera.y -= 10;
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		App->render->camera.x -= 10;
	}

	if (App->input->GetKey(SDL_SCANCODE_9) == KEY_DOWN)
	{
		lvlChest->UnLockChest();
		lvlChest->OpenChest();
		portal->OpenPortal();
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
