#include "ModuleGUI.h"
#include "Application.h"
#include "Fonts.h"

#include "Label.h"
#include "InputBox.h"
#include "GUIElem.h"

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
	if (elementsToSpawn.size() > 0)
	{
		std::list<GUIElem*>::iterator it;
		for (it = elementsToSpawn.begin(); it != elementsToSpawn.end(); ++it)
		{
			if ((*it)->hasParent())
				(*it)->getParent()->addChild(*it);
			else
				GUIElemList.push_back(*it);
		}
		elementsToSpawn.clear();
	}
	return elementsToSpawn.size() <= 0;
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
	if (elementsToKill.size() > 0)
	{
		std::list<GUIElem*>::iterator it;
		for (it = elementsToKill.begin(); it != elementsToKill.end(); ++it)
		{
			GUIElemList.remove(*it);
		}
		elementsToKill.clear();
	}
	return elementsToKill.size() <= 0;
}

bool ModuleGUI::CleanUp()
{
	std::list<GUIElem*>::reverse_iterator it;
	for (it = GUIElemList.rbegin(); it != GUIElemList.rend(); ++it)
	{
		delete (*it);
	}
	GUIElemList.clear();

	return GUIElemList.size() <= 0;
}

//----------------------------------------------------------------------------------------------------//

bool ModuleGUI::DestroyElem(GUIElem* element)
{
	elementsToKill.push_back(element);
	return true;
}

Label* ModuleGUI::CreateLabel(fPoint position, LabelInfo& info, GUIElem* parent, Module* listener)
{
	Label* label = new Label(position, info, parent, listener);

	elementsToSpawn.push_back((GUIElem*)label);

	return label;
}

InputBox* ModuleGUI::CreateInputBox(fPoint localPos, InputBoxInfo& info, Module* listener, GUIElem* parent)
{
	InputBox* inputBox = new InputBox(localPos, info, listener, parent);

	elementsToSpawn.push_back((GUIElem*)inputBox);

	return inputBox;
}

SDL_Texture* ModuleGUI::getAtlas() const
{
	return atlas;
}