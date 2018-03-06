#include "App.h"
#include "ModuleRender.h"
#include "Label.h"
#include "Fonts.h"
#include "GUIElem.h"

Label::Label(fPoint position, LabelInfo& info, GUIElem* parent, Module* listener) : GUIElem(position, listener, {}, GUIElemType::LABEL, parent)
{
	text = info.text;
	font = Application->fonts->getFontbyName(info.fontName);
	texturetoBlit = Application->fonts->Print(text.c_str(), info.color, font);
}

Label::~Label() {}

bool Label::Update(float dt)
{
	bool result = false;

	
	result = Application->render->Blit(texturetoBlit, (int)(this->screenPos.x + Application->render->camera.x), (int)(this->screenPos.y + Application->render->camera.y));

	UpdateChilds(dt);

	return result;
}

void Label::EditText(char* text, SDL_Color color)
{
	this->text = text;
	SDL_DestroyTexture(texturetoBlit);
	texturetoBlit = Application->fonts->Print(text, color, font);
}
