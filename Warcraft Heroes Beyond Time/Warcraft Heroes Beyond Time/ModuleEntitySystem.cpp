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
#include "Guldan.h"

#include "Console.h"

#include "Brofiler\Brofiler.h"

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
			App->entities->AddEnemy({ App->scene->player->pos.x, App->scene->player->pos.y - 60 }, FOOTMAN);
		}
		else if (parameter == "thrall")
		{
			App->entities->AddPlayer({ App->scene->player->pos.x, App->scene->player->pos.y - 60 }, THRALL);
		}
		else if (parameter == "archer")
		{
			App->entities->AddEnemy({ App->scene->player->pos.x, App->scene->player->pos.y - 60 }, ARCHER);
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
		else if (parameter == "godmode" && parameterNumeric == 1)
		{
			//Activate Godmode
		}

		else if (parameter == "godmode" && parameterNumeric == 1)
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

	//------------- Loading Stats -------------------------------------------------------------
	pugi::xml_node thrall = entitiesNode.child("players").child("thrall");
	thrallstats.maxhp = thrall.attribute("hp").as_uint(0);
	thrallstats.hp = thrallstats.maxhp;
	thrallstats.speed = thrall.attribute("speed").as_uint(0);
	thrallstats.damage = thrall.attribute("damage").as_uint(0);
	thrallstats.energyPercentbyHit = thrall.attribute("energy_percent_hit").as_uint(20);
	thrallstats.skillMultiplier = thrall.attribute("skill_multiplier").as_uint(3);

	pugi::xml_node archerT1 = entitiesNode.child("enemies").child("archer_tier1");
	archerT1stats.maxhp =						archerT1.attribute("hp").as_uint(0);
	archerT1stats.hp =							archerT1stats.maxhp;
	archerT1stats.speed =						archerT1.attribute("speed").as_uint(0);
	archerT1stats.damage =						archerT1.attribute("damage").as_float(0);
	archerT1stats.dropping_chance =				archerT1.attribute("dropping_chance").as_uint(0);
	archerT1stats.strong_attack_chance =		archerT1.attribute("chance_powerful_attack").as_uint();
	archerT1stats.arrows_life =					archerT1.attribute("arrows_life").as_float();
	archerT1stats.arrows_speed =				archerT1.attribute("arrows_speed").as_float();
	archerT1stats.time_between_attacks =		archerT1.attribute("time_between_attacks").as_float();
	archerT1stats.time_stunned_after_attack =	archerT1.attribute("time_stunned_after_attack").as_float();
	archerT1stats.vision_range =				archerT1.attribute("vision_range").as_uint();
	archerT1stats.attack_range =				archerT1.attribute("attack_range").as_uint();
	archerT1stats.minimal_distance_player =		archerT1.attribute("minimal_distance_with_player").as_uint();

	pugi::xml_node archerT2 =					entitiesNode.child("enemies").child("archer_tier2");
	archerT2stats.maxhp =						archerT2.attribute("hp").as_uint(0);
	archerT2stats.hp =							archerT2stats.maxhp;
	archerT2stats.speed =						archerT2.attribute("speed").as_uint(0);
	archerT2stats.damage =						archerT2.attribute("damage").as_float(0);
	archerT2stats.dropping_chance =				archerT2.attribute("dropping_chance").as_uint(0);
	archerT2stats.strong_attack_chance =		archerT2.attribute("chance_powerful_attack").as_uint();
	archerT2stats.arrows_life =					archerT2.attribute("arrows_life").as_float();
	archerT2stats.arrows_speed =				archerT2.attribute("arrows_speed").as_float();
	archerT2stats.time_between_attacks =		archerT2.attribute("time_between_attacks").as_float();
	archerT2stats.time_stunned_after_attack =	archerT2.attribute("time_stunned_after_attack").as_float();
	archerT2stats.vision_range =				archerT2.attribute("vision_range").as_uint();
	archerT2stats.attack_range =				archerT2.attribute("attack_range").as_uint();
	archerT2stats.minimal_distance_player =		archerT2.attribute("minimal_distance_with_player").as_uint();

	pugi::xml_node archerT3 = entitiesNode.child("enemies").child("archer_tier3");
	archerT3stats.maxhp =						archerT3.attribute("hp").as_uint(0);
	archerT3stats.hp =							archerT3stats.maxhp;
	archerT3stats.speed =						archerT3.attribute("speed").as_uint(0);
	archerT3stats.damage =						archerT3.attribute("damage").as_float(0);
	archerT3stats.dropping_chance =				archerT3.attribute("dropping_chance").as_uint(0);
	archerT3stats.strong_attack_chance =		archerT3.attribute("chance_powerful_attack").as_uint();
	archerT3stats.arrows_life =					archerT3.attribute("arrows_life").as_float();
	archerT3stats.arrows_speed =				archerT3.attribute("arrows_speed").as_float();
	archerT3stats.time_between_attacks =		archerT3.attribute("time_between_attacks").as_float();
	archerT3stats.time_stunned_after_attack =	archerT3.attribute("time_stunned_after_attack").as_float();
	archerT3stats.vision_range =				archerT3.attribute("vision_range").as_uint();
	archerT3stats.attack_range =				archerT3.attribute("attack_range").as_uint();
	archerT3stats.minimal_distance_player =		archerT3.attribute("minimal_distance_with_player").as_uint();


	pugi::xml_node guldan = entitiesNode.child("enemies").child("guldan");
	guldanstats.maxhp = guldan.attribute("hp").as_uint(0);
	guldanstats.hp = guldanstats.maxhp;
	guldanstats.speed = guldan.attribute("speed").as_uint(0);
	guldanstats.damage = guldan.attribute("damage").as_uint(0);
	guldanstats.dropping_chance = guldan.attribute("dropping_chance").as_uint(0);
	guldanstats.strong_attack_chance = guldan.attribute("strong_attack_chance").as_uint();

	

	//Loading CD'S---------------------------------------------------------------------------
	dashCD = entitiesNode.child("players").child("cds").attribute("dash").as_float(1);
	timepostdead = entitiesNode.child("players").child("cds").attribute("deadinfloor").as_float(2);
	invpostdamaged = entitiesNode.child("players").child("cds").attribute("invulpostdamaged").as_float(1);

	return true;
}

