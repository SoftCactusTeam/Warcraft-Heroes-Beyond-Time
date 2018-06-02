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
#include "ModuleEffects.h"
#include "ModuleProjectiles.h"
#include "Guldan.h"
#include "ModuleTransitions.h"
#include "ModuleVideo.h"
#include "ParticleSystem.h"
#include "ModuleGUI.h"


#include "Brofiler\Brofiler.h"
#include "Label.h"
#include "InputBox.h"
#include "Button.h"
#include "GUIWindow.h"
#include "Slider.h"
#include "GUIImage.h"
#include "ItemContainer.h"
#include <string>





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

	// Title animation
	titleAnim.PushBack({ 0, 0, 16, 16 });
	titleAnim.PushBack({ 0, 0, 16, 16 });
	titleAnim.PushBack({ 0, 0, 16, 16 });
	titleAnim.PushBack({ 0, 0, 16, 16 });
	titleAnim.PushBack({ 0, 0, 16, 16 });
	titleAnim.PushBack({ 0, 0, 16, 16 });
	titleAnim.PushBack({ 0, 0, 16, 16 });
	titleAnim.PushBack({ 0, 0, 16, 16 });
	titleAnim.PushBack({ 0, 0, 16, 16 });
	titleAnim.PushBack({ 0, 0, 16, 16 });
	titleAnim.PushBack({ 0, 0, 16, 16 });
	titleAnim.PushBack({ 0, 0, 16, 16 });
	titleAnim.PushBack({ 0, 0, 16, 16 });
	titleAnim.PushBack({ 0, 0, 16, 16 });
	titleAnim.PushBack({ 0, 0, 16, 16 });
	titleAnim.PushBack({ 0, 0, 16, 16 });
	titleAnim.PushBack({ 0, 0, 16, 16 });
	titleAnim.PushBack({ 0, 0, 16, 16 });
	titleAnim.PushBack({ 0, 0, 16, 16 });
	titleAnim.PushBack({ 0, 0, 16, 16 });
	titleAnim.PushBack({ 0, 0, 16, 16 });
	titleAnim.PushBack({ 624, 21, 448, 129 });
	titleAnim.PushBack({ 0, 0, 16, 16 });
	titleAnim.PushBack({ 624, 21, 448, 129 });
	titleAnim.PushBack({ 0, 0, 16, 16 });
	titleAnim.PushBack({ 624, 21, 448, 129 });
	titleAnim.PushBack({ 624, 21, 448, 129 });
	titleAnim.PushBack({ 624, 21, 448, 129 });
	titleAnim.PushBack({ 624, 21, 448, 129 });
	titleAnim.PushBack({ 624, 21, 448, 129 });
	titleAnim.PushBack({ 624, 21, 448, 129 });
	titleAnim.PushBack({ 624, 21, 448, 129 });
	titleAnim.speed = 0.2f;
	titleAnim.loop = false;

	// Shadow thrall animation
	thrallShadowAnim.PushBack({ 6, 363, 214, 307 });
	thrallShadowAnim.PushBack({ 224, 363, 214, 307 });
	thrallShadowAnim.PushBack({ 445, 363, 214, 307 });
	thrallShadowAnim.PushBack({ 667, 363, 214, 307 });
	thrallShadowAnim.PushBack({ 6, 363, 214, 307 });
	thrallShadowAnim.PushBack({ 224, 363, 214, 307 });
	thrallShadowAnim.PushBack({ 445, 363, 214, 307 });
	thrallShadowAnim.PushBack({ 667, 363, 214, 307 });
	thrallShadowAnim.PushBack({ 6, 363, 214, 307 });
	thrallShadowAnim.PushBack({ 224, 363, 214, 307 });
	thrallShadowAnim.PushBack({ 445, 363, 214, 307 });
	thrallShadowAnim.PushBack({ 667, 363, 214, 307 });
	thrallShadowAnim.PushBack({ 6, 363, 214, 307 });
	thrallShadowAnim.PushBack({ 224, 363, 214, 307 });
	thrallShadowAnim.PushBack({ 445, 363, 214, 307 });
	thrallShadowAnim.PushBack({ 667, 363, 214, 307 });

	thrallShadowAnim.PushBack({ 6, 699, 214, 307 });
	thrallShadowAnim.PushBack({ 224, 699, 214, 307 });
	thrallShadowAnim.PushBack({ 445, 699, 214, 307 });
	thrallShadowAnim.PushBack({ 667, 699, 214, 307 });
	thrallShadowAnim.PushBack({ 6, 1019, 214, 307 });
	thrallShadowAnim.PushBack({ 224, 1019, 214, 307 });
	thrallShadowAnim.PushBack({ 445, 1019, 214, 307 });
	thrallShadowAnim.PushBack({ 667, 1019, 214, 307 });

	thrallShadowAnim.PushBack({ 6, 363, 214, 307 });
	thrallShadowAnim.PushBack({ 224, 363, 214, 307 });
	thrallShadowAnim.PushBack({ 445, 363, 214, 307 });
	thrallShadowAnim.PushBack({ 667, 363, 214, 307 });
	thrallShadowAnim.PushBack({ 6, 363, 214, 307 });
	thrallShadowAnim.PushBack({ 224, 363, 214, 307 });
	thrallShadowAnim.PushBack({ 445, 363, 214, 307 });
	thrallShadowAnim.PushBack({ 667, 363, 214, 307 });

	thrallShadowAnim.speed = 0.2f;
	thrallShadowAnim.loop = true;

	// Bolt animation
	boltAnim.PushBack({ 640, 0, 157, 241 });
	boltAnim.PushBack({ 640, 0, 157, 241 });
	boltAnim.PushBack({ 640, 0, 157, 241 });
	boltAnim.PushBack({ 640, 0, 157, 241 });
	boltAnim.PushBack({ 640, 0, 157, 241 });
	boltAnim.PushBack({ 640, 0, 157, 241 });
	boltAnim.PushBack({ 640, 0, 157, 241 });
	boltAnim.PushBack({ 640, 0, 157, 241 });
	boltAnim.PushBack({ 640, 0, 157, 241 });
	boltAnim.PushBack({ 640, 0, 157, 241 });
	boltAnim.PushBack({ 640, 0, 157, 241 });
	boltAnim.PushBack({ 640, 0, 157, 241 });
	boltAnim.PushBack({ 640, 0, 157, 241 });
	boltAnim.PushBack({ 640, 0, 157, 241 });
	boltAnim.PushBack({ 640, 0, 157, 241 });
	boltAnim.PushBack({ 640, 0, 157, 241 });
	boltAnim.PushBack({ 640, 0, 157, 241 });
	boltAnim.PushBack({ 640, 0, 157, 241 });
	boltAnim.PushBack({ 640, 0, 157, 241 });
	boltAnim.PushBack({ 640, 0, 157, 241 });
	boltAnim.PushBack({ 640, 0, 157, 241 });

	boltAnim.PushBack({ 14, 1344, 157, 241 });
	boltAnim.PushBack({ 201, 1344, 157, 241 });
	boltAnim.PushBack({ 640, 0, 157, 241 });
	boltAnim.PushBack({ 395, 1344, 157, 241 });
	boltAnim.PushBack({ 640, 0, 157, 241 });
	boltAnim.PushBack({ 600, 1344, 157, 241 });
	boltAnim.PushBack({ 798, 1344, 157, 241 });
	boltAnim.PushBack({ 997, 1344, 157, 241 });
	boltAnim.PushBack({ 1170, 1344, 157, 241 });
	boltAnim.PushBack({ 1363, 1344, 157, 241 });
	boltAnim.PushBack({ 1523, 1344, 157, 241 });

	boltAnim.speed = 0.2f;
	boltAnim.loop = true;
}

