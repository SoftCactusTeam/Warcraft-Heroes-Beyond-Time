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
#include "ModulePrinter.h"
#include "Item.h"
#include "WCItem.h"
#include "ModuleTextures.h"
#include "ModuleItems.h"
#include "FileSystem.h"
#include "BossEntity.h"
#include "Brofiler\Brofiler.h"
#include "Label.h"
#include "InputBox.h"
#include "Button.h"
#include "GUIWindow.h"
#include "Slider.h"
#include "GUIImage.h"



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

Scene::~Scene() {}

bool Scene::Awake(pugi::xml_node& sceneNode)
{
	App->audio->PlayMusic(App->audio->MainMenuBSO.data(), 0);

	return true;
}

bool Scene::Start()
{
	gratitudeON = false;
	App->gui->Activate();

	currentPercentAudio = App->audio->MusicVolumePercent;

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
			App->items->Activate();
			App->colliders->Activate();
			App->entities->Activate();
			App->console->Activate();
			App->map->Activate();
			App->printer->Activate();

			BROFILER_CATEGORY("InGame Generation", Profiler::Color::Chocolate);
			MapData mapInfo;
			mapInfo.sizeX = 50;
			mapInfo.sizeY = 50;
			mapInfo.iterations = 300;
			mapInfo.tilesetPath = "maps/Tiles.png";
			mapInfo.seed = seed;
			seed = NULL;
			lvlIndex++;

			App->map->GenerateMap(mapInfo);
			player = App->entities->AddPlayer({ 25 * 46,25 * 46}, THRALL);
			player_HP_Bar = App->gui->CreateHPBar(player, { 10,5 });

			App->path->LoadPathMap();

			iPoint enemy = App->map->GetRandomValidPoint();
			App->entities->AddEnemy({ (float)enemy.x * 46, (float)enemy.y * 46 }, ARCHER);

			enemy = App->map->GetRandomValidPoint();
			App->entities->AddEnemy({ (float)enemy.x * 46 , (float)enemy.y * 46 }, ARCHER);

			enemy = App->map->GetRandomValidPoint();
			App->entities->AddEnemy({ (float)enemy.x * 46 , (float)enemy.y * 46 }, ARCHER);

			enemy = App->map->GetRandomValidPoint();
			App->entities->AddEnemy({ (float)enemy.x * 46 , (float)enemy.y * 46 }, ARCHER);

			enemy = App->map->GetRandomValidPoint();
			App->entities->AddEnemy({ (float)enemy.x * 46 , (float)enemy.y * 46 }, ARCHER);

			enemy = App->map->GetRandomValidPoint();
			App->entities->AddEnemy({ (float)enemy.x * 46 , (float)enemy.y * 46 }, ARCHER);

			
			iPoint chestPos = App->map->GetRandomValidPointProxy(30, 5);
			lvlChest = App->entities->AddChest({ (float)chestPos.x * 46,(float)chestPos.y * 46 }, MID_CHEST);
			lvlChest->UnLockChest();
			break;
		}
		case Stages::BOSS_ROOM:
		{
			App->items->Activate();
			App->colliders->Activate();
			App->entities->Activate();
			App->console->Activate();
			App->map->Activate();
			App->printer->Activate();

			App->audio->PlayMusic(App->audio->GuldanBSO.data(), 1);
			App->map->GenerateBossMap();
			portal = (PortalEntity*)App->entities->AddStaticEntity({ 15 * 46,17 * 46, }, PORTAL);
			portal->locked = true;
			player = App->entities->AddPlayer({ 15 * 46,16 * 46, }, THRALL);
			player_HP_Bar = App->gui->CreateHPBar(player, { 10,5 });
			BossEntity* guldan = App->entities->AddBoss({ 14 * 48,5 * 48 }, BossType::GULDAN);
			App->gui->CreateBossHPBar(guldan, { 640 / 2 - 312 / 2,320 });
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

	bool ret = true;

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

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		if (player != nullptr)
			player->SetDamage(25, true);
	}

	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_REPEAT)
	{
		if (player != nullptr)
			player->IncreaseEnergy(100);
	}

	//GENERATE A NEW MAP
	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN && actual_scene == Stages::INGAME && !App->console->isWritting())
	{
		restart = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_F1) && actual_scene == Stages::INGAME)
	{
		actual_scene = Stages::BOSS_ROOM;
		restart = true;
	}

	if (actual_scene == Stages::INGAME && App->entities->enemiescount <= 0)
	{
		GeneratePortal();
	}

	if (actual_scene == Stages::MAIN_MENU && App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN) // DELETE THIS AFTER VERTICAL
	{
		App->audio->PlayMusic(App->audio->InGameBSO.data(), 1);
		actual_scene = Stages::INGAME;
		seed = 1523809027;
		restart = true;
	}

	//PAUSE GAME
	if (actual_scene == Stages::INGAME || actual_scene == Stages::BOSS_ROOM)
	{
		if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN ||
			App->input->GetPadButtonDown(SDL_CONTROLLER_BUTTON_START) == KEY_DOWN)
		{
			if (!paused)
			{
				App->audio->PauseFX();
				paused = true;
				currentPercentAudio = App->audio->MusicVolumePercent;
				uint tmpAudio = (uint)(currentPercentAudio * 0.3f);
				if (tmpAudio == 0)
					tmpAudio = 1;
				App->audio->setMusicVolume(tmpAudio);
				CreatePauseMenu();
		
			}
			else
			{
				App->audio->ResumeFX();
				paused = false;
				// Decreasing audio when pause game
				App->audio->setMusicVolume(currentPercentAudio);
				App->gui->DestroyElem(PauseMenu);
			}
		}
	}

	return ret;
}

