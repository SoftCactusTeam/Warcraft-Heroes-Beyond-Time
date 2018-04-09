#include "ModuleGUI.h"
#include "Application.h"
#include "Fonts.h"
#include "ModuleTextures.h"
#include "PlayerEntity.h"

#include "GUIElem.h"
#include "Label.h"
#include "InputBox.h"
#include "Button.h"
#include "Slider.h"
#include "GUIWindow.h"
#include "GUIImage.h"
#include "Stat.h"
#include "PlayerHPBar.h"

#include "Brofiler\Brofiler.h"

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
			std::list<GUIElem*>::iterator it_b;
			for (it_b = GUIElemList.begin(); it_b != GUIElemList.end(); ++it_b)
			{
				if ((*it_b) == (*it))
				{
					delete (*it_b);
					GUIElemList.erase(it_b);
					break;
				}
			}
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
	BROFILER_CATEGORY("ClearGUI", Profiler::Color::Chocolate);
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


	/*elementsToSpawn.push_back((GUIElem*)inputBox);*/

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
	GUIWindow* window = new GUIWindow(localPos, type, parent, listener);
	elementsToSpawn.push_back(window);
	return window;
}

GUIElem* ModuleGUI::CreateGUIImage(fPoint localPos, SDL_Rect atlasRect, Module* listener, GUIElem* parent)
{
	GUIImage* image = new GUIImage(localPos, atlasRect, listener, parent);
	elementsToSpawn.push_back(image);
	return image;
}

GUIElem* ModuleGUI::CreateStatsWindow(PlayerEntity* player, fPoint localPos)
{
	//GUIWindow* statswindow = (GUIWindow*)CreateGUIWindow(localPos, WINDOWFROMATLAS, nullptr, nullptr);

	//Recolocar los stats en funcion de la window
	/*Stat* damage = new Stat(player, localPos, statswindow, Stat::StatType::DAMAGE); 
	Stat* armor = new Stat(player, localPos, statswindow, Stat::StatType::ARMOR); 
	Stat* energybyhit = new Stat(player, localPos, statswindow, Stat::StatType::ENERGYBYHIT); 
	Stat* speed = new Stat(player, localPos, statswindow, Stat::StatType::SPEED); 
	Stat* critchance = new Stat(player, localPos, statswindow, Stat::StatType::CRITCHANCE);*/ 

	//Recolocar los iconos respecto a su stat
	/*GUIImage* damageIcon = new GUIImage(localPos, ICONRECT, nullptr, damage);
	GUIImage* armorIcon = new GUIImage(localPos, ICONRECT, nullptr, armor);
	GUIImage* energybyhitIcon = new GUIImage(localPos, ICONRECT, nullptr, energybyhit);
	GUIImage* speedIcon = new GUIImage(localPos, ICONRECT, nullptr, speed);
	GUIImage* critchanceIcon = new GUIImage(localPos, ICONRECT, nullptr, critchance);*/

	/*LabelInfo LabelInfo;
	LabelInfo.color = White;
	LabelInfo.fontName = "Arial80";
	LabelInfo.text = "0";

	//Recolocar las labels respecto a su stat
	Label* damageLabel = new Label(localPos, LabelInfo, damage, nullptr);
	Label* armorLabel = new Label(localPos, LabelInfo, armor, nullptr);
	Label* energybyhitLabel = new Label(localPos, LabelInfo, energybyhit, nullptr);
	Label* speedLabel = new Label(localPos, LabelInfo, speed, nullptr);
	Label* critchanceLabel = new Label(localPos, LabelInfo, critchance, nullptr);*/
	
	//return statswindow;

	return &GUIElem();
}

GUIElem* ModuleGUI::CreateHPBar(PlayerEntity* player, fPoint localPos)
{
	PlayerHPBar* hpbar = new PlayerHPBar(player, localPos, nullptr);
	elementsToSpawn.push_back(hpbar);
	return hpbar;
}

SDL_Texture* ModuleGUI::getAtlas() const
{
	return atlas;
}
