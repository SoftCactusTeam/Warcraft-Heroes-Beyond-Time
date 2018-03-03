#include "Console.h"
#include "App.h"
#include "ModuleInput.h"

Console::Console() {

}

bool Console::Awake() {

}

bool Console::Update() {
	if (Application->input->GetKey(SDL_SCANCODE_EXECUTE) == KeyState::KEY_DOWN)
		listOfActions();
	if (Application->input->GetKey(SDL_SCANCODE_ESCAPE) == KeyState::KEY_DOWN || Application->input->GetKey(SDL_SCANCODE_F12) == KeyState::KEY_DOWN) {
		CleanUp();
		active = false;
	}
}

bool Console::CleanUp(){

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
}