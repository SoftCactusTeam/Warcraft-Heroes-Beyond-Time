#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "GUIElem.h"

//atlasRect defines---------------------------

#define Button1 {144,27,501,214}
#define Button1MouseHover {653,27,501,216}
#define Button1Pressed {1162,27,501,191}

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

};



#endif