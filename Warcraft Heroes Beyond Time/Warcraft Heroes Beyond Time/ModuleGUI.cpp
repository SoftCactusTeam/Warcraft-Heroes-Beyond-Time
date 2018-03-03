#include "ModuleGUI.h"
#include "App.h"
#include "Fonts.h"

#include "Label.h"

ModuleGUI::ModuleGUI() : Module()
{
	name = "GUI";
}

ModuleGUI::~ModuleGUI() {}


bool ModuleGUI::Awake()
{
	return true;
}

bool ModuleGUI::Start()
{
	return true;
}

bool ModuleGUI::PreUpdate()
{
	return true;
}

bool ModuleGUI::Update(float dt)
{
	bool result = true;

	std::list<GUIElem*>::iterator it;
	for (it = GUIElemList.begin(); it != GUIElemList.end() && result; ++it)
	{
		result = (*it)->Update(dt);
	}

	return result;
}

bool ModuleGUI::PostUpdate()
{
	return true;
}

bool ModuleGUI::CleanUp()
{
	std::list<GUIElem*>::reverse_iterator it;
	for (it = GUIElemList.rbegin(); it != GUIElemList.rend(); ++it)
	{
		delete (*it);
	}
	GUIElemList.clear();
	return true;
}

Label* ModuleGUI::CreateLabel(iPoint position, LabelInfo& info, GUIElem* parent, Module* listener)
{
	Label* label = new Label(position, info, parent, listener);

	GUIElemList.push_back(label);

	return label;
}