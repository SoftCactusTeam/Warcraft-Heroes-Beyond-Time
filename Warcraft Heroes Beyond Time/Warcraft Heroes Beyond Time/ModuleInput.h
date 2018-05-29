#ifndef __INPUT_H__
#define __INPUT_H__

#include "Module.h"

#include "SDL\include\SDL_gamecontroller.h"
#include "SDL\include\SDL_scancode.h"
#include "Log.h"

#include <map>

#define NUM_MOUSE_BUTTONS 5
#define J_DEAD_ZONE 12000
#define NUM_J_BUTTONS 16
#define MAX_JAXIS_VALUE 32767.0f

struct SDL_Rect;

enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum KeyState
{
	NON_VALID_STATE = -1,
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

enum class Axis
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	MAX
};

class Input : public Module
{

public:

	Input();
	virtual ~Input();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool CleanUp();

	void Save(pugi::xml_node&);

	void Load(const pugi::xml_node&);

	bool GetWindowEvent(EventWindow ev);
	void InitController();

	inline const char* toString(SDL_GameControllerButton b)
	{
		switch (b)
		{
			case SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A:   return "A";
			case SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B:   return "B";
			case SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_X:   return "X";
			case SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_Y:   return "Y";
			case SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_BACK:return "Back";
			case SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_DOWN: return "DDown";
			case SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT: return "DLeft";
			case SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT: return "DRight";
			case SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_UP: return "DUp";
			case SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_GUIDE: return "Guide";
			case SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSHOULDER: return "LB";
			case SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: return "RB";
			case SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSTICK: return "LStick";
			case SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_RIGHTSTICK: return "RStick";
			case SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_START: return "Start";
			default:      return "[Unknown Button]";
		}
	}

	inline const SDL_GameControllerButton toGameControllerButton(char* button)
	{
		if (strcmp(button, "A") == 0)
			return SDL_CONTROLLER_BUTTON_A;
		else if (strcmp(button, "B") == 0)
			return SDL_CONTROLLER_BUTTON_B;
		else if (strcmp(button, "X") == 0)
			return SDL_CONTROLLER_BUTTON_X;
		else if (strcmp(button, "Y") == 0)
			return SDL_CONTROLLER_BUTTON_Y;
		else if (strcmp(button, "Back") == 0)
			return SDL_CONTROLLER_BUTTON_BACK;
		else if (strcmp(button, "DDown") == 0)
			return SDL_CONTROLLER_BUTTON_DPAD_DOWN;
		else if (strcmp(button, "DUp") == 0)
			return SDL_CONTROLLER_BUTTON_DPAD_UP;
		else if (strcmp(button, "DRight") == 0)
			return SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
		else if (strcmp(button, "DLeft") == 0)
			return SDL_CONTROLLER_BUTTON_DPAD_LEFT;
		else if (strcmp(button, "Guide") == 0)
			return SDL_CONTROLLER_BUTTON_GUIDE;
		else if (strcmp(button, "LB") == 0)
			return SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
		else if (strcmp(button, "RB") == 0)
			return SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
		else if (strcmp(button, "LStick") == 0)
			return SDL_CONTROLLER_BUTTON_LEFTSTICK;
		else if (strcmp(button, "RStick") == 0)
			return SDL_CONTROLLER_BUTTON_RIGHTSTICK;
		else if (strcmp(button, "Start") == 0)
			return SDL_CONTROLLER_BUTTON_START;
		else
			return SDL_CONTROLLER_BUTTON_INVALID;
	}

	const char* getBindingfromAction(char* action)
	{
		return toString((SDL_GameControllerButton)bindingMap.find(action)->second);
	}

	bool rebindAction(char* action, int newBinding)
	{
		bindingMap.find(action)->second = newBinding;
		return true;
	}

	bool resetBinding()
	{
		bindingMap = DEFAULTbindingMap;
		return true;
	}

	KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	KeyState GetMouseButtonDown(int id) const
	{
		return mouse_buttons[id - 1];
	}

	KeyState GetPadButtonDown(int id) const
	{
		return jButtons[id];
	}

	KeyState GetAction(char* action)
	{
		uint enumValue = -1;
		try
		{
			enumValue = bindingMap.at(action);
		}
			
		catch (const std::out_of_range& oor)
		{
			LOG("Action not binded yet");
			return KeyState::NON_VALID_STATE;
		}

		return GetPadButtonDown(enumValue);
	}

	float GetXAxis() const
	{
		return xAxis;
	}

	float GetYAxis() const
	{
		return yAxis;
	}

	KeyState GetAxis(int id) const
	{
		return axis[id];
	}

	bool InsideDeadZone() const
	{
		return xDeadZone && yDeadZone;
	}

	float GetPercentageFromAxis() const;
	float GetAngleFromAxis() const;

	void GetMousePosition(int &x, int &y);
	void GetMouseMotion(int& x, int& y);

	bool IsAnyKeyPressed();
	bool IsTextReady() { return textReady; };
	void SetTextReadyFalse() { textReady = false; };
	char* GetText() { return inputText; };
	bool IsKeyboardAvailable() { return kbAvailable; }
	void ExternActionsAtKeyInput(int key);
	bool PlayJoyRumble(float strength, Uint32 length);
	bool isControllerConnected()const { return controller != nullptr; }

private:
	bool		windowEvents[WE_COUNT];
	KeyState*	keyboard;
	KeyState	mouse_buttons[NUM_MOUSE_BUTTONS];
	KeyState	jButtons[NUM_J_BUTTONS];
	KeyState    axis[(int)Axis::MAX];

	float		axis_counters[(int)Axis::MAX] = {0.0f, 0.0f, 0.0f, 0.0f};

	int			mouse_motion_x;
	int			mouse_motion_y;
	int			mouse_x;
	int			mouse_y;

	char*		inputText = nullptr;
	bool		textReady = false;
	bool		key_pressed = false;

	SDL_Joystick* joystick = NULL;
	SDL_GameController* controller = NULL;
	SDL_Haptic* controllerHaptic = NULL;

	std::map<char*, uint> bindingMap;
	std::map<char*, uint> DEFAULTbindingMap;

	float xAxis = 0;
	float yAxis = 0;
	bool xDeadZone = false;
	bool yDeadZone = false;
	bool kbAvailable = false;
};

#endif