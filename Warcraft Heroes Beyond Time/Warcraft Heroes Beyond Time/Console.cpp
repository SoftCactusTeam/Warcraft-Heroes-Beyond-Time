#include "Console.h"
#include "App.h"
#include "ModuleInput.h"

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

bool Console::listOfActions() {
	char* order = actualConsoleTextOrder;
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