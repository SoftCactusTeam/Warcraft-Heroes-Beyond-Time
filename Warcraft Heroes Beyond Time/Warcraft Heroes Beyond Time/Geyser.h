#ifndef __GEYSER_H__
#define __GEYSER_H__

#include "Projectile.h"

struct GeyserInfo : public ProjectileInfo
{
	GeyserInfo() {};
	GeyserInfo(const GeyserInfo& info) : ProjectileInfo((const ProjectileInfo&)info) {};
};

class Geyser : public Projectile
{
public:

	Geyser(const GeyserInfo* info, Projectile_type type);
	~Geyser();

	bool Update(float dt);
	bool Draw() const;

	void OnCollision(Collider* yours, Collider* collideWith);
	void OnCollisionContinue(Collider* yours, Collider* collideWith);
	void OnCollisionLeave(Collider* yours, Collider* collideWith);

private:

	GeyserInfo* toData = nullptr;

	enum class GeyserAnimations
	{
		no_anim = -1,
		moving_anim,
		max_anim
	};

	Animation geyAnims[(unsigned int)GeyserAnimations::max_anim];
};

#endif