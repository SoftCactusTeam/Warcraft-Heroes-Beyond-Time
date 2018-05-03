#ifndef __ITEMCONTAINER_H__
#define __ITEMCONTAINER_H__

#include "GUIElem.h"
#include "Animation.h"

class Item;
class Module;

class ItemContainer : public GUIElem
{
public:

	ItemContainer() {}
	ItemContainer(fPoint localPos, Module* listener, Item* item, GUIElem* parent = nullptr);
	~ItemContainer() {}

	bool Update(float dt);
	bool Draw();

	void Focus()
	{
		if (!focused)
		{
			focused = true;
			anim = &focused_anim;
		}
	}
	void UnFocus()
	{
		if (focused)
		{
			focused = false;
			anim = &grow_anim;
		}
	}

public:
	TimeBasedAnimation* anim = nullptr;
	TimeBasedAnimation grow_anim;
	TimeBasedAnimation select_anim;
	TimeBasedAnimation focused_anim;
private:
	bool selected = false;
	Item* item = nullptr;
};

#endif