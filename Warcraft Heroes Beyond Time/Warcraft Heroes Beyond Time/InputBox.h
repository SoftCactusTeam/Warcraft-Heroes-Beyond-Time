#ifndef __INPUTBOX_H__
#define __INPUTBOX_H__

#include "GUIElem.h"

struct InputBoxInfo
{
	SDL_Color color = { 255,255,255,255 };
	char* fontName = nullptr;
};

class InputBox : public GUIElem
{
public:
	InputBox() {};
	InputBox(iPoint position, InputBoxInfo& info, GUIElem* parent, Module* listener);
	~InputBox();
	bool Update(float dt);
	void EnableInput();
	void DisableInput();
	void ClearBox();

private:
	bool ReadyToWrite = false;
	std::string text = nullptr;
	TTF_Font* font = nullptr;
	SDL_Color color = { 255,255,255,255 };
};

#endif
