#ifndef __ITEMCONTAINER_H__
#define __ITEMCONTAINER_H__

#include "GUIElem.h"

class Item;

class ItemContainer : public GUIElem
{
public:

	ItemContainer() {}
	ItemContainer(fPoint localPos, Module* listener, SDL_Rect atlasRect, GUIElem* parent = nullptr);
	~ItemContainer() {}

	bool Update(float dt);
	bool Draw();

private:
	bool selected = false;
	Item* item = nullptr;
};

#endif