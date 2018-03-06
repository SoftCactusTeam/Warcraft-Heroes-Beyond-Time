#include "ModuleRender.h"
#include "Label.h"
#include "Fonts.h"
#include "GUIElem.h"
#include "App.h"
#include "ModuleInput.h"

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

bool Label::MouseHover() const
{
	int x, y;
	Application->input->GetMousePosition(x, y);

	bool result = false;

	fPoint worldPos = { screenPos.x + Application->render->camera.x, screenPos.y + Application->render->camera.y };

	int w, h;
	SDL_QueryTexture(texturetoBlit, nullptr, nullptr, &w, &h);

	//if collides
	if (!(x < worldPos.x ||
		x > worldPos.x + w ||
		y < worldPos.y ||
		y > worldPos.y + h))
	{
		result = true;
	}
	return result;
}

void Label::EditText(char* text, SDL_Color color)
{
	this->text = text;
	SDL_DestroyTexture(texturetoBlit);
	texturetoBlit = Application->fonts->Print(text, color, font);
}


