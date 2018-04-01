#include "Stat.h"
#include "Log.h"
#include "Label.h"
#include "PlayerEntity.h"

Stat::Stat(PlayerEntity* player, fPoint localPos, GUIElem* parent, StatType stype) : GUIElem(localPos, nullptr, {0,0,0,0}, GUIElemType::STAT, parent), player(player), stype(stype){}
Stat::~Stat()
{
	DestroyChilds();
}

bool Stat::Update(float dt)
{
	Label* label = nullptr;

	std::list<GUIElem*>::iterator it;
	for (it = childs.begin(); it != childs.end(); ++it)
	{
		if ((*it)->type == GUIElemType::LABEL)
			label = (Label*)(*it);
	}

	if (label == nullptr)
	{
		LOG("No labels found inside one Stat");
		return false;
	}
	else
	{
		std::string newString;
		switch (stype)
		{
		case StatType::DAMAGE:
			newString = player->numStats.damage;
			break;
		case StatType::ARMOR:
			newString = player->numStats.armor;
			break;
		case StatType::ENERGYBYHIT:
			newString = player->numStats.energyPercentbyHit;
			break;
		case StatType::CRITCHANCE:
			newString = player->numStats.critChance;
			break;
		case StatType::SPEED:
			newString = player->numStats.speed;
			break;
		}
		label->EditText(newString);
	}
		return UpdateChilds(dt);
}

bool Stat::Draw()
{
	return DrawChilds();
}