#ifndef __Console_H__
#define __Console_H__

#include "Module.h"
#include "Globals.h"
#include <vector>

class Label;

class ConsoleOrder {
public:
	std::string orderName = "";
	virtual std::string Exec() = 0;
	//virtual std::string callHelp() = 0;

private:
	bool isActive = false;
};

class Console : public Module
{
public:

	Console();

	void Init();
	bool Awake();
	bool Update(float dt);
	bool CleanUp();

	bool ExecConsoleOrder(std::string name);
	void PrintAtConsole(std::string textToPrint);
	void AddConsoleOrderToList(ConsoleOrder* consoleOrder);

private:
	std::string actualConsoleTextOrder;


	Label* text = nullptr;

	std::vector<ConsoleOrder*> consoleOrderList;
};

#endif