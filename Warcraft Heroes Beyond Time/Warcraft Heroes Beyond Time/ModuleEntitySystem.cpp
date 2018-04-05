#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Application.h"
#include "Log.h"
#include "ModuleEntitySystem.h"
#include "ModuleTextures.h"
#include "Scene.h"

#include "Entity.h"
#include "PlayerEntity.h"
#include "BossEntity.h"
#include "EnemyEntity.h"
#include "ConsumableEntity.h"
#include "ChestEntity.h"
#include "StaticObjectEntity.h"
#include "ModuleMapGenerator.h"
#include "ModuleColliders.h"
#include "Thrall.h"
#include "Enemy_Footman.h"
#include "Enemy_Archer.h"
#include "PortalEntity.h"

#include "Console.h"

#include "Brofiler\Brofiler.h"

class Entities_ConsoleOrder : public ConsoleOrder 
{
	std::string orderName() 
	{ 
		return "spawn"; 
	}

	void Exec(std::string parameter, int parameterNumeric) 
	{
		if (parameter == "footman")
			App->entities->AddEnemy(App->scene->player->pos, FOOTMAN);
		if (parameter == "archer")
			App->entities->AddEnemy(App->scene->player->pos, ARCHER);
	}
};

class Spawn_ConsoleOrder : public ConsoleOrder
{
	std::string orderName()
	{
		return "spawn";
	}

	void Exec(std::string parameter, int parameterNumeric)
	{
		if (parameter == "footman")
		{

		}
		else if (parameter == "thrall")
		{
			App->entities->AddPlayer({ App->scene->player->pos.x, App->scene->player->pos.y - 60 }, THRALL);
		}
		else if (parameter == "archer")
		{

		}

		else if (parameter == "wizard")
		{

		}
		else if (parameter == "darkknight")
		{

		}
		else if (parameter == "healthpotion")
		{

		}
		
	}
};

class Equip_ConsoleOrder : public ConsoleOrder
{
	std::string orderName()
	{
		return "equip";
	}

	void Exec(std::string parameter, int parameterNumeric)
	{
		//Items
	}
};

class Clear_ConsoleOrder : public ConsoleOrder
{
	std::string orderName()
	{
		return "clear";
	}

	void Exec(std::string parameter, int parameterNumeric)
	{
		if (parameter == "enemies")
		{
			//EntitySystem Function that set all enemies boolean to true.
		}
	}
};

class Player_ConsoleOrder : public ConsoleOrder
{
	std::string orderName()
	{
		return "player";
	}

	void Exec(std::string parameter, int parameterNumeric)
	{
		if (parameter == "energy")
		{
			//Set energy to parameterNumeric (not more than 100%).
		}
		else if (parameter == "godmodeon")
		{
			//Activate Godmode
		}
			
		else if (parameter == "godmodeoff")
		{
			//DeActivate Godmode
		}
		else if (parameter == "freezone")
		{
			App->scene->player->DrawFreeZone((bool)parameterNumeric);
		}
			
	}
};

EntitySystem::EntitySystem() : Module()
{
	name = "entitySystem";
}

void EntitySystem::Init()
{
	active = false;
}

bool EntitySystem::Awake(pugi::xml_node& entitiesNode)
{
	srand(time(NULL));
	ConsoleOrder* txell_consoleOrder = new Entities_ConsoleOrder;
	App->console->AddConsoleOrderToList(txell_consoleOrder);

	ConsoleOrder* spawn_consoleOrder = new Spawn_ConsoleOrder;
	App->console->AddConsoleOrderToList(spawn_consoleOrder);

	ConsoleOrder* equip_consoleOrder = new Equip_ConsoleOrder;
	App->console->AddConsoleOrderToList(equip_consoleOrder);

	ConsoleOrder* clear_consoleOrder = new Clear_ConsoleOrder;
	App->console->AddConsoleOrderToList(clear_consoleOrder);

	ConsoleOrder* player_consoleOrder = new Player_ConsoleOrder;
	App->console->AddConsoleOrderToList(player_consoleOrder);

	//------------- Loading Stats -------------------------------------------------------------
	pugi::xml_node thrall = entitiesNode.child("players").child("thrall");
	thrallstats.hp = thrall.attribute("hp").as_uint(0);
	thrallstats.speed = thrall.attribute("speed").as_uint(0);
	thrallstats.damage = thrall.attribute("damage").as_uint(0);
	thrallstats.energyPercentbyHit = thrall.attribute("energy_percent_hit").as_uint(0);

	pugi::xml_node footman = entitiesNode.child("enemies").child("footman");
	footmanstats.hp = footman.attribute("hp").as_uint(0);
	footmanstats.speed = footman.attribute("speed").as_uint(0);
	footmanstats.damage = footman.attribute("damage").as_uint(0);
	footmanstats.dropping_chance = footman.attribute("dropping_chance").as_uint(0);
	footmanstats.range = footman.attribute("range").as_uint(0);
	footmanstats.difficulty = footman.attribute("difficulty").as_uint(0);

	pugi::xml_node archer = entitiesNode.child("enemies").child("archer");
	archerstats.hp = archer.attribute("hp").as_uint(0);
	archerstats.speed = archer.attribute("speed").as_uint(0);
	archerstats.damage = archer.attribute("damage").as_uint(0);
	archerstats.dropping_chance = archer.attribute("dropping_chance").as_uint(0);
	archerstats.range = archer.attribute("range").as_uint(0);
	archerstats.difficulty = archer.attribute("difficulty").as_uint(0);

	pugi::xml_node wizard = entitiesNode.child("enemies").child("wizard");
	wizardstats.hp = wizard.attribute("hp").as_uint(0);
	wizardstats.speed = wizard.attribute("speed").as_uint(0);
	wizardstats.damage = wizard.attribute("damage").as_uint(0);
	wizardstats.dropping_chance = wizard.attribute("dropping_chance").as_uint(0);
	wizardstats.range = wizard.attribute("range").as_uint(0);
	wizardstats.difficulty = wizard.attribute("difficulty").as_uint(0);

	pugi::xml_node darkknight = entitiesNode.child("enemies").child("dark_knight");
	darkknightstats.hp = darkknight.attribute("hp").as_uint(0);
	darkknightstats.speed = darkknight.attribute("speed").as_uint(0);
	darkknightstats.damage = darkknight.attribute("damage").as_uint(0);
	darkknightstats.dropping_chance = darkknight.attribute("dropping_chance").as_uint(0);
	darkknightstats.range = darkknight.attribute("range").as_uint(0);
	darkknightstats.difficulty = darkknight.attribute("difficulty").as_uint(0);

	return true;
}

