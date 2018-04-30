#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleProjectiles.h"
#include "Projectile.h"
#include "FelBall.h"
#include "ArcherArrow.h"

#include <assert.h>

ModuleProjectiles::ModuleProjectiles()
{
}

ModuleProjectiles::~ModuleProjectiles()
{
}

void ModuleProjectiles::Init()
{
	active = false;
}

bool ModuleProjectiles::Awake(pugi::xml_node& xml_node)
{
	return true;
}

bool ModuleProjectiles::Start()
{
	projectilesAtlas = App->textures->Load("sprites/ProjectilesAtlas.png");
	projectileClassicAtlas = App->textures->Load("sprites/Projectiles.png");
	return true;
}

bool ModuleProjectiles::PreUpdate()
{
	if (toSpawnProjectilesList.size() > 0)
	{
		std::list<Projectile*>::const_iterator it;

		for (it = toSpawnProjectilesList.begin(); it != toSpawnProjectilesList.end(); ++it)
		{
			projectilesList.push_back(*it);
		}

		toSpawnProjectilesList.clear();
	}

	return toSpawnProjectilesList.size() <= 0;
}

bool ModuleProjectiles::Update(float dt)
{
	bool result = true;

	std::list<Projectile*>::const_iterator it;

	for (it = projectilesList.begin(); it != projectilesList.end() && result; ++it)
	{
		result = (*it)->Update(dt);
	}

	return result;
}

bool ModuleProjectiles::PostUpdate()
{
	bool ret = true;

	if (toKillProjectilesList.size() > 0)
	{
		std::list<Projectile*>::const_iterator it;

		for (it = toKillProjectilesList.begin(); it != toKillProjectilesList.end(); ++it)
		{
			projectilesList.remove(*it);
			delete (*it);
		}

		toKillProjectilesList.clear();
	}

	ret = toKillProjectilesList.size() <= 0;

	if (ret)
	{
		std::list<Projectile*>::const_iterator it;

		for (it = projectilesList.begin(); it != projectilesList.end() && ret; ++it)
		{
			ret = (*it)->Draw();
		}
	}

	return ret;
}

bool ModuleProjectiles::CleanUp()
{
	std::list<Projectile*>::const_iterator it;

	for (it = projectilesList.begin(); it != projectilesList.end(); ++it)
	{
		delete (*it);
	}

	projectilesList.clear();

	toSpawnProjectilesList.clear();

	toKillProjectilesList.clear();

	App->textures->UnLoad(projectilesAtlas);

	return projectilesList.size() <= 0;
}

void ModuleProjectiles::AddProjectile(const ProjectileInfo* projectile, Projectile_type type)
{
	assert((int)type != (int)Projectile_type::no_type);

	Projectile* newProjectile = nullptr;

	switch (type)
	{
	case Projectile_type::fel_ball:
		newProjectile = new FelBall(new FelBallInfo(*(FelBallInfo*)projectile), type);
		break;
	case Projectile_type::archer_arrow:
		newProjectile = new ArcherArrow(new ArcherArrowInfo(*(ArcherArrowInfo*)projectile), type);
		break;
	}

	toSpawnProjectilesList.push_back(newProjectile);
}

void ModuleProjectiles::DestroyProjectile(Projectile* toDelete)
{
	toKillProjectilesList.push_back(toDelete);
}

const SDL_Texture* ModuleProjectiles::GetProjectileAtlas() const
{
	return projectilesAtlas;
}

const SDL_Texture* ModuleProjectiles::GetProjectileClassicAtlas() const
{
	return projectileClassicAtlas;
}