#ifndef __ITEMCONTAINER_H__
#define __ITEMCONTAINER_H__

#include "GUIElem.h"
#include "Animation.h"
#include "ModuleAudio.h"

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
	bool HandleInput(float dt);
	bool MouseHover() const;
	bool AreYouPicking() override;


	void Focus()
	{
		if (!focused)
		{
			focused = true;
			//anim = &focused_anim;
			App->audio->PlayFx(App->audio->ButtonHovered);
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