#include "RingItem.h"
#include "ModuleRender.h"
#include "Application.h"
#include "Scene.h"
#include "ModulePrinter.h"
#include "ModuleColliders.h"
#include "Thrall.h"


RingItem::RingItem()
{
}


RingItem::~RingItem()
{
}

bool RingItem::Start()
{
	return false;
}

bool RingItem::Act(ModuleItems::ItemEvent event, float dt)
{
	return false;
}

bool RingItem::Draw()
{
	return false;
}

bool RingItem::printIconOnScreen(iPoint pos)
{
	return false;
}

Slow_Shit::Slow_Shit(Collider * temp, iPoint pos)
{
}

Slow_Shit::~Slow_Shit()
{
}
