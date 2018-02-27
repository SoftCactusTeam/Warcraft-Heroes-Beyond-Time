#ifndef __MODULEGUI_H__
#define __MODULEGUI_H__

#include "Module.h"

class ModuleGUI : public Module
{
	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
};




#endif