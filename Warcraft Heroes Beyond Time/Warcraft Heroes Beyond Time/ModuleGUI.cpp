#include "ModuleGUI.h"


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
	return true;
}

bool ModuleGUI::PostUpdate()
{
	return true;
}

bool ModuleGUI::CleanUp()
{
	return true;
}



//------------------------------------------------------

GUIElem* ModuleGUI::createWindow(std::string tag, fPoint position, Label* title, std::list<GUIElem*>* childs, GUIElem* parent)
{
	return &GUIElem();
	
}

GUIElem* ModuleGUI::createButton(std::string tag, fPoint position, Label* Text, GUIElem* parent)
{
	return &GUIElem();
}

GUIElem* ModuleGUI::createImage(std::string tag, fPoint position, SDL_Rect atlasRec, GUIElem* parent)
{
	return &GUIElem();
}

GUIElem* ModuleGUI::createLabel(std::string tag, fPoint position, std::string text, GUIElem* parent/*, font*/)
{
	return &GUIElem();
}