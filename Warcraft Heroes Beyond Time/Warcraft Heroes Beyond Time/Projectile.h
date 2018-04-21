#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "ModuleProjectiles.h"
#include "p2Point.h"
#include "SDL\include\SDL.h"
#include "Animation.h"

struct Collider;

struct ProjectileInfo
{
	ProjectileInfo() {};
	ProjectileInfo(const ProjectileInfo& info) : pos(info.pos), layer(info.layer), speed(info.speed), life(info.life) {};
	ProjectileInfo(const iPoint pos, const int layer, int speed, int life) : pos(pos), layer(layer), speed(speed), life(life) {};

	int life = 0;
	int layer = 0;
	int speed = 0;
	iPoint pos = { 0,0 };
};

class Projectile
{
public:

	Projectile();
	Projectile(const ProjectileInfo& info, Projectile_type type);
	~Projectile();

	bool Update(float dt);
	bool Draw();

	int DecreaseLifePerTime(float dt);

	void OnCollision(Collider* yours, Collider* collideWith);
	void OnCollisionContinue(Collider* yours, Collider* collideWith);
	void OnCollisionLeave(Collider* yours, Collider* collideWith);

protected:

	Collider* projCollider = nullptr;
	Projectile_type type = Projectile_type::no_type;
	Animation* actualAnim = nullptr;

private:
	ProjectileInfo data;
};

#endif
