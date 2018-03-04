#ifndef __LABEL_H__
#define __LABEL_H__

#include <string>
#include "GUIElem.h"

struct LabelInfo 
{
	char* text = nullptr;
	char* fontName = nullptr;
	SDL_Color color = { 255,255,255,255 };
};

class Label : public GUIElem
{
public:
	Label() {};
	Label(iPoint position, LabelInfo& info, GUIElem* parent, Module* listener);
	~Label();
	bool Update(float dt);
	void EditText(char* text, SDL_Color color);

private:
	std::string text = nullptr;
	TTF_Font* font = nullptr;
};

#endif