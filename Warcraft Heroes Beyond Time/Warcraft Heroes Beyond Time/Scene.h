#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"


class Scene : public Module
{
public:

	Scene();
	virtual ~Scene();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

public:

	virtual bool OnUIEvent(GUIElem* UIelem, UIEvents _event);
	

public:
	enum class Stages
	{
		NO_STAGE,
		MAIN_MENU, 
		INGAME

	} actual_scene = Stages::MAIN_MENU;


private:
	bool restart = false;
};


#endif