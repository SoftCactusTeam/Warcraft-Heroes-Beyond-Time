#ifndef __FELBALL_H__
#define __FELBALL_H__

#include "Projectile.h"

struct FelBallInfo : public ProjectileInfo
{
	FelBallInfo() {};
	FelBallInfo(const FelBallInfo& info) : ProjectileInfo(info.pos, info.layer, info.speed, info.life), rotationPivot(info.rotationPivot) {};

	iPoint rotationPivot = { 0,0 };
};

class FelBall : public Projectile
{
public:

	FelBall(FelBallInfo& info);
	~FelBall();

	bool Update(float dt);

	void OnCollision(Collider* yours, Collider* collideWith);
	void OnCollisionContinue(Collider* yours, Collider* collideWith);
	void OnCollisionLeave(Collider* yours, Collider* collideWith);

private:

	FelBallInfo data;

	enum class FelAnimations
	{
		no_anim = -1,
		moving_anim,
		max_anim

	};

	Animation felAnims[(unsigned int)FelAnimations::max_anim];
};

#endif