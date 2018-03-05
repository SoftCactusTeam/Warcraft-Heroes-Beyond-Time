#include "Console.h"
#include "App.h"
#include "ModuleInput.h"
#include "ModuleGUI.h"
#include "Label.h"

Console::Console() {
	name = "console";
}

bool Console::Awake() {
	active = false;
	return true;
}

bool Console::Update(float dt) {

	return true;
}

bool Console::CleanUp(){
	return true;
}

void Console::PrintAtConsole(std::string textToPrint) {

}

bool Console::listOfActions() {
	std::string order = actualConsoleTextOrder;
	actualConsoleTextOrder = "";

	if (order == "pause_entities") {

	}
	else if (order == "exit_game") {

	}
	else {	// EN CAS DE QUE CAP DE LES OPCIONS SIGUI BONA, ES MANTINDRA EL TEXT PER ARRECLARHO
		actualConsoleTextOrder = order;
	}

	return true;
}

void Console::AddConsoleOrderToList(ConsoleOrder* consoleOrder) {
	consoleOrderList.push_back(consoleOrder);
}