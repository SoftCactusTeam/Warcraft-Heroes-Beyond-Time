#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "ModuleProjectiles.h"
#include "p2Point.h"
#include "SDL\include\SDL.h"

struct Collider;

struct ProjectileInfo
{
	ProjectileInfo(const ProjectileInfo& info) : pos(info.pos), rectToBlit(info.rectToBlit), layer(info.layer) {};
	iPoint pos = { 0,0 };
	SDL_Rect rectToBlit = { 0,0,0,0 };
	int layer = 0;
};

class Projectile
{
public:

	Projectile(const ProjectileInfo& info, Projectile_type type);
	~Projectile();

	bool Update(float dt);
	bool Draw();

	void OnCollision(Collider* yours, Collider* collideWith);
	void OnCollisionContinue(Collider* yours, Collider* collideWith);
	void OnCollisionLeave(Collider* yours, Collider* collideWith);

private:

	Collider* projCollider = nullptr;
	Projectile_type type = Projectile_type::no_type;
	ProjectileInfo data;
};

#endif
