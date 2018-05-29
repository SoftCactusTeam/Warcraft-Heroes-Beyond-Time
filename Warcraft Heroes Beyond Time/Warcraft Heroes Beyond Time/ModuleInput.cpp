#include "p2Defs.h"
#include "Log.h"

#include "Application.h"

#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "Label.h"
#include "ModuleGUI.h"

#include "Console.h"


#include "SDL/include/SDL.h"

#define MAX_KEYS 300


static const float axisCD = 0.3f;

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
	
	if (SDL_InitSubSystem(SDL_INIT_HAPTIC) < 0)
	{
		printf_s("SDL_HAPTIC could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	InitController();

	return ret;
}

void Input::InitController()
{
	if (SDL_NumJoysticks() < 1)
		LOG("Warning: No joystick detected");
	else
	{
		joystick = SDL_JoystickOpen(0);
		if (joystick == NULL)
		{
			LOG("Warning: Unable to open joystick! SDL Error: %s", SDL_GetError());
		}
		else
		{
			controllerHaptic = SDL_HapticOpenFromJoystick(joystick);
			if (controllerHaptic == NULL)
			{
				LOG("Warning: Controller does not support haptics! SDL Error: %s", SDL_GetError());
			}
			else
			{
				//Get initialize rumble
				if (SDL_HapticRumbleInit(controllerHaptic) < 0)
				{
					LOG("Warning: Unable to initialize rumble! SDL Error: %s", SDL_GetError());
				}
			}

			controller = SDL_GameControllerOpen(0);
			if (controller == NULL)
				LOG("Warning: Couldn't initialize the controller! SDL Error: %s", SDL_GetError());
			else
			{
				DEFAULTbindingMap.insert(std::pair<char*, uint>("Attack", SDL_CONTROLLER_BUTTON_X));
				DEFAULTbindingMap.insert(std::pair<char*, uint>("Dash", SDL_CONTROLLER_BUTTON_A));
				DEFAULTbindingMap.insert(std::pair<char*, uint>("Skill", SDL_CONTROLLER_BUTTON_Y));
				bindingMap = DEFAULTbindingMap;
			}
		}
	}
}

bool Input::Start()
{
	SDL_StopTextInput();

	return true;
}

bool Input::PreUpdate()
{
	key_pressed = false;
	Label::ButtonPressed = -1;


	static SDL_Event event;

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			kbAvailable = true;
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
			kbAvailable = true;
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

		case SDL_CONTROLLERBUTTONDOWN:
			if (event.cbutton.which == 0)
			{
				kbAvailable = false;
				jButtons[event.cbutton.button] = KEY_DOWN;
				//GUI RECEIVES THE FIRST BUTTON HERE WHEN BINDING
				Label::ButtonPressed = event.cbutton.button;
			}
			break;

		case SDL_CONTROLLERBUTTONUP:
			if (event.cbutton.which == 0)
				jButtons[event.cbutton.button] = KEY_UP;

		case SDL_TEXTINPUT:
			kbAvailable = true;
			inputText = event.text.text;
			textReady = true;
			break;
		case SDL_JOYAXISMOTION:
			if (event.jaxis.which == 0)
				kbAvailable = false;
			break;
		}
	}

	//Check Axis States

	if (joystick != nullptr)
	{
		//xAxis
		Sint16 Joy_xAxis = SDL_JoystickGetAxis(joystick, 0);
		{
			if (Joy_xAxis < -J_DEAD_ZONE || Joy_xAxis > J_DEAD_ZONE)
			{
				xAxis = Joy_xAxis;
				xDeadZone = false;
				if (xAxis > 0)
				{
					if (axis[(int)Axis::RIGHT] == KeyState::KEY_DOWN || axis[(int)Axis::RIGHT] == KeyState::KEY_REPEAT)
						axis[(int)Axis::RIGHT] = KeyState::KEY_REPEAT;
					else
					{
						if (axis_counters[(int)Axis::RIGHT] == 0.0f)
						{
							axis[(int)Axis::RIGHT] = KeyState::KEY_DOWN;
							axis_counters[(int)Axis::LEFT] = axisCD;
							axis_counters[(int)Axis::RIGHT] = axisCD;
						}
						else
						{
							if (axis[(int)Axis::RIGHT] == KeyState::KEY_DOWN || axis[(int)Axis::RIGHT] == KeyState::KEY_REPEAT)
								axis[(int)Axis::RIGHT] = KeyState::KEY_UP;
							else axis[(int)Axis::RIGHT] = KeyState::KEY_IDLE;
						}
					}
				}
				else if (xAxis < 0)
				{
					if (axis[(int)Axis::LEFT] == KeyState::KEY_DOWN || axis[(int)Axis::LEFT] == KeyState::KEY_REPEAT)
						axis[(int)Axis::LEFT] = KeyState::KEY_REPEAT;
					else
					{
						if (axis_counters[(int)Axis::LEFT] == 0.0f)
						{
							axis[(int)Axis::LEFT] = KeyState::KEY_DOWN;
							axis_counters[(int)Axis::RIGHT] = axisCD;
							axis_counters[(int)Axis::LEFT] = axisCD;
						}
						else
						{
							if (axis[(int)Axis::LEFT] == KeyState::KEY_DOWN || axis[(int)Axis::LEFT] == KeyState::KEY_REPEAT)
								axis[(int)Axis::LEFT] = KeyState::KEY_UP;
							else axis[(int)Axis::LEFT] = KeyState::KEY_IDLE;
						}
					}

				}
			}
			else
			{
				xAxis = 0;
				xDeadZone = true;
				if (axis[(int)Axis::RIGHT] == KeyState::KEY_DOWN || axis[(int)Axis::RIGHT] == KeyState::KEY_REPEAT)
					axis[(int)Axis::RIGHT] = KeyState::KEY_UP;
				else axis[(int)Axis::RIGHT] = KeyState::KEY_IDLE;

				if (axis[(int)Axis::LEFT] == KeyState::KEY_DOWN || axis[(int)Axis::LEFT] == KeyState::KEY_REPEAT)
					axis[(int)Axis::LEFT] = KeyState::KEY_UP;
				else axis[(int)Axis::LEFT] = KeyState::KEY_IDLE;

			}
		}

		//yAxis
		Sint16 Joy_yAxis = SDL_JoystickGetAxis(joystick, 1);
		{
			if (Joy_yAxis < -J_DEAD_ZONE || Joy_yAxis > J_DEAD_ZONE)
			{
				yAxis = Joy_yAxis;
				yDeadZone = false;

				if (yAxis > 0)
				{
					if (axis[(int)Axis::DOWN] == KeyState::KEY_DOWN || axis[(int)Axis::DOWN] == KeyState::KEY_REPEAT)
						axis[(int)Axis::DOWN] = KeyState::KEY_REPEAT;
					else
					{
						if (axis_counters[(int)Axis::DOWN] == 0.0f)
						{
							axis[(int)Axis::DOWN] = KeyState::KEY_DOWN;
							axis_counters[(int)Axis::UP] = axisCD;
							axis_counters[(int)Axis::DOWN] = axisCD;
						}
						else
						{
							if (axis[(int)Axis::DOWN] == KeyState::KEY_DOWN || axis[(int)Axis::DOWN] == KeyState::KEY_REPEAT)
								axis[(int)Axis::DOWN] = KeyState::KEY_UP;
							else axis[(int)Axis::DOWN] = KeyState::KEY_IDLE;
						}
					}

				}
				else if (yAxis < 0)
				{
					if (axis[(int)Axis::UP] == KeyState::KEY_DOWN || axis[(int)Axis::UP] == KeyState::KEY_REPEAT)
						axis[(int)Axis::UP] = KeyState::KEY_REPEAT;
					else
					{
						if (axis_counters[(int)Axis::UP] == 0.0f)
						{
							axis[(int)Axis::UP] = KeyState::KEY_DOWN;
							axis_counters[(int)Axis::DOWN] = axisCD;
							axis_counters[(int)Axis::UP] = axisCD;
						}
						else
						{
							if (axis[(int)Axis::UP] == KeyState::KEY_DOWN || axis[(int)Axis::UP] == KeyState::KEY_REPEAT)
								axis[(int)Axis::UP] = KeyState::KEY_UP;
							else axis[(int)Axis::UP] = KeyState::KEY_IDLE;
						}
					}
				}
			}
			else
			{
				yAxis = 0;
				yDeadZone = true;
				if (axis[(int)Axis::DOWN] == KeyState::KEY_DOWN || axis[(int)Axis::DOWN] == KeyState::KEY_REPEAT)
					axis[(int)Axis::DOWN] = KeyState::KEY_UP;
				else axis[(int)Axis::DOWN] = KeyState::KEY_IDLE;

				if (axis[(int)Axis::UP] == KeyState::KEY_DOWN || axis[(int)Axis::UP] == KeyState::KEY_REPEAT)
					axis[(int)Axis::UP] = KeyState::KEY_UP;
				else axis[(int)Axis::UP] = KeyState::KEY_IDLE;
			}
		}

		if (xAxis != 0 || yAxis != 0)
			kbAvailable = false;
	}
	


	//Check if controller has been connected to the system
	if (controller == nullptr && joystick == nullptr && SDL_NumJoysticks() > 0)
		InitController();

	//Manage axis counters
	for (int i = 0; i < (int)Axis::MAX; ++i)
	{
		axis_counters[i] -= App->dt;
		if (axis_counters[i] < 0.0f)
			axis_counters[i] = 0.0f;
	}

	return true;
}

