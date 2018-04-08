#include "Application.h"
#include "Scene.h"
#include "ModuleEntitySystem.h"
#include "ModuleGUI.h"
#include "ModuleInput.h"
#include "Console.h"
#include "ModuleAudio.h"
#include "ModuleMapGenerator.h"
#include "ModuleRender.h"
#include "ModuleColliders.h"
#include "ChestEntity.h"
#include "PortalEntity.h"
#include "Pathfinding.h"
#include "PlayerEntity.h"

#include "Brofiler\Brofiler.h"

#include "Label.h"
#include "InputBox.h"
#include "Button.h"
#include "GUIWindow.h"
#include "Slider.h"



class ConsoleMap : public ConsoleOrder
{
	std::string orderName() { return "map"; }
	void Exec(std::string parametre, int parametreNumeric) {
		if (parametre == "printwalkables")
			if (parametreNumeric == 1)
				App->path->printWalkables = true;
			else if (parametreNumeric == 0)
				App->path->printWalkables = false;
	}
};


Scene::Scene()
{
	name = "scene";
}

Scene::~Scene(){}

bool Scene::Awake(pugi::xml_node& sceneNode)
{
	App->audio->PlayMusic(App->audio->MainMenuBSO.data(), 0);

	return true;
}

bool Scene::Start()
{
	App->gui->Activate();

	switch (actual_scene)
	{
		case Stages::MAIN_MENU:
		{
			CreateMainMenuScreen();

			break;
		}
		case Stages::SETTINGS:
		{
			CreateSettingsScreen();

			break;
		}
		case Stages::INGAME:
		{
			App->colliders->Activate();
			App->entities->Activate();
			App->console->Activate();
			App->map->Activate();



			BROFILER_CATEGORY("InGame Generation", Profiler::Color::Chocolate);
			MapData mapInfo;
			mapInfo.sizeX = 50;
			mapInfo.sizeY = 50;
			mapInfo.iterations = 600;
			mapInfo.tilesetPath = "maps/Tiles.png";
			lvlIndex++;

			App->map->GenerateMap(mapInfo);

			player = App->entities->AddPlayer({ 25*48,25*48 }, THRALL);
			App->gui->CreateHPBar(player, { 10,5 });

			App->path->LoadPathMap();

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

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_REPEAT)
	{
		if (player->numStats.hp > 10)
			player->numStats.hp -= 10;
		else
			player->numStats.hp = 0;
	}

	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_REPEAT)
	{
		if (player->numStats.energy > 10)
			player->numStats.energy -= 10;
		else
			player->numStats.energy = 0;
	}



//GENERATE A NEW MAP
	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN && actual_scene == Stages::INGAME && lvlIndex != 8 && !App->console->isWritting())
	{
		restart = true;
	}
	else if(App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN && actual_scene == Stages::INGAME && lvlIndex == 8 && !App->console->isWritting())
	{
		lvlIndex = 0;
		actual_scene = Stages::MAIN_MENU;
		restart = true;
		// RESTART THIS MODULE AND THE ENTIRE GAME // GO TO MAIN MENU
	}

	//CONTROLLER RUMBLES
	if (App->input->GetPadButtonDown(SDL_CONTROLLER_BUTTON_A) == KEY_DOWN)
	{
		App->input->PlayJoyRumble(0.75f, 100);
	}

	if (App->input->GetKey(SDL_SCANCODE_9) == KEY_DOWN)
	{
		lvlChest->UnLockChest();
		lvlChest->OpenChest();
		portal->OpenPortal();
	}

	if (App->input->GetKey(SDL_SCANCODE_8) == KEY_DOWN)
		player->SetDamage(25, true);

	//PAUSE GAME
		if (actual_scene == Stages::INGAME)
		{
			if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN ||
				App->input->GetPadButtonDown(SDL_CONTROLLER_BUTTON_START) == KEY_DOWN) //Start button, idk
			{
				if (!paused)
				{
					paused = true;
					CreatePauseMenu();
				}
				else
				{
					paused = false;
					App->gui->DestroyElem(PauseMenu);
				}
			}
		}
	return true;
}

