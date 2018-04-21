#ifndef __FELBALL_H__
#define __FELBALL_H__

#include "Projectile.h"

struct FelBallInfo : public ProjectileInfo
{
	FelBallInfo() {};
	FelBallInfo(const FelBallInfo& info) : ProjectileInfo((const ProjectileInfo&)info), rotationPivot(info.rotationPivot) {};

	iPoint rotationPivot = { 0,0 };
};

class FelBall : public Projectile
{
public:

	FelBall(const FelBallInfo& info, Projectile_type type);
	~FelBall();

	bool Update(float dt);
	bool Draw() const;

	void OnCollision(Collider* yours, Collider* collideWith);
	void OnCollisionContinue(Collider* yours, Collider* collideWith);
	void OnCollisionLeave(Collider* yours, Collider* collideWith);

private:

	FelBallInfo* toData = nullptr;
	float angle = 0;

	enum class FelAnimations
	{
		no_anim = -1,
		moving_anim,
		max_anim

	};

	Animation felAnims[(unsigned int)FelAnimations::max_anim];
};

#endif