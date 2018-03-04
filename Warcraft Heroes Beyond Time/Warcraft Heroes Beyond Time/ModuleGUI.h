#ifndef __MODULEGUI_H__
#define __MODULEGUI_H__

#include "Module.h"
#include <list>
#include <map>
#include "p2Point.h"
#include "SDL/include/SDL.h"
#include "Fonts.h"

class GUIElem;
class Label;
class InputBox;
class LabelInfo;
class InputBoxInfo;

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
	Label* CreateLabel(iPoint position, LabelInfo& info, GUIElem* parent = nullptr, Module* listener = nullptr);
	InputBox* CreateInputBox(iPoint position, InputBoxInfo& info, GUIElem* parent = nullptr, Module* listener = nullptr);
	bool DestroyElem(GUIElem* element);

public:
	std::list<GUIElem*> GUIElemList;
	std::list<GUIElem*> elementsToSpawn;
	std::list<GUIElem*> elementsToKill;
};




#endif