bool Input::CleanUp()
{
	LOG("Quitting haptic");
	SDL_HapticClose(controllerHaptic);
	controllerHaptic = NULL;

	LOG("Quitting joystick");
	SDL_JoystickClose(joystick);
	joystick = NULL;

	LOG("Quitting controller");
	SDL_GameControllerClose(controller);
	controller = NULL;
	
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);

	return true;
}

bool Input::GetWindowEvent(EventWindow ev)
{
	return windowEvents[ev];
}

float Input::GetPercentageFromAxis() const
{
	float moduleVec = sqrtf((pow(xAxis, 2.0f) + pow(yAxis, 2.0f)));

	return moduleVec / MAX_JAXIS_VALUE;
}

float Input::GetAngleFromAxis() const
{
	float angle = RAD_2_DEG(atan2(yAxis, xAxis));

	if (angle < 0)
		angle += 360.0f;

	return angle;
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


bool Input::PlayJoyRumble(float strength, Uint32 length)
{
	if (SDL_HapticRumblePlay(controllerHaptic, strength, length) != 0)
	{
		LOG("Warning: Unable to play rumble! %s", SDL_GetError());
		return false;
	}
	else
		return true;
}

void Input::ExternActionsAtKeyInput(const int key) {
	// NOMES FER SERVIR EN CASOS MOLT CONCRETS I OPTIMS !!!!!!!!
	// printf_s("%i\n", key);		// PER TROBAR EL NUMERO DE LES TECLES
	switch (key) 
	{
		case SDL_SCANCODE_GRAVE:	// º button -> OpenConsole
			if(App->console->isActive())
				App->console->SwitchWrittingState();
		break;
	}
}

void Input::Save(pugi::xml_node& inputNode)
{
	pugi::xml_node binding = inputNode.append_child("Binding");
	binding.append_attribute("Attack").set_value(getBindingfromAction("Attack"));
	binding.append_attribute("Skill").set_value(getBindingfromAction("Skill"));
	binding.append_attribute("Dash").set_value(getBindingfromAction("Dash"));
}

void Input::Load(const pugi::xml_node& inputNode)
{
	pugi::xml_node binding = inputNode.child("Binding");
	bindingMap.find("Attack")->second = App->input->toGameControllerButton((char*)binding.attribute("Attack").as_string());
	bindingMap.find("Skill")->second = App->input->toGameControllerButton((char*)binding.attribute("Skill").as_string());
	bindingMap.find("Dash")->second = App->input->toGameControllerButton((char*)binding.attribute("Dash").as_string());
}