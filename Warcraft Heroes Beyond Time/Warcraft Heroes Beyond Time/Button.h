#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "GUIElem.h"

//atlasRect defines---------------------------

#define Button1 {64,12,226,96}
#define Button1MouseHover {293,12,225,97}
#define Button1Pressed {522,12,224,97}

//----------------------------------

class Label;

enum class BType
{
	NO_BTYPE,
	PLAY,
	GO_MMENU,
	SETTINGS,
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
	void MoveChilds(fPoint dist);
};



#endif