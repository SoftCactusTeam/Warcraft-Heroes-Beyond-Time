#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "GUIElem.h"

//atlasRect defines---------------------------

#define Button1 {38,30,158,42}
#define Button1MouseHover {205,30,158,42}
#define Button1Pressed {374,30,158,42}

//----------------------------------

class Label;

enum class BType
{
	NO_BTYPE,
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
};



#endif