#include "Console.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleGUI.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleEntitySystem.h"
#include "PlayerEntity.h"
#include "Scene.h"


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

	std::vector<ConsoleOrder*>::iterator it;
	for (it = consoleOrderVector.begin(); it != consoleOrderVector.end(); ++it)
		delete *it;
	consoleOrderVector.clear();
}

void Console::Init()
{
	active = false;
}

bool Console::Awake(pugi::xml_node& consoleNode) 
{
	writting = false;
	return true;
}

bool Console::Start() 
{
	
	uint windowsWidth;
	uint windowHeight;
	App->window->GetWindowSize(windowsWidth, windowHeight);

	rectConsoleQuad = { 0, 0, 640, 17 };

	InputBoxInfo defInputBox;
	defInputBox.color = White;
	defInputBox.fontName = "Arial11";

	box = (InputBox*)App->gui->CreateInputBox({ 5, 0 }, defInputBox, nullptr, nullptr);

	return true;
}

bool Console::Update(float dt) 
{
	if (writting)
	{
		rectConsoleQuad.x = - App->render->camera.x;
		rectConsoleQuad.y = - App->render->camera.y;
		App->render->DrawQuad(rectConsoleQuad, 0, 0, 0, 200);
		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN)
		{
			ExecConsoleOrder(box->text);
		}
	}
	
	return true;
}


bool Console::CleanUp()
{
	return true;
}

bool Console::ExecConsoleOrder(std::string name) 
{
	std::string nom_ordre;
	std::string parametre_ordre;
	int			numeric_parametre_ordre = 0;

	for (int i = 0; i < name.size(); i++) 
	{
		if (name.at(i) == '_') 
		{
			for (int o = 0; o < i; o++) //AQUI ES GENERA EL nom_ordre
			{	
				nom_ordre += name.at(o);
			}

			for (int e = i + 1; e < name.size(); e++) //AQUI ES GENERA EL parametre_ordre
			{	
				if (name.at(e) == '_') 
				{
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
	if (parametre_ordre.size() == 0) 
	{
		nom_ordre = name;
		parametre_ordre = "not";
		numeric_parametre_ordre = -1;
	}

	for (int i = 0; i < consoleOrderVector.size(); i++) 
	{
		if (consoleOrderVector[i]->orderName() == nom_ordre) 
		{
			consoleOrderVector[i]->Exec(parametre_ordre, numeric_parametre_ordre);
			break;
		}
	}

	box->ClearBox();
	return true;
}

void Console::AddConsoleOrderToList(ConsoleOrder* consoleOrder) 
{
	consoleOrderVector.push_back(consoleOrder);
}

void Console::SwitchWrittingState()
{
	writting = !writting;
	if (writting)
	{
		box->EnableInput();
		App->scene->player->Walk(false);
	}
		
	else
	{
		box->DisableInput();
		box->ClearBox();
		App->scene->player->Walk(true);
	}
		
}