bool EntitySystem::Start()
{
	BROFILER_CATEGORY("ActivateEntities", Profiler::Color::Chocolate);
	LOG("Loading textures");
	spritesheetsEntities.push_back(App->textures->Load("sprites/thrall_spritesheet.png"));
	spritesheetsEntities.push_back(App->textures->Load("sprites/Footman/Footman_sprite.png"));
	spritesheetsEntities.push_back(App->textures->Load("sprites/all_items.png"));
	spritesheetsEntities.push_back(App->textures->Load("sprites/portal.png"));
	spritesheetsEntities.push_back(App->textures->Load("sprites/Archer/Archer_sprite.png"));
	spritesheetsEntities.push_back(App->textures->Load("sprites/Projectiles.png"));
	spritesheetsEntities.push_back(App->textures->Load("sprites/Archer/Archer_Smoke.png"));
	spritesheetsEntities.push_back(App->textures->Load("sprites/Boss_Guldan.png"));

	bool ret = true;

	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end() && ret; ++it)
	{
		ret = (*it)->Start();
	}

	enemiescount = 0;

	return true;
}

bool EntitySystem::PreUpdate()
{
	if (toSpawn.size() >= 0)
	{
		for (std::list<Entity*>::iterator it = toSpawn.begin(); it != toSpawn.end(); ++it)
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

	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end() && ret;)
	{
		ret = (*it)->Draw();

		if (ret)
			ret = (*it)->PostUpdate();

		if ((*it)->destroy)
		{
			App->colliders->deleteColliderbyOwner(*it);
			delete(*it);
			it = entities.erase(it);
		}
		else
		{
			++it;
		}

	}

	return ret;
}

