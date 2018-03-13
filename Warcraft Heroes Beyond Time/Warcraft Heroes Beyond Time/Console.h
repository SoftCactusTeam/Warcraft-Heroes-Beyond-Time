#ifndef __Console_H__
#define __Console_H__

#include "Module.h"
#include "Globals.h"
#include "InputBox.h"
#include <vector>
#include "SDL_image\include\SDL_image.h"

class Label;

class ConsoleOrder {
public:

	virtual std::string orderName() = 0;
	virtual void Exec(std::string parameter, int parameterNumeric) = 0;		// Return a string

private:

	bool isActive = false;
};

class Console : public Module
{
public:

	Console();
	~Console();

	bool Awake(pugi::xml_node& consoleNode);
	bool Start();
	bool Update(float dt);
	bool CleanUp();

	bool ExecConsoleOrder(std::string name);
	void AddConsoleOrderToList(ConsoleOrder* consoleOrder);

private:

	std::vector<ConsoleOrder*> consoleOrderList;
	InputBox* box = nullptr;

	SDL_Rect rectConsoleQuad = {0,0,0,0};
};

#endif