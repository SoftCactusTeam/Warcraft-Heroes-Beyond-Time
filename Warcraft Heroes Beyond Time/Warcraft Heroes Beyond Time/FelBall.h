#ifndef __FELBALL_H__
#define __FELBALL_H__

#include "Projectile.h"

// ODD_EVEN
#define TIME_ODD_EVEN 0.3f
#define RADIUS_DECREASE_ODD_EVEN 50.0f
#define DISTANCE_TO_DELETE_FROM_GULDAN_SPIRAL 20.0f
#define TIME_UNTIL_BALLS_BACK_SPIRAL 5.0f

struct FelBallInfo : public ProjectileInfo
{
	FelBallInfo() {};
	FelBallInfo(const FelBallInfo& info) : ProjectileInfo((const ProjectileInfo&)info), rotationPivot(info.rotationPivot), angle(info.angle), radiusToIncrease(info.radiusToIncrease), startRadius(info.startRadius), fel_movement(info.fel_movement) {};

	enum fel_ball_movement
	{
		no_type,
		odd_even_type,
		complete_circle,
		spiral,
		hexagon,
		incepcion,
		explosion
	} fel_movement;

	fPoint rotationPivot = { 0.0f,0.0f };
	double angle = 0.0f;
	double radiusToIncrease = 0.0f;
	float startRadius = 0.0f;

};

class FelBall : public Projectile
{
public:

	FelBall(const FelBallInfo* info, Projectile_type type);
	~FelBall();

	bool Update(float dt);
	bool Draw() const;

	void OnCollision(Collider* yours, Collider* collideWith);
	void OnCollisionContinue(Collider* yours, Collider* collideWith);
	void OnCollisionLeave(Collider* yours, Collider* collideWith);

private:

	FelBallInfo* toData = nullptr;
	float timer = 0.0f;
	bool destroyTheBall = false;
	bool slowSpeed = false;
	float timeSlowed = 0.0f;
	enum class FelAnimations
	{
		no_anim = -1,
		moving_anim,
		back_anim,
		end_anim,
		max_anim
	};

	Animation felAnims[(unsigned int)FelAnimations::max_anim];
};

#endif