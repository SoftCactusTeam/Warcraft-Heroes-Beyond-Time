#ifndef _FREEZE_BALL_ITEM_
#define _FREEZE_BALL_ITEM_

#include "Item.h"
#include <memory>

#define GIMLI_ICON {178,176,16,16} //The rect from the texture of your gui icon
#define GIMLI_INGAME {178,176,16,16}

class Collider;

class FreezeBallItem : public Item
{
public:
	FreezeBallItem() {}
	virtual ~FreezeBallItem() {}

	bool Start();
	bool Act(ModuleItems::ItemEvent event, float dt = App->dt);
	bool Draw();
	bool printYourStuff(iPoint pos);

private:
	fPoint Ball_pos = { 0,0 };
	fPoint Ball_final_pos = { 0,0 };
	uint radius = 50;
	std::weak_ptr<Collider*> ball_col;
	float ball_counter = 0.0f;
	float angle = 0.0f;
	float angular_vel = 0.0f;

	std::string softDescription = "\"Stop there!\"";
	std::string Title = "Im an item";
};

#endif