bool Scene::PostUpdate()
{
	bool ret = true;
	if (actual_scene == Stages::MAIN_MENU || actual_scene == Stages::SETTINGS)
	{
		SDL_Rect back = { 0,0,640,360 };
		//App->render->DrawQuad(back, 0, 205, 193, 255, true, false);
		App->render->DrawQuad(back, 64, 66, 159, 255, true, false);
		

	}

	if (App->path->printWalkables == true)
		App->path->PrintWalkableTiles();

	if (actual_scene == Stages::BOSS_ROOM && gratitudeON)
	{
		
		App->render->DrawQuad({ -App->render->camera.x,-App->render->camera.y,640,360 }, 0, 0, 0, 200 , true, true);
	}

	BROFILER_CATEGORY("SceneRestart", Profiler::Color::Chocolate);
	if (restart)
	{
		restart = false;
		this->DeActivate();
		this->Activate();
	}
	
	return ret;
}

bool Scene::CleanUp()
{
	App->gui->DeActivate();
	App->map->DeActivate();
	App->entities->DeActivate();
	App->console->DeActivate();
	App->path->ClearMap();
	App->colliders->DeActivate();

	if (actual_scene == Stages::MAIN_MENU)
	{
		App->items->DeActivate();
	}	

	player = nullptr;
	lvlChest = nullptr;
	portal = nullptr;
	PauseMenu = nullptr;

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
			button->MoveChilds({ 0.0f, 1.0f });
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
				if (actual_scene == Stages::INGAME || actual_scene == Stages::BOSS_ROOM)
				{
					App->audio->PlayMusic(App->audio->MainMenuBSO.data(), 0);
					App->audio->setMusicVolume(currentPercentAudio);
					App->audio->HaltFX();
				}
				
				actual_scene = Stages::MAIN_MENU;
				paused = false;
				restart = true;
				break;
			case BType::RESUME:
				paused = false;
				App->audio->ResumeFX();
				App->gui->DestroyElem(PauseMenu);
				App->audio->setMusicVolume(currentPercentAudio);
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
	GUIWindow* window = (GUIWindow*)App->gui->CreateGUIWindow({ 0,0 }, { 0,0,0,0 }, nullptr, nullptr);

	//PLAY BUTTON
	Button* button = (Button*)App->gui->CreateButton({ 640 / 2 - 158 / 2, 50.0f }, BType::PLAY, this, window);

	/*LabelInfo defLabel;
	defLabel.color = White;
	defLabel.fontName = "LifeCraft80";
	defLabel.text = "PLAY";
	App->gui->CreateLabel(getPosByResolution({ 55,10 }), defLabel, button, this);*/

	LabelInfo defLabel;
	defLabel.color = White;
	defLabel.fontName = "LifeCraft80";
	defLabel.text = "Start Demo";
	App->gui->CreateLabel({ 33,11 }, defLabel, button, this);

	//SETTINGS BUTTON
	Button* button2 = (Button*)App->gui->CreateButton({ 640 / 2 - 158 / 2, 150.0f }, BType::SETTINGS, this, window);

	LabelInfo defLabel2;
	defLabel2.color = White;
	defLabel2.fontName = "LifeCraft80";
	defLabel2.text = "Settings";
	App->gui->CreateLabel({ 42,10 }, defLabel2, button2, this);

	//EXIT GAME BUTTON
	Button* button3 = (Button*)App->gui->CreateButton({ 640 / 2 - 158 / 2, 250.0f }, BType::EXIT_GAME, this, window);

	LabelInfo defLabel3;
	defLabel3.color = White;
	defLabel3.fontName = "LifeCraft80";
	defLabel3.text = "Quit";
	App->gui->CreateLabel({ 60,10 }, defLabel3, button3, this);

	//VERSION LABEL
	LabelInfo versionLabel;
	versionLabel.color = White;
	versionLabel.fontName = "Arial30";
	versionLabel.text = App->gui->getVersion();
	App->gui->CreateLabel({ 10,340 }, versionLabel, nullptr, nullptr);

}

