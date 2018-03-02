#ifndef __LABEL_H__
#define __LABEL_H__

#include <string>
#include "ModuleRender.h"

class Label : public GUIElem
{
	friend class ModuleGUI;

public:

	Label(fPoint position, GUIElem* parent) : GUIElem(GUIElemType::LABEL, position, parent) {}
	
	virtual ~Label() 
	{
		text.clear();
	}

	bool Update(float dt);



private:

	std::string text;
	TTF_Font* font;
};


bool Label::Update(float dt)
{
	bool result = false;

	SDL_Texture* textBlitting = Application->fonts->Print(text.c_str(), { 255,255,255,255 }, font);
	result = Application->render->Blit(textBlitting, position.x, position.y);

	return result;
}




#endif