bool EntitySystem::Start()
{
	LOG("Loading textures");
	spritesheetsEntities.push_back(App->textures->Load("images/thrall_spritesheet.png"));
	spritesheetsEntities.push_back(App->textures->Load("Sprites/Footman/Footman_sprite.png"));
	spritesheetsEntities.push_back(App->textures->Load("all_items.png"));
	spritesheetsEntities.push_back(App->textures->Load("Mines.png"));
	spritesheetsEntities.push_back(App->textures->Load("Sprites/Archer/Archer_sprite.png"));
	spritesheetsEntities.push_back(App->textures->Load("Sprites/Archer/Archer_Arrow.png"));

	bool ret = true;

	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end() && ret; ++it)
	{
		ret = (*it)->Start();
	}

	return true;
}

bool EntitySystem::PreUpdate()
{	
	if (toSpawn.size() >= 0)
	{
		for(std::list<Entity*>::iterator it = toSpawn.begin(); it != toSpawn.end(); ++it)
		{
			(*it)->Start();
			entities.push_back(*it);
		}
		toSpawn.clear();
	}

	return toSpawn.size() <= 0;
}

bool EntitySystem::Update(float dt)
{
	bool ret = true;

	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end() && ret; ++it)
	{
		ret = (*it)->Update(dt);
	}

	return ret;
}

bool EntitySystem::PostUpdate()
{
	bool ret = true;

	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end() && ret; ++it)
	{
		ret = (*it)->Draw();

		if(ret)
			ret = (*it)->PostUpdate();

		if ((*it)->destroy)
			entities.remove((*it));
	}

	return ret;
}

bool EntitySystem::CleanUp()
{
	bool ret = true;

	LOG("Entity System: CleanUp...");

	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end() && ret; ++it)
		ret = (*it)->Finish();
	
	if (ret)
		ret = ClearEntitiesList();

	if (ret)
		ret = UnloadTexturesVector();

	return ret;
}

bool EntitySystem::ClearEntitiesList()
{
	bool ret = true;

	LOG("Entity System: Releasing entities...");

	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end() && ret; ++it)
	{
		delete *it;
	}

	entities.clear();

	return entities.size() <= 0;
}

bool EntitySystem::UnloadTexturesVector()
{
	bool ret = true;

	LOG("Entity System: Unloading textures...");

	for (int i = 0; i < spritesheetsEntities.size() && ret; ++i)
	{
		ret = App->textures->UnLoad(spritesheetsEntities[i]);
	}

	if (ret)
		spritesheetsEntities.clear();

	return spritesheetsEntities.size() <= 0;
}

void EntitySystem::AddEnemy(fPoint coor, ENEMY_TYPE type)
{
	BROFILER_CATEGORY("AddEnemy", Profiler::Color::Chocolate);
	EnemyEntity* newEntity = nullptr;
	switch (type) {
	case ENEMY_TYPE::FOOTMAN:
		newEntity = new Enemy_Footman(coor, ENEMY_TYPE::FOOTMAN, spritesheetsEntities[FOOTMAN_SHEET]);
		break;
	case ENEMY_TYPE::ARCHER:
		newEntity = new Enemy_Archer(coor, ENEMY_TYPE::ARCHER, spritesheetsEntities[ARCHER_SHEET]);
		break;
	case ENEMY_TYPE::MAGE:
		newEntity = new EnemyEntity(coor, ENEMY_TYPE::MAGE, nullptr);
		break;
	case ENEMY_TYPE::DEATH_KNIGHT:
		newEntity = new EnemyEntity(coor, ENEMY_TYPE::DEATH_KNIGHT, nullptr);
		break;
	case ENEMY_TYPE::GOBLIN:
		newEntity = new EnemyEntity(coor, ENEMY_TYPE::GOBLIN, nullptr);
		break;
	case ENEMY_TYPE::SKELETON:
		newEntity = new EnemyEntity(coor, ENEMY_TYPE::SKELETON, nullptr);
		break;
	}
	toSpawn.push_back(newEntity);
	App->colliders->AddCollider({ 0,0,32,32 }, COLLIDER_ENEMY, (Entity*)newEntity, { 20,20 });

}

