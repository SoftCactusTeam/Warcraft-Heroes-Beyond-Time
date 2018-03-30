#include "ModuleGUI.h"
#include "Application.h"
#include "Fonts.h"
#include "ModuleTextures.h"

#include "GUIElem.h"
#include "Label.h"
#include "InputBox.h"
#include "Button.h"
#include "Slider.h"
#include "GUIWindow.h"

ModuleGUI::ModuleGUI() : Module()
{
	name = "GUI";
}

ModuleGUI::~ModuleGUI() {}

bool ModuleGUI::Awake(pugi::xml_node& guiNode)
{
	return true;
}

bool ModuleGUI::Start()
{
	atlas = App->textures->Load("GUI/atlas.png");
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
	bool ret = true;

	if (elementsToKill.size() > 0)
	{
		std::list<GUIElem*>::iterator it;
		for (it = elementsToKill.begin(); it != elementsToKill.end(); ++it)
		{
			GUIElemList.remove(*it);
		}
		elementsToKill.clear();
	}

	ret = elementsToKill.size() <= 0;

	if (ret)
	{
		std::list<GUIElem*>::iterator it;
		for (it = GUIElemList.begin(); it != GUIElemList.end() && ret; ++it)
		{
			ret = (*it)->Draw();
		}

	}

	return ret;
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

GUIElem* ModuleGUI::CreateLabel(fPoint position, LabelInfo& info, GUIElem* parent, Module* listener)
{
	Label* label = new Label(position, info, parent, listener);

	elementsToSpawn.push_back((GUIElem*)label);

	return label;
}

GUIElem* ModuleGUI::CreateInputBox(fPoint localPos, InputBoxInfo& info, Module* listener, GUIElem* parent)
{
	InputBox* inputBox = new InputBox(localPos, info, listener, parent);

	elementsToSpawn.push_back((GUIElem*)inputBox);

	return inputBox;
}

GUIElem* ModuleGUI::CreateButton(fPoint localPos, BType btype, Module* listener, GUIElem* parent)
{
	Button* button = new Button(localPos, btype, parent, listener);
	elementsToSpawn.push_back(button);
	return button;
}

GUIElem* ModuleGUI::CreateSlider(fPoint localPos, SliderInfo sInfo, Module* listener, GUIElem* parent)
{
	Slider* slider = new Slider(localPos, listener, sliderbar, sInfo, GUIElem::GUIElemType::SLIDER, parent);
	elementsToSpawn.push_back(slider);
	return slider;
}

GUIElem* ModuleGUI::CreateGUIWindow(fPoint localPos, SDL_Rect type, Module* listener, GUIElem* parent)
{
	GUIWindow* window = new GUIWindow(localPos, StoneWindow, parent, listener);
	elementsToSpawn.push_back(window);
	return window;
}

SDL_Texture* ModuleGUI::getAtlas() const
{
	return atlas;
}
