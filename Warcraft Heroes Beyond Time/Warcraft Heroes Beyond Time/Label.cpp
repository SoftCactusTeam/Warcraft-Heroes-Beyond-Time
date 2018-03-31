#include "ModuleRender.h"
#include "Label.h"
#include "Fonts.h"
#include "GUIElem.h"
#include "Application.h"
#include "ModuleInput.h"

Label::Label(fPoint position, LabelInfo& info, GUIElem* parent, Module* listener) : GUIElem(position, listener, {}, GUIElemType::LABEL, parent)
{
	text = info.text;
	font = App->fonts->getFontbyName(info.fontName);
	texturetoBlit = App->fonts->Print(text.c_str(), info.color, font);
	color = info.color;
}

Label::~Label() {}

bool Label::Update(float dt)
{
	bool result = false;

	result = UpdateChilds(dt);

	return result;
}

bool Label::Draw()
{
	bool result = true;

	result = App->render->Blit(texturetoBlit, (int)(this->screenPos.x - App->render->camera.x), (int)(this->screenPos.y - App->render->camera.y), nullptr, 0.3);

	if (result)
		result = DrawChilds();

	return result;
}

bool Label::MouseHover() const
{
	int x, y;
	App->input->GetMousePosition(x, y);

	bool result = false;

	fPoint worldPos = { screenPos.x - App->render->camera.x, screenPos.y - App->render->camera.y };

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

void Label::EditText(std::string text, SDL_Color color)
{
	this->text = text;
	SDL_DestroyTexture(texturetoBlit);
	texturetoBlit = App->fonts->Print(text.data(), (ColorEquals({0,0,0,0}, color) ? this->color : color), font);
}


