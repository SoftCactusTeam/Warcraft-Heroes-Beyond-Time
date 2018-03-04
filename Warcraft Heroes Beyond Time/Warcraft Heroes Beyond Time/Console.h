#ifndef __Console_H__
#define __Console_H__

#include "Module.h"
#include "Globals.h"

class Console : public Module
{
public:

	Console();

	bool Awake();
	bool Update(float dt);
	bool CleanUp();

	void PrintAtConsole(std::string textToPrint);

private:
	std::string actualConsoleTextOrder;

private:
	bool listOfActions();
	Label* text = nullptr;
};

#endif