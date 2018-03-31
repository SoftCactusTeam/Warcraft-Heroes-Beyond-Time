#ifndef __INPUTBOX_H__
#define __INPUTBOX_H__

#include "GUIElem.h"

struct InputBoxInfo
{
	SDL_Color color = Black;
	char* fontName = nullptr;
};

class InputBox : public GUIElem
{
public:
	InputBox() {};
	InputBox(fPoint localPos, InputBoxInfo& info, Module* listener, GUIElem* parent = nullptr);
	~InputBox();
	bool Update(float dt);
	bool Draw();
	void EnableInput();
	void DisableInput();
	void ClearBox();

public:
	std::string text;

private:
	bool ReadyToWrite = false;
	TTF_Font* font = nullptr;
	SDL_Color color = Black;
	SDL_Texture* texturetoBlit = nullptr;
};

#endif
