#ifndef __INPUTBOX_H__
#define __INPUTBOX_H__

#include "GUIElem.h"

//atlasRect defines---------------------------

#define InputBox1 {0,0,0,0}

//----------------------------------

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
	void EnableInput();
	void DisableInput();
	void ClearBox();

private:
	bool ReadyToWrite = false;
	std::string text;
	TTF_Font* font = nullptr;
	SDL_Color color = Black;
	SDL_Texture* texturetoBlit = nullptr;
};

#endif
