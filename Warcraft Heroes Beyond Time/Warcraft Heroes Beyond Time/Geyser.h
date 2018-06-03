#ifndef __GEYSER_H__
#define __GEYSER_H__

#include "Projectile.h"

#define TIMEFOLLOWINGPLAYER 2.0f
#define TIMEUNTILEXPLODE 0.4f

#define TIMEUNTILSTOPGEYSEREXPLODE 0.8f

struct GeyserInfo : public ProjectileInfo
{
	GeyserInfo() {};
	GeyserInfo(const GeyserInfo& info) : ProjectileInfo((const ProjectileInfo&)info), geyser_movement(info.geyser_movement) {};

	enum Geyser_style
	{
		no_type,
		follow_player,
		stay_in_pos
	} geyser_movement;

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

	float timeUntilExplode = 0.0f;

	enum class GeyserAnimations
	{
		no_anim = -1,
		selection,
		explosion,
		following,
		max_anim
	};

	enum class state
	{
		no_state,
		following,
		stop,
		explosion
	} geyser_state;

	Animation geyAnims[(unsigned int)GeyserAnimations::max_anim];
};

#endif