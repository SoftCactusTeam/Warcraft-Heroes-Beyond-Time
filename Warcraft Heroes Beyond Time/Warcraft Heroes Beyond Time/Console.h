#ifndef __Console_H__
#define __Console_H__

#include "Module.h"
#include "Globals.h"
#include <vector>

#include "InputBox.h"

class ConsoleOrder {
public:
	std::string orderName = 0;
	virtual void Exec() = 0;		// FER QUE RETORNI UN STRING
private:
	bool isActive = false;
};

class Console : public Module
{
public:

	Console();

	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();

	bool ExecConsoleOrder(std::string name);
	void PrintAtConsole(std::string textToPrint);
	void AddConsoleOrderToList(ConsoleOrder* consoleOrder);

private:
	std::vector<ConsoleOrder*> consoleOrderList;
	InputBox* box = nullptr;
};

#endif