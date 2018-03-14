#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "GUIElem.h"

//atlasRect defines---------------------------

#define Button1 {64,12,226,96}
#define Button1MouseHover {293,12,225,97}
#define Button1Pressed {522,12,224,97}

//----------------------------------

class Label;

class Button : public GUIElem
{
public:

public:
	Button() {}
	Button(fPoint localPos, GUIElem* parent = nullptr, Module* listener = nullptr);

	virtual ~Button();

public:
	bool Update(float dt);
	void MoveChilds(fPoint dist);
};



#endif