void EntitySystem::AddBoss(fPoint coor, BOSS_TYPE type)
{
	BossEntity* newEntity = nullptr;
	switch (type) {
	case BOSS_TYPE::GULDAN:
		newEntity = new BossEntity(coor, BOSS_TYPE::GULDAN, nullptr);
		break;
	case BOSS_TYPE::LICH_KING:
		newEntity = new BossEntity(coor, BOSS_TYPE::LICH_KING, nullptr);
		break;
	case BOSS_TYPE::ILLIDAN:
		newEntity = new BossEntity(coor, BOSS_TYPE::ILLIDAN, nullptr);
		break;
	}
	toSpawn.push_back((Entity*)newEntity);
}

PlayerEntity* EntitySystem::AddPlayer(fPoint coor, PLAYER_TYPE type)
{
	BROFILER_CATEGORY("AddPlayer", Profiler::Color::Chocolate);
	PlayerEntity* newEntity = nullptr;
	switch (type)
	{
	case PLAYER_TYPE::THRALL:
		newEntity = new Thrall(coor, PLAYER_TYPE::THRALL, spritesheetsEntities[THRALL_SHEET]);
		break;
	case PLAYER_TYPE::VALEERA:
		newEntity = new PlayerEntity(coor, PLAYER_TYPE::VALEERA, nullptr);
		break;
	case PLAYER_TYPE::SYLVANAS:
		newEntity = new PlayerEntity(coor, PLAYER_TYPE::SYLVANAS, nullptr);
		break;
	}

	toSpawn.push_back(newEntity);
	App->colliders->AddCollider({ 0,0,32,32 }, COLLIDER_PLAYER, newEntity,{10,10});
	return newEntity;
}

void EntitySystem::AddConsumable(fPoint coor, CONSUMABLE_TYPE type)
{
	ConsumableEntity* newEntity = nullptr;
	switch (type) {
	case CONSUMABLE_TYPE::ATACK_POTION:
		newEntity = new ConsumableEntity(coor, CONSUMABLE_TYPE::ATACK_POTION, nullptr);
		break;
	case CONSUMABLE_TYPE::LIVE_POTION:
		newEntity = new ConsumableEntity(coor, CONSUMABLE_TYPE::LIVE_POTION, nullptr);
		break;
	case CONSUMABLE_TYPE::MOVEMENT_SPEED_POTION:
		newEntity = new ConsumableEntity(coor, CONSUMABLE_TYPE::MOVEMENT_SPEED_POTION, nullptr);
		break;
	}
	toSpawn.push_back((Entity*)newEntity);
}

ChestEntity* EntitySystem::AddChest(fPoint coor, CHEST_TYPE type) 
{
	BROFILER_CATEGORY("AddChest", Profiler::Color::Chocolate);
	ChestEntity* newEntity = nullptr;
	switch (type) {
	case CHEST_TYPE::LOW_CHEST:
		newEntity = new ChestEntity(coor, CHEST_TYPE::LOW_CHEST, nullptr);
		break;
	case CHEST_TYPE::MID_CHEST:
		newEntity = new ChestEntity(coor, CHEST_TYPE::MID_CHEST, spritesheetsEntities[ITEMS_SHEET]);
		break;
	case CHEST_TYPE::HIGH_CHEST:
		newEntity = new ChestEntity(coor, CHEST_TYPE::HIGH_CHEST, nullptr);
		break;
	}

	toSpawn.push_back(newEntity);

	return newEntity;
}

StaticEntity* EntitySystem::AddStaticEntity(fPoint coor, STATIC_ENTITY_TYPE type)
{
	BROFILER_CATEGORY("AddStaticEntity", Profiler::Color::Chocolate);
	PortalEntity* newEntity = nullptr;
	switch (type)
	{
	case STATIC_ENTITY_TYPE::PORTAL:
		newEntity = new PortalEntity(coor, STATIC_ENTITY_TYPE::PORTAL, spritesheetsEntities[MINES_SHEET]);
		break;
	}
	toSpawn.push_back(newEntity);

	return newEntity;
}

void EntitySystem::Save(pugi::xml_node& eSystemNode)
{
	eSystemNode.append_attribute("Testing") = "True";
}

void EntitySystem::Load(const pugi::xml_node& eSystemNode)
{

	return;
}

void EntitySystem::ClearEnemies()
{
	/*std::list<Entity*>::iterator it;
	for (it = entities.begin(); it != entities.end(); ++it)
	{
		if((*it)->)
	}*/
}


int EntitySystem::GetRandomNumber(int rang)
{
	return rand() % rang;
}