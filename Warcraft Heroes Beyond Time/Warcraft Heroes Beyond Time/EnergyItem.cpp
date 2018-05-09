#include "EnergyItem.h"



EnergyItem::EnergyItem()
{
}


EnergyItem::~EnergyItem()
{
}

bool EnergyItem::Start()
{
	return true;
}

bool EnergyItem::Act(ModuleItems::ItemEvent event, float dt)
{
	switch (event)
	{
	case ModuleItems::ItemEvent::PLAYER_HITTED:


	}
	return true;
}

bool EnergyItem::Draw()
{
	return true;
}

bool EnergyItem::printIconOnScreen(iPoint pos)
{
	return true;
}
