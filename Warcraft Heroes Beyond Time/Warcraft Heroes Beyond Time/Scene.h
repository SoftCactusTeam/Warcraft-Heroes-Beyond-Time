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

	

private:


};


#endif