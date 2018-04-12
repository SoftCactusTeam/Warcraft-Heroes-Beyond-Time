#ifndef __BOSSHPBAR_H__
#define __BOSSHPBAR_H__

#include "GUIElem.h"

class BossEntity;

class BossHPBar : public GUIElem
{
public:

	BossHPBar() {}
	BossHPBar(BossEntity* boss, fPoint localPos, GUIElem* parent = nullptr);
	virtual ~BossHPBar() {}

	bool Update(float dt);
	bool Draw();

private:
	BossEntity* boss = nullptr;
	SDL_Rect actualrect = { 301, 841, 310, 8 };

};



#endif