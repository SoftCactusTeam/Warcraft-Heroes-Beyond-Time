#ifndef __ITEMCONTAINER_H__
#define __ITEMCONTAINER_H__

#include "GUIElem.h"
#include "Animation.h"

class Item;

class ItemContainer : public GUIElem
{
public:

	ItemContainer() {}
	ItemContainer(fPoint localPos, Module* listener, Item* item, GUIElem* parent = nullptr);
	~ItemContainer() {}

	bool Update(float dt);
	bool Draw();

private:

	TimeBasedAnimation anim;
	bool selected = false;
	Item* item = nullptr;
};

#endif