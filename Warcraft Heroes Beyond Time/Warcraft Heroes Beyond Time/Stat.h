#ifndef __STAT_H__
#define __STAT_H__

#include "GUIElem.h"

class PlayerEntity;
class GUIImage;
class Label;

class Stat : public GUIElem
{
public:

	enum class StatType
	{
		NO_TYPE = -1,
		DAMAGE,
		ARMOR,
		ENERGYBYHIT,
		SPEED,
		CRITCHANCE
	}stype;

	Stat() {}
	Stat(PlayerEntity* player, fPoint localPos, GUIElem* parent = nullptr, StatType stype = StatType::NO_TYPE);
	virtual ~Stat();

	bool Update(float dt);
	bool Draw();


private:

	PlayerEntity * player = nullptr;
};




#endif