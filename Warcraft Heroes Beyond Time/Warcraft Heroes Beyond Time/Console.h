#ifndef __Console_H__
#define __Console_H__

#include "Module.h"
#include "Globals.h"
#include <vector>

class ConsoleOrder {
public:
	char* retName = "";
	virtual void Exec() = 0;
	virtual void callHelp() = 0;
private:
	bool isActive = false;
};

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
	bool listOfActions();
	Label* text = nullptr;

	std::vector<ConsoleOrder> orderList;
};

#endif