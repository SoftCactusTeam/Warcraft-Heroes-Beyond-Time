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
	ProjectileInfo(const ProjectileInfo& info) : pos(info.pos), layer(info.layer), speed(info.speed), life(info.life), angle(info.angle), radiusToIncrease(info.radiusToIncrease) {};

	int life = 0;
	int layer = 0;
	float speed = 0.0f;
	fPoint pos = { 0.0f,0.0f };
	double angle = 0.0f;
	double radiusToIncrease = 0.0f;
};

class Projectile
{
public:

	Projectile();
	Projectile(const ProjectileInfo& info, Projectile_type type);
	virtual ~Projectile();

	virtual bool Update(float dt);
	virtual bool Draw() const;

	virtual int DecreaseLifePerTime(float dt);
	virtual fPoint RotateAround(fPoint pointToRotate, fPoint rotationPivot, double angle, double radius) const;

	virtual void OnCollision(Collider* yours, Collider* collideWith);
	virtual void OnCollisionContinue(Collider* yours, Collider* collideWith);
	virtual void OnCollisionLeave(Collider* yours, Collider* collideWith);

protected:

	ProjectileInfo data;

	Collider* projCollider = nullptr;
	Projectile_type projType = Projectile_type::no_type;
	Animation* actualAnim = nullptr;
};

#endif
