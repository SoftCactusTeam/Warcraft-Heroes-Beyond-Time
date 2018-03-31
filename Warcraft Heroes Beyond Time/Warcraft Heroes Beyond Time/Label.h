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
	Label() {}
	Label(fPoint position, LabelInfo& info, GUIElem* parent, Module* listener);
	~Label();
	bool Update(float dt);
	bool Draw();
	bool MouseHover() const;
	void EditText(std::string text, SDL_Color color = {0,0,0,0});

private:
	std::string text;
	TTF_Font* font = nullptr;
	SDL_Texture* texturetoBlit = nullptr;
	SDL_Color color;
};

#endif