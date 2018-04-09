#ifndef __PLAYERHPBAR_H__
#define __PLAYERHPBAR_H__

#include "GUIElem.h"

#define BAR {32, 816, 250, 43}
#define BAR_BACK {66, 860, 182, 7}
#define HP_COLOR {66, 867, 182, 7}
#define ENERGY_COLOR {66, 875, 182, 7}
#define FULL_ENERGY_COLOR {66, 883, 182, 7}

class PlayerEntity;
class GUIImage;
class Label;

class PlayerHPBar : public GUIElem
{
public:

	PlayerHPBar() {}
	PlayerHPBar(PlayerEntity* player, fPoint localPos, GUIElem* parent = nullptr);
	virtual ~PlayerHPBar();

	bool Update(float dt);
	bool Draw();


private:

	fPoint hpPos = { 34, 12 };
	fPoint energyPos = { 34, 23 };
	fPoint portraitPos = { 0, 0 };
	fPoint consumablePos = { 0, 0 };

	SDL_Rect hp = HP_COLOR;
	SDL_Rect energy = ENERGY_COLOR;

	PlayerEntity* player = nullptr;
};




#endif