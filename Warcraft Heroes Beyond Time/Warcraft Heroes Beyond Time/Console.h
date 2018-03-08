#ifndef __Console_H__
#define __Console_H__

#include "Module.h"
#include "Globals.h"
#include "InputBox.h"
#include <vector>
#include "SDL_image\include\SDL_image.h"

class ConsoleOrder {
public:
	virtual std::string orderName() = 0;
	virtual void Exec() = 0;		// FER QUE RETORNI UN STRING
private:
	bool isActive = false;
};

class Console : public Module
{
public:

	Console();
	~Console();

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

	SDL_Rect rectConsoleQuad = {0,0,0,0};
};

#endif