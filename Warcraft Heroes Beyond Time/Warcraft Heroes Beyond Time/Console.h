#ifndef __Console_H__
#define __Console_H__

#include "Module.h"
#include "Globals.h"
#include <vector>

class ConsoleOrder {
public:
	char* retName = "";
	virtual char* Exec() = 0;
	virtual char* callHelp() = 0;
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
	void AddConsoleOrderToList(ConsoleOrder* consoleOrder);

private:
	std::string actualConsoleTextOrder;
	bool listOfActions();
	Label* text = nullptr;

	std::vector<ConsoleOrder*> consoleOrderList;
};

#endif