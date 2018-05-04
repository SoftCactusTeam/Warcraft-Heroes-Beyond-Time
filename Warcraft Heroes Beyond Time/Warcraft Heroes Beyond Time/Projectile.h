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
	ProjectileInfo(const ProjectileInfo& info) : pos(info.pos), layer(info.layer), life(info.life) {};

	int life = 0;
	int layer = 0;
	fPoint pos = { 0.0f,0.0f };
};

class Projectile
{
public:

	Projectile();
	Projectile(const ProjectileInfo* info, Projectile_type type);
	virtual ~Projectile();

	virtual bool Update(float dt);
	virtual bool Draw() const;

	virtual int DecreaseLifePerTime(float dt);
	virtual fPoint RotateAround(fPoint pointToRotate, fPoint rotationPivot, double angle, double radius) const;

	virtual void OnCollision(Collider* yours, Collider* collideWith);
	virtual void OnCollisionContinue(Collider* yours, Collider* collideWith);
	virtual void OnCollisionLeave(Collider* yours, Collider* collideWith);

	void getPos(float& x, float& y)
	{
		x = data->pos.x;
		y = data->pos.y;
	}

protected:

	ProjectileInfo* data;

	Collider* projCollider = nullptr;
	Projectile_type projType = Projectile_type::no_type;
	Animation* actualAnim = nullptr;
};

#endif
