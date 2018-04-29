#ifndef _DMG_BALL_ITEM_
#define _DMG_BALL_ITEM_

#include "Item.h"

#define CLOACK_ICON {158,176,16,16} //The rect from the texture of your gui icon

class Collider;

class DMGBallItem : public Item
{
public:
	DMGBallItem() {}
	virtual ~DMGBallItem() {}

	bool Start();
	bool Act(ModuleItems::ItemEvent event, float dt = App->dt);
	bool Draw();
	bool printIconOnScreen(iPoint pos);

private:
	fPoint Ball_pos = { 0,0 };
	fPoint Ball_final_pos = { 0,0 };
	uint radius = 50;
	Collider* ball_col = nullptr;
	float ball_counter = 0.0f;
	float angle = 0.0f;
	float angular_vel = 0.0f;
};

#endif