void Scene::CreateSettingsScreen()
{
	GUIWindow* window = (GUIWindow*)App->gui->CreateGUIWindow({ 0,0 }, { 0,0,0,0 }, nullptr, nullptr);

	//MUSIC VOLUME SLIDER
	SliderInfo sinfo;
	sinfo.type = Slider::SliderType::MUSIC_VOLUME;
	Slider* slider = (Slider*)App->gui->CreateSlider({ 183, 95 }, sinfo, this, window);

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
	Slider* slider2 = (Slider*)App->gui->CreateSlider({ 183, 190 }, sinfo2, this, window);

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
	Button* button3 = (Button*)App->gui->CreateButton({ 640 / 2 - 158 / 2, 250.0f }, BType::GO_MMENU, this, window);

	LabelInfo defLabel2;
	defLabel2.color = White;
	defLabel2.fontName = "LifeCraft80";
	defLabel2.text = "Back";
	App->gui->CreateLabel({ 56,11 }, defLabel2, button3, this);
}

void Scene::CreatePauseMenu()
{
	fPoint localPos = { 640 / 2 - 249 / 2, 360 / 2 - 286 / 2 };
	PauseMenu = (GUIWindow*)App->gui->CreateGUIWindow(localPos, WoodWindow, this);
	PauseMenu->blackBackground = true;

	Button* Resume = (Button*)App->gui->CreateButton({ 249 / 2 - 158 / 2, 40 }, BType::RESUME, this, PauseMenu);

	LabelInfo defLabel1;
	defLabel1.color = White;
	defLabel1.fontName = "LifeCraft80";
	defLabel1.text = "Resume";
	App->gui->CreateLabel({ 46,10 }, defLabel1, Resume, this);

	Button* MainMenu = (Button*)App->gui->CreateButton({ 249 / 2 - 158 / 2, 120 }, BType::GO_MMENU, this, PauseMenu);

	LabelInfo defLabel2;
	defLabel2.color = White;
	defLabel2.fontName = "LifeCraft46";
	defLabel2.text = "Return to the Main Menu";
	App->gui->CreateLabel({ 18,15 }, defLabel2, MainMenu, this);

	Button* SaveAndExit = (Button*)App->gui->CreateButton({ 249 / 2 - 158 / 2, 200 }, BType::EXIT_GAME, this, PauseMenu);

	LabelInfo defLabel3;
	defLabel3.color = White;
	defLabel3.fontName = "LifeCraft80";
	defLabel3.text = "Save and Exit";
	App->gui->CreateLabel({ 23,10 }, defLabel3, SaveAndExit, this);
}

void Scene::AddCommands()
{
	ConsoleOrder* order = new ConsoleMap();
	App->console->AddConsoleOrderToList(order);
}

void Scene::GeneratePortal()
{
	if (portal == nullptr)
	{
		iPoint position = App->map->GetRandomValidPointProxy(20, 5);

		portal = (PortalEntity*)App->entities->AddStaticEntity({ (float)position.x * 46, (float)position.y * 46 }, PORTAL);
	}
}

void Scene::GoMainMenu()
{
	if (actual_scene == Stages::INGAME || actual_scene == Stages::BOSS_ROOM)
		App->audio->PlayMusic(App->audio->MainMenuBSO.data(), 0.5);
	actual_scene = Stages::MAIN_MENU;
	restart = true;
}

void Scene::GoBossRoom()
{
	actual_scene = Stages::BOSS_ROOM;
	restart = true;
}

void Scene::CreateGratitudeScreen()
{
	App->gui->DestroyElem(player_HP_Bar);
	gratitudeON = true;
	LabelInfo gratitude;
	gratitude.color = White;
	gratitude.fontName = "LifeCraft90";
	gratitude.multilabelWidth = 1000;
	gratitude.text = "                    Victory! \n Thanks for playing the demo. Your support means a lot ^^ \n More at: @SoftCactus_Team";
	App->gui->CreateLabel({ 160, 130 }, gratitude, nullptr, nullptr);
}