bool Scene::PostUpdate()
{
	// TODO -- fer que es pintin els walkables
	if (App->path->printWalkables == true)
		App->path->PrintWalkableTiles();

	BROFILER_CATEGORY("SceneRestart", Profiler::Color::Chocolate);
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
	App->map->DeActivate();
	App->entities->DeActivate();
	App->console->DeActivate();
	App->path->ClearMap();
	App->colliders->DeActivate();


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
				{
					App->audio->PlayFx(App->audio->ButtonHovered);
					button->atlasRect = Button1MouseHover;
					break;
				}
				case UIEvents::MOUSE_RIGHT_UP:
				{
					button->atlasRect = Button1MouseHover;
					break;
				}
				case UIEvents::MOUSE_LEFT_CLICK:
				{
					App->audio->PlayFx(App->audio->ButtonClicked);
					button->atlasRect = Button1Pressed;
					button->MoveChilds({0.0f, 1.0f});
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
					button->MoveChilds({ 0.0f, -1.0f });
					switch (button->btype)
					{
					case BType::PLAY:
						App->audio->PlayMusic(App->audio->InGameBSO.data(), 1);
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
						if (actual_scene == Stages::INGAME)
							App->audio->PlayMusic(App->audio->MainMenuBSO.data(), 0);
						actual_scene = Stages::MAIN_MENU;
						paused = false;
						restart = true;
						break;
					case BType::RESUME:
						paused = false;
						App->gui->DestroyElem(PauseMenu);
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

void Scene::CreateMainMenuScreen()
{

	//PLAY BUTTON
	Button* button = (Button*)App->gui->CreateButton(getPosByResolution({ 250, 50.0f }), BType::PLAY, this);

	/*LabelInfo defLabel;
	defLabel.color = White;
	defLabel.fontName = "LifeCraft80";
	defLabel.text = "PLAY";
	App->gui->CreateLabel(getPosByResolution({ 55,10 }), defLabel, button, this);*/

	LabelInfo defLabel;
	defLabel.color = White;
	defLabel.fontName = "LifeCraft80";
	defLabel.text = "Start Demo";
	App->gui->CreateLabel(getPosByResolution({ 33,11 }), defLabel, button, this);

	//SETTINGS BUTTON
	Button* button2 = (Button*)App->gui->CreateButton(getPosByResolution({ 250, 150.0f }), BType::SETTINGS, this);

	LabelInfo defLabel2;
	defLabel2.color = White;
	defLabel2.fontName = "LifeCraft80";
	defLabel2.text = "Settings";
	App->gui->CreateLabel(getPosByResolution({ 42,10 }), defLabel2, button2, this);

	//EXIT GAME BUTTON
	Button* button3 = (Button*)App->gui->CreateButton(getPosByResolution({ 250, 250.0f }), BType::EXIT_GAME, this);

	LabelInfo defLabel3;
	defLabel3.color = White;
	defLabel3.fontName = "LifeCraft80";
	defLabel3.text = "Quit";
	App->gui->CreateLabel(getPosByResolution({ 60,10 }), defLabel3, button3, this);
}

void Scene::CreateSettingsScreen()
{
	//MUSIC VOLUME SLIDER
	SliderInfo sinfo;
	sinfo.type = Slider::SliderType::MUSIC_VOLUME;
	Slider* slider = (Slider*)App->gui->CreateSlider(getPosByResolution({ 185, 95 }), sinfo, this, nullptr);

	LabelInfo defLabel3;
	defLabel3.color = White;
	defLabel3.fontName = "Arial80";
	std::string temp = (char*)std::to_string(App->audio->MusicVolumePercent).data();
	defLabel3.text = (char*)temp.data();
	App->gui->CreateLabel({ 265,-4 }, defLabel3, slider, this);

	LabelInfo defLabel;
	defLabel.color = Black;
	defLabel.fontName = "LifeCraft90";
	defLabel.text = "Music Volume";
	App->gui->CreateLabel({ 0,-35 }, defLabel, slider, this);

	//FX VOLUME SLIDER
	SliderInfo sinfo2;
	sinfo2.type = Slider::SliderType::FX_VOLUME;
	Slider* slider2 = (Slider*)App->gui->CreateSlider(getPosByResolution({ 185, 190 }), sinfo2, this, nullptr);

	LabelInfo defLabel4;
	defLabel4.color = White;
	defLabel4.fontName = "Arial80";
	std::string temp2 = (char*)std::to_string(App->audio->FXVolumePercent).data();
	defLabel4.text = (char*)temp2.data();
	App->gui->CreateLabel({ 265,-4 }, defLabel4, slider2, this);

	LabelInfo defLabel5;
	defLabel5.color = Black;
	defLabel5.fontName = "LifeCraft90";
	defLabel5.text = "FX Volume";
	App->gui->CreateLabel({ 0,-35 }, defLabel5, slider2, this);

	//BACK BUTTON
	Button* button3 = (Button*)App->gui->CreateButton(getPosByResolution({ 240, 250.0f }), BType::GO_MMENU, this);

	LabelInfo defLabel2;
	defLabel2.color = White;
	defLabel2.fontName = "LifeCraft80";
	defLabel2.text = "Back";
	App->gui->CreateLabel(getPosByResolution({ 56,11 }), defLabel2, button3, this);
}

void Scene::CreatePauseMenu()
{
	fPoint localPos = getPosByResolution({ 640 / 2 - 249 / 2, 360 / 2 - 286 / 2 });
	PauseMenu = (GUIWindow*)App->gui->CreateGUIWindow(localPos, WoodWindow, this);

	Button* Resume = (Button*)App->gui->CreateButton(getPosByResolution({ 249 / 2 - 158 / 2, 40 }), BType::RESUME, this, PauseMenu);

	LabelInfo defLabel1;
	defLabel1.color = White;
	defLabel1.fontName = "LifeCraft80";
	defLabel1.text = "Resume";
	App->gui->CreateLabel(getPosByResolution({ 46,10 }), defLabel1, Resume, this);

	Button* MainMenu = (Button*)App->gui->CreateButton(getPosByResolution({ 249 / 2 - 158 / 2, 120 }), BType::GO_MMENU, this, PauseMenu);

	LabelInfo defLabel2;
	defLabel2.color = White;
	defLabel2.fontName = "LifeCraft46";
	defLabel2.text = "Return to the Main Menu";
	App->gui->CreateLabel(getPosByResolution({ 18,15 }), defLabel2, MainMenu, this);

	Button* SaveAndExit = (Button*)App->gui->CreateButton(getPosByResolution({ 249 / 2 - 158 / 2, 200 }), BType::EXIT_GAME, this, PauseMenu);

	LabelInfo defLabel3;
	defLabel3.color = White;
	defLabel3.fontName = "LifeCraft80";
	defLabel3.text = "Save and Exit";
	App->gui->CreateLabel(getPosByResolution({ 23,10 }), defLabel3, SaveAndExit, this);
}

fPoint Scene::getPosByResolution(fPoint pos) const
{
	uint actualW = App->render->camera.w;
	uint actualH = App->render->camera.h;

	float percentX = (pos.x * 100) / 640;
	float percentY = (pos.y * 100) / 360;


	fPoint ret = { (actualW * percentX)/100, (actualH * percentY)/100};

	return ret;
}

void Scene::AddCommands()
{
	ConsoleOrder* order = new ConsoleMap();
	App->console->AddConsoleOrderToList(order);
}
