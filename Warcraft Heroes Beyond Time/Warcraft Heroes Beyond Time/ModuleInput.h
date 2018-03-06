#ifndef __INPUT_H__
#define __INPUT_H__

#include "Module.h"

#include "SDL\include\SDL_gamecontroller.h"
#include "SDL\include\SDL_scancode.h"

//#define NUM_KEYS 352
#define NUM_MOUSE_BUTTONS 5
//#define LAST_KEYS_PRESSED_BUFFER 50

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
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class Input : public Module
{

public:

	Input();
	virtual ~Input();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool CleanUp();

	bool GetWindowEvent(EventWindow ev);

	KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	KeyState GetMouseButtonDown(int id) const
	{
		return mouse_buttons[id - 1];
	}

	bool GetWindowEvent(int code);

	void GetMousePosition(int &x, int &y);
	void GetMouseMotion(int& x, int& y);

	bool IsAnyKeyPressed();
	bool IsTextReady() { return textReady; };
	void SetTextReadyFalse() { textReady = false; };
	char* GetText() { return inputText; };

	void ExternActionsAtKeyInput(int key);
private:
	bool		windowEvents[WE_COUNT];
	KeyState*	keyboard;
	KeyState	mouse_buttons[NUM_MOUSE_BUTTONS];
	int			mouse_motion_x;
	int			mouse_motion_y;
	int			mouse_x;
	int			mouse_y;

	char*		inputText = nullptr;
	bool		textReady = false;
	bool		key_pressed = false;
};

#endif