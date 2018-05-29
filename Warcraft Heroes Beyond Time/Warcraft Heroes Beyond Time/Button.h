#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "GUIElem.h"
#include "FileSystem.h"

//atlasRect defines---------------------------

#define Button1 {38,29,158,42}
#define Button1MouseHover {205,29,158,42}
#define Button1Pressed {374,29,158,42}

//----------------------------------

class Label;

enum class BType
{
	NO_BTYPE,
	CONTINUE,
	PLAY,
	GO_MMENU,
	SETTINGS,
	RESUME,
	EXIT_GAME

};

class Button : public GUIElem
{
public:
	BType btype = BType::NO_BTYPE;

public:
	Button() {}
	Button(fPoint localPos, BType btype, GUIElem* parent = nullptr, Module* listener = nullptr);

	virtual ~Button();

public:
	bool Update(float dt);
	bool Draw();
	void MoveChilds(fPoint dist);
	void Focus()
	{
		if (!focused)
		{
			if (btype != BType::CONTINUE || App->fs->isGameSaved())
			{
				focused = true;
				atlasRect = Button1MouseHover;
			}
		}
	}
	void UnFocus()
	{
		if (focused)
		{
			if (btype != BType::CONTINUE || App->fs->isGameSaved())
			{
				focused = false;
				atlasRect = Button1;
			}
		}
	}
};



#endif