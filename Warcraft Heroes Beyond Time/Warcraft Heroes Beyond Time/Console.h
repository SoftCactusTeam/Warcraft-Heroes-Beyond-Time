#ifndef __Console_H__
#define __Console_H__

#include "Module.h"

class Console : public Module
{
public:

	Console();

	bool Awake();
	bool Update(float dt);
	bool CleanUp();

private:
	char* actualConsoleTextOrder;

private:
	bool listOfActions();

};

#endif