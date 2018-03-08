#include "Console.h"
#include "App.h"
#include "ModuleInput.h"
#include "ModuleGUI.h"

Console::Console() {
	name = "console";
}

bool Console::Awake() {
	InputBoxInfo defInputBox;
	defInputBox.color = Blue;
	defInputBox.fontName = "Arial16";

	box = Application->gui->CreateInputBox({ 0, 50 }, defInputBox, nullptr, nullptr);
	box->DisableInput();

	active = false;
	return true;
}

bool Console::Start() {
	box->EnableInput();
	return true;
}

bool Console::Update(float dt) {
	if (Application->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN)
		ExecConsoleOrder(box->text);
	return true;
}

bool Console::CleanUp(){
	box->ClearBox();
	box->DisableInput();
	return true;
}

void Console::PrintAtConsole(std::string textToPrint) {

}

bool Console::ExecConsoleOrder(std::string name){
	for (int i = 0; i < consoleOrderList.size(); i++) {
		if (consoleOrderList[i]->orderName() == name) {
			consoleOrderList[i]->Exec();
			i = consoleOrderList.size() + 1;	// SURTI DEL BUCLE, POSAR UN break o continue
		}
	}
	box->ClearBox();
	return true;
}

void Console::AddConsoleOrderToList(ConsoleOrder* consoleOrder) {
	consoleOrderList.push_back(consoleOrder);
}