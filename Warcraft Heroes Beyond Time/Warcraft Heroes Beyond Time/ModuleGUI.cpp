#include "ModuleGUI.h"
#include "App.h"

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



//------------------------------------------------------

GUIElem* ModuleGUI::createWindow(fPoint position, Label* title, std::list<GUIElem*>* childs, GUIElem* parent)
{
	return &GUIElem();
}

GUIElem* ModuleGUI::createButton(fPoint position, Label* Text, GUIElem* parent)
{
	return &GUIElem();
}

GUIElem* ModuleGUI::createImage(fPoint position, SDL_Rect atlasRec, GUIElem* parent)
{
	return &GUIElem();
}

GUIElem* ModuleGUI::createLabel(fPoint position, std::string text, TTF_Font* font, GUIElem* parent)
{
	Label* label = new Label(position, parent);
	label->text = text;
	label->font = font;

	GUIElemList.push_back(label);
	return label;


}