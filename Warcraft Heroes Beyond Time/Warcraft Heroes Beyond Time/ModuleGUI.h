#ifndef __MODULEGUI_H__
#define __MODULEGUI_H__

#include "Module.h"
#include <list>
#include <map>
#include "p2Point.h"
#include "SDL/include/SDL.h"
#include "Fonts.h"


class GUIElem;
class LabelInfo;
class InputBoxInfo;
struct SliderInfo;
enum class BType;

class PlayerEntity;


class ModuleGUI : public Module
{
public:

	ModuleGUI();
	virtual ~ModuleGUI();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

public:
	GUIElem* CreateLabel(fPoint position, LabelInfo& info, GUIElem* parent = nullptr, Module* listener = nullptr);
	GUIElem* CreateInputBox(fPoint localPos, InputBoxInfo& info, Module* listener = nullptr, GUIElem* parent = nullptr);
	GUIElem* CreateButton(fPoint localPos, BType btype, Module* listener, GUIElem* parent = nullptr);
	GUIElem* CreateSlider(fPoint localPos, SliderInfo sInfo, Module* listener, GUIElem* parent = nullptr);
	GUIElem* CreateGUIWindow(fPoint localPos, SDL_Rect type, Module* listener, GUIElem* parent = nullptr);
	GUIElem* CreateGUIImage(fPoint localPos, SDL_Rect atlasRect, Module* listener, GUIElem* parent = nullptr);
	GUIElem* CreateStatsWindow(PlayerEntity* player, fPoint localPos);
	GUIElem* CreateHPBar(PlayerEntity* player, fPoint localPos);

	bool DestroyElem(GUIElem* element);
	SDL_Texture* getAtlas() const;

public:
	std::list<GUIElem*> GUIElemList;
	std::list<GUIElem*> elementsToSpawn;
	std::list<GUIElem*> elementsToKill;

private:
	SDL_Texture* atlas = nullptr;
};




#endif