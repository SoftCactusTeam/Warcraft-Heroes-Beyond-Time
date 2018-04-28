#ifndef __ARCHERARROW_H__
#define __ARCHERARROW_H__

#include "Projectile.h"

// ODD_EVEN
#define TIME_ODD_EVEN 0.3f
#define RADIUS_DECREASE_ODD_EVEN 50.0f
#define DISTANCE_TO_DELETE_FROM_GULDAN_SPIRAL 20.0f
#define TIME_UNTIL_BALLS_BACK_SPIRAL 5.0f

struct ArcherArrowInfo : public ProjectileInfo
{
	ArcherArrowInfo() {};
	ArcherArrowInfo(const ArcherArrowInfo& info) : ProjectileInfo((const ProjectileInfo&)info), rotationPivot(info.rotationPivot), angle(info.angle), radiusToIncrease(info.radiusToIncrease), startRadius(info.startRadius), fel_movement(info.fel_movement) {};

	fPoint rotationPivot = { 0.0f,0.0f };
	double angle = 0.0f;
	double radiusToIncrease = 0.0f;
	float startRadius = 0.0f;

};

class ArcherArrow : public Projectile
{
public:

	ArcherArrow(const ArcherArrowInfo* info, Projectile_type type);
	~ArcherArrow();

	bool Update(float dt);
	bool Draw() const;

	void OnCollision(Collider* yours, Collider* collideWith);
	void OnCollisionContinue(Collider* yours, Collider* collideWith);
	void OnCollisionLeave(Collider* yours, Collider* collideWith);

private:

	ArcherArrowInfo * toData = nullptr;
	float timer = 0.0f;
	enum class ArrowAnimations
	{
		no_anim = -1,
		moving_anim,
		back_anim,
		max_anim
	};

	Animation arrowAnims[(unsigned int)ArrowAnimations::max_anim];
};

#endif // __ARCHERARROW_H__