#ifndef __MODULEGUI_H__
#define __MODULEGUI_H__

#include "Module.h"
#include <list>
#include <string>
#include "p2Point.h"
#include "SDL/include/SDL.h"

class Label;


class GUIElem
{
	friend class ModuleGUI;
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
	std::string name = "Default";
	fPoint position;

public:

	//Methods here

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

	GUIElem* createWindow(std::string tag, fPoint position, Label* title = nullptr, std::list<GUIElem*>* childs = nullptr, GUIElem* parent = nullptr);
	GUIElem* createButton(std::string tag, fPoint position, Label* Text, GUIElem* parent = nullptr);
	GUIElem* createImage(std::string tag, fPoint position, SDL_Rect atlasRec, GUIElem* parent = nullptr);
	GUIElem* createLabel(std::string tag, fPoint position, std::string text, GUIElem* parent = nullptr/*, font*/);

private:

	std::list<GUIElem*> GUIElemList;

};




#endif