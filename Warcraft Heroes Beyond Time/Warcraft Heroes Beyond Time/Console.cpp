#include "Console.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleGUI.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"


// ====================================================================================== //
// ===================== INSTRUCCIONS CONSOLA =========================================== //
// ====== TU LI PASSES UN STRING, ON LA PRIMERA PARAULA ES L'ORDRE, I =================== //
// ====== ACOMPANYADA D'UNA _ POTS POSAR-LI UN PARAMTETRE (NO OBLIGATORI)================ //
// ====== DESPRES POTS POSSAR UNA SEGONA _ DESPRES DEL PARAMETRE I PASAR-LI============== //
// ====== UN VALOR NUMERIC, QUE REBRA LA FUNCIO COM A ENTER (int) ======================= //
// ====================================================================================== //

Console::Console() {
	name = "console";
}

Console::~Console() {
	for (int i = 0; i < consoleOrderList.size(); i++)
		delete consoleOrderList[i];
}

bool Console::Awake() {
	uint windowsWidth;
	uint windowHeight;
	Application->window->GetWindowSize(windowsWidth, windowHeight);
	rectConsoleQuad = { 0,0, (int)windowsWidth, 20 };
	
	InputBoxInfo defInputBox;
	defInputBox.color = Black;
	defInputBox.fontName = "Arial16";

	box = Application->gui->CreateInputBox({ 0, 0 }, defInputBox, nullptr, nullptr);
	box->DisableInput();

	active = false;
}

bool Console::Awake(pugi::xml_node& AwakeNode)
{
	return true;
}


bool Console::Start() {
	box->EnableInput();
	return true;
}

bool Console::Update(float dt) {
	Application->render->DrawQuad(rectConsoleQuad, 255, 255, 255, 120);
	if (Application->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN)
		ExecConsoleOrder(box->text);
	return true;
}


bool Console::CleanUp(){
	box->ClearBox();
	box->DisableInput();
	return true;
}

bool Console::ExecConsoleOrder(std::string name){
	std::string nom_ordre;
	std::string parametre_ordre;
	int			numeric_parametre_ordre = 0;

	for (int i = 0; i < name.size(); i++) {
		if (name.at(i) == '_') {
			for (int o = 0; o < i; o++) {	//AQUI ES GENERA EL nom_ordre
				nom_ordre += name.at(o);
			}
			for (int e = i + 1; e < name.size(); e++) {	//AQUI ES GENERA EL parametre_ordre
				if (name.at(e) == '_') {
					std::string auxiliarStringNumber = "";
					for (int u = e + 1; u < name.size(); u++)
						auxiliarStringNumber += name.at(u);
					numeric_parametre_ordre = atoi(auxiliarStringNumber.c_str());
					e = name.size();
				}
				else
					parametre_ordre += name.at(e);
				
			}
			i = name.size();
		}
	}
	if (parametre_ordre.size() == 0) {
		nom_ordre = name;
		parametre_ordre = "not";
		numeric_parametre_ordre = -1;
	}

bool Console::ExecConsoleOrder(std::string name)
{
	for (int i = 0; i <= consoleOrderList.size(); i++) 
	{
		if (consoleOrderList[i]->orderName == name) 
		{
			i = consoleOrderList.size() + 1;
			consoleOrderList[i]->Exec();
	for (int i = 0; i < consoleOrderList.size(); i++) {
		if (consoleOrderList[i]->orderName() == nom_ordre) {
			consoleOrderList[i]->Exec(parametre_ordre, numeric_parametre_ordre);
			i = consoleOrderList.size() + 1;	// SURTI DEL BUCLE, POSAR UN break o continue
		}
	}
	box->ClearBox();
	return true;
}

void Console::AddConsoleOrderToList(ConsoleOrder* consoleOrder) 
{
	consoleOrderList.push_back(consoleOrder);
}
