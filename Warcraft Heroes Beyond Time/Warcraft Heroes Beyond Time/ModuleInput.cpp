#include "p2Defs.h"
#include "Log.h"

#include "Application.h"

#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleGUI.h"

#include "Console.h"

#include "SDL/include/SDL.h"
#define MAX_KEYS 300

Input::Input() : Module()
{
	name = "input";

	keyboard = new KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KeyState) * NUM_MOUSE_BUTTONS);
	memset(jButtons, KEY_IDLE, sizeof(KeyState) * NUM_J_BUTTONS);
}

Input::~Input()
{
	delete[] keyboard;
}

bool Input::Awake(pugi::xml_node& inputNode)
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0)
	{
		LOG("SDL_GAMECONTROLLER could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if (SDL_NumJoysticks() < 1)
		LOG("Warning: No joystick detected");
	else
	{
		controller = SDL_JoystickOpen(0);
		if (controller == NULL)
		{
			LOG("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
		}
	}

	return ret;
}

bool Input::Start()
{
	SDL_StopTextInput();

	return true;
}

bool Input::PreUpdate()
{
	key_pressed = false;

	static SDL_Event event;

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			key_pressed = true;

			if (keyboard[i] == KEY_IDLE) { 
				// AQUI CADA COP QUE ES CLIQUI UNA TECLA NOVA, S'EXECUTARA UNA FUNCIÓ ON POSAR COSES EXTERNES, COM OBRIRLA CONSOLA
				keyboard[i] = KEY_DOWN;
				ExternActionsAtKeyInput(i);
			}	// ---------------------------------------------------------------------------------------------------------------
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if (mouse_buttons[i] == KEY_DOWN)
			mouse_buttons[i] = KEY_REPEAT;

		if (mouse_buttons[i] == KEY_UP)
			mouse_buttons[i] = KEY_IDLE;
	}

	for (int i = 0; i < NUM_J_BUTTONS; ++i)
	{
		if (jButtons[i] == KEY_DOWN)
			jButtons[i] = KEY_REPEAT;

		if (jButtons[i] == KEY_UP)
			jButtons[i] = KEY_IDLE;
	}

	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			windowEvents[WE_QUIT] = true;
			break;

		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
				//case SDL_WINDOWEVENT_LEAVE:
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_MINIMIZED:
			case SDL_WINDOWEVENT_FOCUS_LOST:
				windowEvents[WE_HIDE] = true;
				break;

				//case SDL_WINDOWEVENT_ENTER:
			case SDL_WINDOWEVENT_SHOWN:
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_RESTORED:
				windowEvents[WE_SHOW] = true;
				break;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			mouse_buttons[event.button.button - 1] = KEY_DOWN;
			break;

		case SDL_MOUSEBUTTONUP:
			mouse_buttons[event.button.button - 1] = KEY_UP;
			break;

		case SDL_MOUSEMOTION:
		{

			int scale = App->window->GetScale();
			mouse_motion_x = event.motion.xrel / scale;
			mouse_motion_y = event.motion.yrel / scale;
			mouse_x = event.motion.x / scale;
			mouse_y = event.motion.y / scale;
		}
			break;

		case SDL_JOYAXISMOTION:
			if (event.jaxis.which == 0)
			{
				if (event.jaxis.axis == 0)
				{
					if (event.jaxis.value < -J_DEAD_ZONE || event.jaxis.value > J_DEAD_ZONE)
						xAxis = event.jaxis.value;
					else
						xAxis = 0;
				}
				else if (event.jaxis.axis == 1)
				{
					if (event.jaxis.value < -J_DEAD_ZONE || event.jaxis.value > J_DEAD_ZONE)
						yAxis = event.jaxis.value;
					else
						yAxis = 0;
				}
			}
			break;

		case SDL_JOYBUTTONDOWN:
			if (event.jbutton.which == 0)
				jButtons[event.jbutton.button - 1] = KEY_DOWN;
			break;

		case SDL_JOYBUTTONUP:
			if (event.jbutton.which == 0)
				jButtons[event.jbutton.button - 1] = KEY_UP;

		case SDL_TEXTINPUT:
			inputText = event.text.text;
			textReady = true;

			break;
		}
	}

	return true;
}

bool Input::CleanUp()
{
	LOG("Quitting game controller");
	SDL_JoystickClose(controller);
	controller = NULL;

	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);

	return true;
}

bool Input::GetWindowEvent(EventWindow ev)
{
	return windowEvents[ev];
}

void Input::GetMousePosition(int& x, int& y)
{
	x = mouse_x;
	y = mouse_y;
}

void Input::GetMouseMotion(int& x, int& y)
{
	x = mouse_motion_x;
	y = mouse_motion_y;
}

bool Input::IsAnyKeyPressed()
{
	bool is_any_key_pressed = key_pressed;
	key_pressed = false;
	return is_any_key_pressed;
}

void Input::ExternActionsAtKeyInput(const int key) {
	// NOMES FER SERVIR EN CASOS MOLT CONCRETS I OPTIMS !!!!!!!!
	// printf_s("%i\n", key);		// PER TROBAR EL NUMERO DE LES TECLES
	switch (key) {
	case 53:	// º button -> OpenConsole
		if (App->console->isActive() == false)
			App->console->Activate();
		else
			App->console->DeActivate();
		break;
	}
}