#include "App.h"
#include "ModuleRender.h"
#include "Label.h"
#include "Fonts.h"

Label::Label(iPoint position, LabelInfo& info, GUIElem* parent, Module* listener) : GUIElem(position, parent, listener)
{
	text = info.text;
	font = Application->fonts->getFontbyName(info.fontName);
	textureToBlit = Application->fonts->Print(text.c_str(), info.color, font);
}

Label::~Label() {}

bool Label::Update(float dt)
{
	bool result = false;

	result = Application->render->Blit(textureToBlit, position.x, position.y);

	return result;
}

void Label::EditText(char* text, SDL_Color color)
{
	this->text = text;
	SDL_DestroyTexture(textureToBlit);
	textureToBlit = Application->fonts->Print(text, color, font);
}