Scene::~Scene() {}

bool Scene::Awake(pugi::xml_node& sceneNode)
{
	
	return true;
}

bool Scene::Start()
{
	gratitudeON = false;
	restart = false;
	App->gui->Activate();

	currentPercentAudio = App->audio->MusicVolumePercent;

	//SetScene(next_scene);

	switch (actual_scene)
	{
		case Stages::INTRO_VIDEO:
		{
			App->video->Activate();
			/*next_scene = Stages::MAIN_MENU;
			restart = true;*/

			break;
		}
		case Stages::MAIN_MENU:
		{
			App->audio->PlayMusic(App->audio->MainMenuBSO.data(), 0);
			thrallShadowAnim.Reset();
			boltAnim.Reset();
			titleAnim.Reset();

			// Loading BG texture
		    textureBG = App->textures->Load("GUI/bg_menu.png");
			bgEmitter1 = App->psystem->AddEmiter({ 700.0f, 200.0f }, EmitterType::EMITTER_TYPE_PIXEL_SMOKE, -3, true);
			bgEmitter2 = App->psystem->AddEmiter({ -150.0f, 200.0f }, EmitterType::EMITTER_TYPE_PIXEL_SMOKE, -3, true);

			//VERSION LABEL
			LabelInfo versionLabel;
			versionLabel.color = White;
			versionLabel.fontName = "Arial30";
			versionLabel.text = App->gui->getVersion();
			App->gui->CreateLabel({ 10,340 }, versionLabel);

			if (menuAnimOff)
			{
				CreateMainMenuScreen();
				menuAnimOn = true;
			}
			

			lvlIndex = 0;

			break;
		}
		case Stages::SETTINGS:
		{
			thrallShadowAnim.Reset();
			boltAnim.Reset();
			textureBG = App->textures->Load("GUI/bg_menu.png");
			bgEmitter1 = App->psystem->AddEmiter({ 700.0f, 200.0f }, EmitterType::EMITTER_TYPE_PIXEL_SMOKE, -3, true);
			bgEmitter2 = App->psystem->AddEmiter({ -150.0f, 200.0f }, EmitterType::EMITTER_TYPE_PIXEL_SMOKE, -3, true);
			CreateSettingsScreen();

			break;
		}
		case Stages::INGAME:
		{
			BROFILER_CATEGORY("InGame Generation", Profiler::Color::Chocolate);
			int result = App->map->UseYourPowerToGenerateMeThisNewMap(lvlIndex);

			if (result == -1)
				return false;
			else if (result == 0)
			{
				App->items->Activate();
				App->colliders->Activate();
				App->entities->Activate();
				App->console->Activate();
				App->map->Activate();
				App->printer->Activate();
				App->projectiles->Activate();

				App->audio->PlayMusic(App->audio->GuldanBSO.data(), 1);

				portal = (PortalEntity*)App->entities->AddStaticEntity({ 15 * 46,17 * 46, }, PORTAL);
				portal->locked = true;
				player = App->entities->AddPlayer({ 15 * 46 + 10,16 * 46}, THRALL, playerStats);
				player_HP_Bar = App->gui->CreateHPBar(player, { 10,5 });
				guldan = (Guldan*)App->entities->AddBoss(GULDAN_BASE, BossType::GULDAN);
			}
			else
			{
				App->effects->Activate();
				App->colliders->Activate();
				App->entities->Activate();
				App->console->Activate();
				App->map->Activate();
				App->printer->Activate();
				App->projectiles->Activate();

				//Create lvlIndex Label
				LabelInfo lvlInfo;
				lvlInfo.color = White;
				lvlInfo.fontName = "LifeCraft80";
				lvlInfo.multilabelWidth = 700;
				std::string temp = std::string("Lvl: ") + std::to_string(lvlIndex + 1);
				lvlInfo.text = (char*)temp.data();
				Label* lvlLabel = (Label*)App->gui->CreateLabel({580, 20}, lvlInfo, nullptr, nullptr);

				//Player
				if(!player)
					player = App->entities->AddPlayer({ (float)App->map->begginingNode->pos.x * 46, (float)App->map->begginingNode->pos.y * 46 }, THRALL, playerStats);
				
				//Dash Particles
				if (testEmitter == nullptr)
				{
					testEmitter = App->psystem->AddEmiter({(player->pos.x - App->render->camera.x) / App->winScale, (player->pos.y - App->render->camera.y) / App->winScale }, EMITTER_TYPE_DASH, -1);
					testEmitter->StopEmission();
				}

				//Player HP Bar
				player_HP_Bar = App->gui->CreateHPBar(player, { 10,5 });

				App->path->LoadPathMap();

				//Archers Generation
				std::list<SDL_Rect>::iterator it = App->map->archers.begin();
				std::advance(it, lvlIndex);

				int numberArchers = 0;
				do
				{
					int randomNumber = rand() % (100 - 1 + 1) + 1;
					if (randomNumber <= (*it).y)
					{
						iPoint enemyPos = App->map->GetRandomValidPoint();
						App->entities->AddEnemy({ (float)enemyPos.x * 46 ,(float)enemyPos.y * 46 }, ENEMY_TYPE::ARCHER_TIER_1);
						numberArchers++;
						if (numberArchers >= (*it).x)
							continue;
					}

					randomNumber = rand() % (100 - 1 + 1) + 1;
					if (randomNumber <= (*it).w)
					{
						iPoint enemyPos = App->map->GetRandomValidPoint();
						App->entities->AddEnemy({ (float)enemyPos.x * 46 ,(float)enemyPos.y * 46 }, ENEMY_TYPE::ARCHER_TIER_2);
						numberArchers++;
						if (numberArchers >= (*it).x)
							continue;
					}

					randomNumber = rand() % (100 - 1 + 1) + 1;
					if (randomNumber <= (*it).h)
					{
						iPoint enemyPos = App->map->GetRandomValidPoint();
						App->entities->AddEnemy({ (float)enemyPos.x * 46 ,(float)enemyPos.y * 46 }, ENEMY_TYPE::ARCHER_TIER_3);
						numberArchers++;
						if (numberArchers >= (*it).x)
							continue;
					}
				}
				while (numberArchers < (*it).x);

				//Chest Spawn
				App->items->Activate();
				if (!App->items->isPoolEmpty())
					lvlChest = App->entities->AddChest({ (float)App->map->chestNode->pos.x * 46 + 5, (float)App->map->chestNode->pos.y * 46 }, MID_CHEST);
				else
					lvlChest = nullptr;
			}

			//Saving each new lvl
			if (!App->transitions->loadTransition)
				App->Save();
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

	if (testEmitter != nullptr)
	{
		testEmitter->MoveEmitter({ ((player->pos.x + App->render->camera.x) / App->winScale) + 5, ((player->pos.y + App->render->camera.y) / App->winScale) + 15 });

		if (actual_scene == Stages::INGAME && player->state == PlayerEntity::states::PL_DASH)
		{
			testEmitter->StartEmission(100);
		}
	}


	//CHECKING IF INTROVIDEO HAS FINISHED PLAYING

	if (App->video->isVideoFinished && actual_scene == Stages::INTRO_VIDEO)
	{
		restart = true;
		next_scene = Stages::MAIN_MENU;
	}

	//PORTAL SPAWN
	if (actual_scene == Stages::INGAME && lvlIndex < App->map->numberOfLevels && portal == nullptr && App->entities->enemiescount == 0)
	{
		GeneratePortal();
	}

	//GENERATE A NEW MAP
	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN && actual_scene == Stages::INGAME && !App->console->isWritting())
	{
		restart = true;
	}

	//Q: GO TO THE NEXT LEVEL
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN && actual_scene == Stages::INGAME && !App->console->isWritting())
	{
		GoNextLevel();
	}

	//F1: GO TO THE BOSS ROOM
	if (App->input->GetKey(SDL_SCANCODE_F1) == KeyState::KEY_DOWN && actual_scene == Stages::INGAME && !App->console->isWritting())
	{
		lvlIndex = 100;
		restart = true;
	}

	//PAUSE GAME
	if (actual_scene == Stages::INGAME)
	{
		if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN ||
			App->input->GetPadButtonDown(SDL_CONTROLLER_BUTTON_START) == KEY_DOWN)
		{
			if (!paused)
			{
				App->audio->PauseFX();
				paused = true;
				player->Walk(false);
				currentPercentAudio = App->audio->MusicVolumePercent;
				uint tmpAudio = (uint)(currentPercentAudio * 0.3f);
				if (tmpAudio == 0)
					tmpAudio = 1;
				App->audio->setMusicVolume(tmpAudio);
				CreatePauseMenu();

			}
			else if(!ItemSelection)
			{
				App->audio->ResumeFX();
				paused = false;
				player->Walk(true);
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

	if (actual_scene == Stages::MAIN_MENU && !menuAnimOn && SDL_RectEquals(&boltAnim.GetCurrentRect(), &SDL_Rect({ 14, 1344, 157, 241 })))
	{
		menuAnimOn = true;
		CreateMainMenuScreen();
	}

	//DRAWING THE BACKGROUND IN THE MAIN MENU
	if (actual_scene == Stages::MAIN_MENU)
	{
		SDL_Rect back = { 0,0,640,360 };
		//App->render->DrawQuad(back, 0, 205, 193, 255, true, false);
		//App->render->DrawQuad(back, 100, 66, 159, 255, true, false);
		App->render->Blit(textureBG, 0, 0, &rectBG, 1.0f, 0.0f);
		App->render->Blit(textureBG, 254, 0, &boltAnim.GetCurrentFrame(), 1.0f, 0.0f);
		App->render->Blit(textureBG, 6, 53, &thrallShadowAnim.GetCurrentFrame(), 1.0f, 0.0f);

		if (!menuAnimOff)
			App->render->Blit(App->gui->getAtlas(), 100, 25, &titleAnim.GetCurrentFrame(), 1.0f, 0.0f);
		else
			App->render->Blit(App->gui->getAtlas(), 100, 25, &SDL_Rect({ 624, 21, 448, 129 }), 1.0f, 0.0f);
	
	}
	else if (actual_scene == Stages::SETTINGS)
	{
		SDL_Rect back = { 0,0,640,360 };
		//App->render->DrawQuad(back, 0, 205, 193, 255, true, false);
		App->render->DrawQuad(back, 100, 66, 159, 255, true, false);
		App->render->Blit(textureBG, 0, 0, &rectBG, 1.0f, 0.0f);
		App->render->Blit(textureBG, 254, 0, &boltAnim.GetCurrentFrame(), 1.0f, 0.0f);
		App->render->Blit(textureBG, 6, 53, &thrallShadowAnim.GetCurrentFrame(), 1.0f, 0.0f);
	}

	//PRINTING WALKABLE TILES (Idk why this is here)
	if (App->path->printWalkables == true)
		App->path->PrintWalkableTiles();

	//SCENE RESET
	BROFILER_CATEGORY("SceneRestart", Profiler::Color::Chocolate);
	if (restart)
	{
		restart = false;

		if (next_scene == Stages::INGAME && actual_scene != Stages::MAIN_MENU)
		{
			App->transitions->StartTransition(this, this, 2.0f, fades::circular_fade);
		}
		else if ((actual_scene == Stages::MAIN_MENU && next_scene == Stages::INGAME) ||
				(actual_scene == Stages::INGAME && next_scene == Stages::MAIN_MENU))
		{
			App->transitions->StartTransition(this, this, 2.0f, fades::slider_fade);
		}

		if ((actual_scene == Stages::MAIN_MENU && next_scene == Stages::SETTINGS) ||
			(actual_scene == Stages::SETTINGS && next_scene == Stages::MAIN_MENU))
		{
			this->DeActivate();
			actual_scene = next_scene;
			this->Activate();
		}

		if (actual_scene == Stages::INTRO_VIDEO && next_scene == Stages::MAIN_MENU)
		{
			actual_scene = next_scene;
			App->transitions->StartTransition(this, this, 1.0f, fades::slider_fade);
		}
	}

	return ret;
}

bool Scene::CleanUp()
{
	if ((actual_scene == Stages::INTRO_VIDEO && next_scene == Stages::MAIN_MENU)
		|| (actual_scene == Stages::INGAME && next_scene == Stages::MAIN_MENU))
	{
		menuAnimOn = false;
		menuAnimOff = false;
	}
		
	
	if (actual_scene == Stages::SETTINGS && next_scene == Stages::MAIN_MENU)
	{
		menuAnimOff = true;
	}
		

	if (textureBG != nullptr)
	{
		App->textures->UnLoad(textureBG);
		textureBG = nullptr;
	}
	
	if (bgEmitter1 != nullptr)
	{
		App->psystem->RemoveEmitter(bgEmitter1);
		bgEmitter1 = nullptr;
	}

	if (bgEmitter2 != nullptr)
	{

		App->psystem->RemoveEmitter(bgEmitter2);
		bgEmitter2 = nullptr;
	}
	
	//HEALING THE PLAYER AFTER FINISHING A LVL
	if (player)
	{
		playerStats = player->numStats;
		uint quantityToHeal = (playerStats.maxhp - playerStats.hp) * playerStats.hpRecover / 100;
		playerStats.hp = playerStats.hp + quantityToHeal > playerStats.maxhp ? playerStats.maxhp : playerStats.hp + quantityToHeal;
	}

	if (testEmitter != nullptr)
	{
		App->psystem->RemoveEmitter(testEmitter);
		testEmitter = nullptr;
	}
		
	App->gui->DeActivate();
	App->map->DeActivate();
	App->entities->DeActivate();
	App->console->DeActivate();
	App->path->ClearMap();
	App->colliders->DeActivate();
	App->effects->DeActivate();
	App->projectiles->DeActivate();
	App->video->DeActivate();

	if (next_scene == Stages::MAIN_MENU)
	{
		App->items->DeActivate();
	}

	player = nullptr;
	lvlChest = nullptr;
	portal = nullptr;
	PauseMenu = nullptr;
	dashBinding = nullptr;
	skillBinding = nullptr;
	attackBinding = nullptr;

	return true;
}

void Scene::Save(pugi::xml_node& sceneNode)
{
	pugi::xml_node PlayerStats = sceneNode.append_child("PlayerStats");
	PlayerStats.append_attribute("hp").set_value(playerStats.hp);
	PlayerStats.append_attribute("maxhp").set_value(playerStats.maxhp);
	PlayerStats.append_attribute("damage").set_value(playerStats.damage);
	PlayerStats.append_attribute("energy").set_value(playerStats.energy);
	PlayerStats.append_attribute("energypercentbyhit").set_value(playerStats.energyPercentbyHit);
	PlayerStats.append_attribute("hpRecover").set_value(playerStats.hpRecover);
	PlayerStats.append_attribute("skillMultiplier").set_value(playerStats.skillMultiplier);
	PlayerStats.append_attribute("speed").set_value(playerStats.speed);

	pugi::xml_node MapStats = sceneNode.append_child("MapStats");
	MapStats.append_attribute("lvlIndex").set_value(lvlIndex);
}

void Scene::Load(const pugi::xml_node& sceneNode)
{
	pugi::xml_node PlayerStats = sceneNode.child("PlayerStats");
	playerStats.hp = PlayerStats.attribute("hp").as_float();
	playerStats.maxhp = PlayerStats.attribute("maxhp").as_float();
	playerStats.damage = PlayerStats.attribute("damage").as_float();
	playerStats.energy = PlayerStats.attribute("energy").as_float();
	playerStats.energyPercentbyHit = PlayerStats.attribute("energypercentbyhit").as_float();
	playerStats.hpRecover = PlayerStats.attribute("hpRecover").as_float();
	playerStats.skillMultiplier = PlayerStats.attribute("skillMultiplier").as_float();
	playerStats.speed = PlayerStats.attribute("speed").as_float();

	pugi::xml_node MapStats = sceneNode.child("MapStats");
	lvlIndex = MapStats.attribute("lvlIndex").as_int();
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
				{	if (button->btype != BType::CONTINUE || (button->btype == BType::CONTINUE && App->fs->isGameSaved()))
					{
						App->audio->HaltFX(App->audio->ButtonHovered);
						App->audio->PlayFx(App->audio->ButtonHovered);
						button->atlasRect = Button1MouseHover;
					}
					
					break;
				}
				case UIEvents::MOUSE_RIGHT_UP:
				{
					if (button->btype != BType::CONTINUE || (button->btype == BType::CONTINUE && App->fs->isGameSaved()))
						button->atlasRect = Button1MouseHover;
					break;
				}
				case UIEvents::MOUSE_LEFT_CLICK:
				{
					if (button->btype != BType::CONTINUE || (button->btype == BType::CONTINUE && App->fs->isGameSaved()))
					{
						App->audio->PlayFx(App->audio->ButtonClicked);
						button->atlasRect = Button1Pressed;
						button->MoveChilds({ 0.0f, 1.0f });
					}
					
					break;
				}
				case UIEvents::MOUSE_LEAVE:
				case UIEvents::NO_EVENT:
				{
					if(button->btype != BType::CONTINUE || (button->btype == BType::CONTINUE && App->fs->isGameSaved()))
						button->atlasRect = Button1;
					break;
				}
				case UIEvents::MOUSE_LEFT_UP:
				{
					if (button->btype != BType::CONTINUE || (button->btype == BType::CONTINUE && App->fs->isGameSaved()))
					{
						button->atlasRect = Button1MouseHover;
						button->MoveChilds({ 0.0f, -1.0f });
					}
					switch (button->btype)
					{
						case BType::PLAY:
							if (!App->transitions->IsFading())
							{
								playerStats = EntitySystem::PlayerStats();
								App->audio->PlayMusic(App->audio->InGameBSO.data(), 1);
								next_scene = Stages::INGAME;

								//New game, delete saved one
								App->fs->deleteSavedGame();

								restart = true;
							}
							break;
						case BType::EXIT_GAME:
							if (!App->transitions->IsFading())
							{
								return false;
							}
							break;
						case BType::SETTINGS:
							if (!App->transitions->IsFading())
							{
								next_scene = Stages::SETTINGS;
								restart = true;
							}
							break;
						case BType::GO_MMENU:
							if (!App->transitions->IsFading())
							{
								if (actual_scene == Stages::INGAME)
								{
									App->audio->PlayMusic(App->audio->MainMenuBSO.data(), 0);
									App->audio->setMusicVolume(currentPercentAudio);
									App->audio->HaltFX();
								}
								else if (actual_scene == Stages::SETTINGS)
									App->SaveInput();

								next_scene = Stages::MAIN_MENU;
								lvlIndex = 0;
								paused = false;
								restart = true;
								
							}
							break;
						case BType::RESUME:
							if (!App->transitions->IsFading())
							{
								paused = false;
								player->Walk(true);
								App->audio->ResumeFX();
								App->gui->DestroyElem(PauseMenu);
								App->audio->setMusicVolume(currentPercentAudio);
							}
							break;

						case BType::CONTINUE:
							if (!App->transitions->IsFading() && App->fs->isGameSaved())
							{
								//Call to the module transitions method to load the saved game with App->Load()
								next_scene = Stages::INGAME;

								App->audio->PlayMusic(App->audio->InGameBSO.data(), 1);
								App->transitions->loadTransition = true;
								restart = true;
							}
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

	//LOGO
	//GUIImage* logo = (GUIImage*)App->gui->CreateGUIImage({ 100,25 }, titleAnim.GetCurrentFrame(), nullptr);
	
	// { 624, 21, 448, 129 }


	//CONTINUE BUTTON
	Button* continueb = (Button*)App->gui->CreateButton({ 241.0f, 115 + 30 }, BType::CONTINUE, this, window);

	LabelInfo defLabelcont;
	defLabelcont.color = White;
	defLabelcont.fontName = "LifeCraft80";
	defLabelcont.text = "Continue";
	App->gui->CreateLabel({ 41,11 }, defLabelcont, continueb, this);

	//NEW GAME BUTTON
	Button* button = (Button*)App->gui->CreateButton({ 241.0f , 165 + 30}, BType::PLAY, this, window);

	LabelInfo defLabel;
	defLabel.color = White;
	defLabel.fontName = "LifeCraft80";
	defLabel.text = "New Game";
	App->gui->CreateLabel({ 35,11 }, defLabel, button, this);

	//SETTINGS BUTTON
	Button* button2 = (Button*)App->gui->CreateButton({ 241.0f , 215 + 30}, BType::SETTINGS, this, window);
	LabelInfo defLabel2;
	defLabel2.color = White;
	defLabel2.fontName = "LifeCraft80";
	defLabel2.text = "Settings";
	App->gui->CreateLabel({ 42,10 }, defLabel2, button2, this);

	//EXIT GAME BUTTON
	Button* button3 = (Button*)App->gui->CreateButton({ 241.0f , 265 + 30}, BType::EXIT_GAME, this, window);
	LabelInfo defLabel3;
	defLabel3.color = White;
	defLabel3.fontName = "LifeCraft80";
	defLabel3.text = "Quit";
	App->gui->CreateLabel({ 60,10 }, defLabel3, button3, this);

}

void Scene::CreateSettingsScreen()
{
	GUIWindow* window = (GUIWindow*)App->gui->CreateGUIWindow({ 0,0 }, { 0,0,0,0 }, nullptr, nullptr);

	//MUSIC VOLUME SLIDER
	SliderInfo sinfo;
	sinfo.type = Slider::SliderType::MUSIC_VOLUME;
	Slider* slider = (Slider*)App->gui->CreateSlider({ 50, 95 }, sinfo, this, window);

	LabelInfo defLabel3;
	defLabel3.color = White;
	defLabel3.fontName = "Arial80";
	std::string temp = (char*)std::to_string(App->audio->MusicVolumePercent).data();
	defLabel3.text = (char*)temp.data();
	App->gui->CreateLabel({ 265,-4 }, defLabel3, slider, this);

	LabelInfo defLabel;
	defLabel.color = White;
	defLabel.fontName = "LifeCraft90";
	defLabel.text = "Music Volume";
	App->gui->CreateLabel({ 0,-35 }, defLabel, slider, this);

	//FX VOLUME SLIDER
	SliderInfo sinfo2;
	sinfo2.type = Slider::SliderType::FX_VOLUME;
	Slider* slider2 = (Slider*)App->gui->CreateSlider({ 50, 190 }, sinfo2, this, window);

	LabelInfo defLabel4;
	defLabel4.color = White;
	defLabel4.fontName = "Arial80";
	std::string temp2 = (char*)std::to_string(App->audio->FXVolumePercent).data();
	defLabel4.text = (char*)temp2.data();
	App->gui->CreateLabel({ 265,-4 }, defLabel4, slider2, this);

	LabelInfo defLabel5;
	defLabel5.color = White;
	defLabel5.fontName = "LifeCraft90";
	defLabel5.text = "FX Volume";
	App->gui->CreateLabel({ 0,-35 }, defLabel5, slider2, this);

	if (App->input->isControllerConnected())
	{
		//ATTACK BINDING
		LabelInfo defAttackLabel;
		defAttackLabel.color = White;
		defAttackLabel.fontName = "LifeCraft90";
		defAttackLabel.text = "Attack";
		Label* attackLabel = attackBinding = (Label*)App->gui->CreateLabel({ 400,93 }, defAttackLabel, window, this);

		GUIImage* symbolAttack = (GUIImage*)App->gui->CreateGUIImage({ 80, 0 }, SELECTION_DOUBLE_SYMBOL, this, attackLabel);
		symbolAttack->symbol = true;

		LabelInfo attackKey;
		attackKey.color = White;
		attackKey.fontName = "Arial80";
		attackKey.text = (char*)App->input->getBindingfromAction("Attack");
		Label* attackkeyLabel = (Label*)App->gui->CreateLabel({ 49, 0 }, attackKey, symbolAttack, this);

		int w, h;
		SDL_QueryTexture((SDL_Texture*)attackkeyLabel->getTexturetoBlit(), nullptr, nullptr, &w, &h);
		attackkeyLabel->setLocalPos({ (float)(58 - ((w / 2) / 3)), -1 });

		//SKILL BINDING
		LabelInfo defSkillLabel;
		defSkillLabel.color = White;
		defSkillLabel.fontName = "LifeCraft90";
		defSkillLabel.text = "Skill";
		Label* skillLabel = skillBinding = (Label*)App->gui->CreateLabel({ 400,150 }, defSkillLabel, window, this);

		GUIImage* symbolSkill = (GUIImage*)App->gui->CreateGUIImage({ 80, 0 }, SELECTION_DOUBLE_SYMBOL, this, skillLabel);
		symbolSkill->symbol = true;

		LabelInfo skillKey;
		skillKey.color = White;
		skillKey.fontName = "Arial80";
		skillKey.text = (char*)App->input->getBindingfromAction("Skill");
		Label* skillkeyLabel = (Label*)App->gui->CreateLabel({ 49, 0 }, skillKey, symbolSkill, this);

		SDL_QueryTexture((SDL_Texture*)skillkeyLabel->getTexturetoBlit(), nullptr, nullptr, &w, &h);
		skillkeyLabel->setLocalPos({ (float)(58 - ((w / 2) / 3)), -1 });


		//DASH BINDING
		LabelInfo defDashLabel;
		defDashLabel.color = White;
		defDashLabel.fontName = "LifeCraft90";
		defDashLabel.text = "Dash";
		Label* dashLabel = dashBinding = (Label*)App->gui->CreateLabel({ 400,207 }, defDashLabel, window, this);

		GUIImage* symbolDash = (GUIImage*)App->gui->CreateGUIImage({ 80, 0 }, SELECTION_DOUBLE_SYMBOL, this, dashLabel);
		symbolDash->symbol = true;

		LabelInfo dashKey;
		dashKey.color = White;
		dashKey.fontName = "Arial80";
		dashKey.text = (char*)App->input->getBindingfromAction("Dash");
		Label* dashkeyLabel = (Label*)App->gui->CreateLabel({ 49, 0 }, dashKey, symbolDash, this);

		SDL_QueryTexture((SDL_Texture*)dashkeyLabel->getTexturetoBlit(), nullptr, nullptr, &w, &h);
		dashkeyLabel->setLocalPos({ (float)(58 - ((w / 2) / 3)), -1 });

	}
	
	//BACK BUTTON
	Button* button3 = (Button*)App->gui->CreateButton({ 50/*640 / 2 - 158 / 2*/, 300 }, BType::GO_MMENU, this, window);

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
	if (portal == nullptr && App->entities->spritesheetsEntities.size() > 0)
	{
		iPoint position = App->map->GetRandomValidPointProxyForThisPos(5, 2, { (int)player->pos.x, (int)player->pos.y });
		portal = (PortalEntity*)App->entities->AddStaticEntity({ (float)position.x * 46, (float)position.y * 46 }, PORTAL);
	}
}

void Scene::GoMainMenu()
{
	if (actual_scene == Stages::INGAME)
	{
		App->audio->PlayMusic(App->audio->MainMenuBSO.data(), 0.5f);
	}
		
	next_scene = Stages::MAIN_MENU;
	restart = true;
	lvlIndex = 0;
}

void Scene::CreateGratitudeScreen()
{
	static bool alreadyCreated = false;

	if(!alreadyCreated)
	{
		GUIWindow* window = (GUIWindow*)App->gui->CreateGUIWindow({ 0,0 }, { 0,0,0,0 }, nullptr, nullptr);
		window->blackBackground = true;
		gratitudeON = true;
		LabelInfo gratitude;
		gratitude.color = White;
		gratitude.fontName = "LifeCraft90";
		gratitude.multilabelWidth = 1500;
		gratitude.text = "                         Victory! \n       Thanks for playing the game. \n       Your support means a lot ^^ \n       More at: @SoftCactus_Team";
		App->gui->CreateLabel({ 150, 130 }, gratitude, nullptr, nullptr);
	}
	alreadyCreated = true;
}

void Scene::CreateItemSelectionScreen(Item* item1, Item* item2, Item* item3)
{
	paused = true;
	ItemSelection = (GUIWindow*)App->gui->CreateGUIWindow({ 0,0 }, { 0,0,0,0 }, nullptr, nullptr);
	ItemSelection->blackBackground = true;
	ItemSelection->vertical = false;

	App->gui->CreateItemContainer({ 30+85,50+121 }, item1, ItemSelection, this);
	App->gui->CreateItemContainer({ 230+85,50+121 }, item2, ItemSelection, this);
	App->gui->CreateItemContainer({ 430+85,50+121 }, item3, ItemSelection, this);
}

void Scene::GoNextLevel()
{
	lvlIndex++;
	restart = true;
}