bool EntitySystem::CleanUp()
{
	BROFILER_CATEGORY("ClearENTITIES", Profiler::Color::Chocolate);
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
	enemiescount++;
	BROFILER_CATEGORY("AddEnemy", Profiler::Color::Chocolate);
	EnemyEntity* newEntity = nullptr;
	switch (type) {
	case ENEMY_TYPE::FOOTMAN:
		newEntity = new Enemy_Footman(coor, ENEMY_TYPE::FOOTMAN, spritesheetsEntities[FOOTMAN_SHEET]);
		App->colliders->AddCollider({ 20,20,32,32 }, Collider::ColliderType::ENTITY, (Entity*)newEntity);
		break;
	case ENEMY_TYPE::ARCHER:
		newEntity = new Enemy_Archer(coor, ENEMY_TYPE::ARCHER, spritesheetsEntities[ARCHER_SHEET], ARCHER_TIER_1);
		App->colliders->AddCollider({ -16+20,-16+20,32,32 }, Collider::ColliderType::ENTITY, (Entity*)newEntity);
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

}

BossEntity* EntitySystem::AddBoss(fPoint coor, BossType type)
{
	BossEntity* newEntity = nullptr;
	switch (type) {
	case BossType::GULDAN:
		newEntity = new Guldan(coor, BossType::GULDAN, spritesheetsEntities[GULDAN_SHEET]);
		break;
	/*case BOSS_TYPE::LICH_KING:
		newEntity = new BossEntity(coor, BOSS_TYPE::LICH_KING, nullptr);
		break;
	case BOSS_TYPE::ILLIDAN:
		newEntity = new BossEntity(coor, BOSS_TYPE::ILLIDAN, nullptr);
		break;*/
	}
	toSpawn.push_back(newEntity);

	return newEntity;
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
		newEntity = new PortalEntity(coor, STATIC_ENTITY_TYPE::PORTAL, spritesheetsEntities[PORTAL_SHEET]);
		newEntity->portalCol = App->colliders->AddCollider({ (int)newEntity->pos.x, (int)newEntity->pos.y, 50, 50 }, Collider::ColliderType::PORTAL);
		break;
	}
	toSpawn.push_back(newEntity);
	newEntity->OpenPortal();

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

void EntitySystem::AddCommands()
{
	ConsoleOrder* spawn_consoleOrder = new Spawn_ConsoleOrder;
	App->console->AddConsoleOrderToList(spawn_consoleOrder);

	ConsoleOrder* equip_consoleOrder = new Equip_ConsoleOrder;
	App->console->AddConsoleOrderToList(equip_consoleOrder);

	ConsoleOrder* clear_consoleOrder = new Clear_ConsoleOrder;
	App->console->AddConsoleOrderToList(clear_consoleOrder);

	ConsoleOrder* player_consoleOrder = new Player_ConsoleOrder;
	App->console->AddConsoleOrderToList(player_consoleOrder);
}

void EntitySystem::ClearEnemies()
{
	//I need a fucking type to do this. Sorry for the madness.

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

bool EntitySystem::checkEntityNearOther(Entity * enity)
{
	std::list<Entity*>::iterator it = App->entities->entities.begin();
	for (; it != App->entities->entities.end(); it++)
	{
		if (*it != enity)
		{
			uint totalX = (*enity).pos.x - (*it)->pos.x;
			uint totalY = (*enity).pos.y - (*it)->pos.y;
			uint distance =  sqrt((totalX * totalX) + (totalY * totalY));
			if (distance < (*enity).anim->GetCurrentRect().w)
			{
				printf_s("colliding");
				return true;
			}
		}
	}
	return false;
}