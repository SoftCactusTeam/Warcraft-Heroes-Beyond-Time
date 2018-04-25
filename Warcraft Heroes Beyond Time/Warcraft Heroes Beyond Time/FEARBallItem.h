#ifndef _FEAR_BALL_ITEM_
#define _FEAR_BALL_ITEM_

#include "Item.h"

#define STICK_ITEM {55,179,32,32} //The rect from the texture of your gui icon

class Collider;

class FEARBallItem : public Item
{
public:
	FEARBallItem() {}
	virtual ~FEARBallItem() {}

	bool Start();
	bool Act(ModuleItems::ItemEvent event, float dt = App->dt);
	bool Draw();
	bool printIconOnScreen(iPoint pos);

	void go_up();
	void go_down();

private:
	fPoint Ball_pos = { 0,0 };
	fPoint Ball_final_pos = { 0,0 };
	uint radius = 0;
	Collider* ball_col = nullptr;
	float ball_counter = 0.0f;
	float angle = 0.0f;
	float angular_vel = 0.0f;
	float speed_growing_radius = 0.0f;
	bool ball_away = false;
};

#endif

