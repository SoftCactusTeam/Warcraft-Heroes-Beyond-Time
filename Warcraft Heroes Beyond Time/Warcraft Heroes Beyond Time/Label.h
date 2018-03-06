#ifndef __LABEL_H__
#define __LABEL_H__

#include <string>
#include "GUIElem.h"
#include "Globals.h"

struct LabelInfo 
{
	char* text = nullptr;
	char* fontName = nullptr;
	SDL_Color color = Black;
};

class Label : public GUIElem
{
public:
	Label() {};
	Label(fPoint position, LabelInfo& info, GUIElem* parent, Module* listener);
	~Label();
	bool Update(float dt);
	void EditText(char* text, SDL_Color color);

private:
	std::string text = nullptr;
	TTF_Font* font = nullptr;
	SDL_Texture* texturetoBlit = nullptr;
};

#endif