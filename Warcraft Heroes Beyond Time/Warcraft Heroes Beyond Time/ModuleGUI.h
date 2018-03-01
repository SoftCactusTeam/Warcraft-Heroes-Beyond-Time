#ifndef __MODULEGUI_H__
#define __MODULEGUI_H__

#include "Module.h"
#include <list>
#include <string>
#include "p2Point.h"
#include "SDL/include/SDL.h"
#include "Fonts.h"

class Label;


class GUIElem
{
	friend class ModuleGUI;

public:

	enum GUIElemType
	{
		NO_ELEM_TYPE = -1,
		BUTTON,
		LABEL,
		INPUTBOX,
		IMAGE,
		SLIDER
	};

protected:

	GUIElemType type = GUIElemType::NO_ELEM_TYPE;
	GUIElem* parent = nullptr;
	fPoint position;

public:
	GUIElem() {}
	GUIElem(GUIElemType type, fPoint position, GUIElem* parent) : type(type), position(position), parent(parent) {}

	virtual ~GUIElem() {}

	virtual bool Update(float dt)
	{
		return true;
	}

};

class ModuleGUI : public Module
{
public:

	ModuleGUI();
	virtual ~ModuleGUI();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

public:

	GUIElem* createWindow(fPoint position, Label* title = nullptr, std::list<GUIElem*>* childs = nullptr, GUIElem* parent = nullptr);
	GUIElem* createButton(fPoint position, Label* Text, GUIElem* parent = nullptr);
	GUIElem* createImage(fPoint position, SDL_Rect atlasRec, GUIElem* parent = nullptr);
	GUIElem* createLabel(fPoint position, std::string text, TTF_Font* font, GUIElem* parent = nullptr);

public:

	std::list<GUIElem*> GUIElemList;
	bool spawned = false;
};




#endif