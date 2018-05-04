#ifndef __THUNDER_H__
#define __THUNDER_H__

#include "Projectile.h"

struct ThunderInfo : public ProjectileInfo
{
	ThunderInfo() {};
	ThunderInfo(const ThunderInfo& info) : ProjectileInfo((const ProjectileInfo&)info) {};

};

class Thunder : public Projectile
{
public:

	Thunder(const ThunderInfo* info, Projectile_type type);
	~Thunder();

	bool Update(float dt);
	bool Draw() const;

	void OnCollision(Collider* yours, Collider* collideWith);
	void OnCollisionContinue(Collider* yours, Collider* collideWith);
	void OnCollisionLeave(Collider* yours, Collider* collideWith);

private:

	ThunderInfo * toData = nullptr;

	enum class ThunderAnimations
	{
		no_anim = -1,
		cast,
		max_anim
	};

	Animation thunderAnims[(unsigned int)ThunderAnimations::max_anim];
};

#endif