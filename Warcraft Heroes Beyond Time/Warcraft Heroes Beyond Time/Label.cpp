#include "ModuleRender.h"
#include "Label.h"
#include "Fonts.h"
#include "GUIElem.h"
#include "Application.h"
#include "ModuleInput.h"
#include "Scene.h"


int Label::ButtonPressed = -1;
bool Label::waitingBindInput = false;

Label::Label(fPoint position, LabelInfo& info, GUIElem* parent, Module* listener) : GUIElem(position, listener, {}, GUIElemType::LABEL, parent)
{
	text = info.text;
	font = App->fonts->getFontbyName(info.fontName);
	texturetoBlit = App->fonts->Print(text.data(), info.color, font, info.multilabelWidth);
	color = info.color;
	multilabelWidth = info.multilabelWidth;
}

Label::~Label()
{
	SDL_DestroyTexture(texturetoBlit);
	texturetoBlit = nullptr;
}

bool Label::Update(float dt)
{
	bool result = false;

	if (focused && !childs.empty())
		childs.front()->Focus();
	else if (!childs.empty())
		childs.front()->UnFocus();
	
	if (focused && ButtonPressed != -1)
	{
		if (!childs.empty() && (this == App->scene->attackBinding || this == App->scene->dashBinding || this == App->scene->skillBinding))
		{
			Label* label = (Label*)childs.front()->getFirstChild();
			label->EditText(App->input->toString((SDL_GameControllerButton)ButtonPressed), label->color);

			int w, h;
			SDL_QueryTexture(label->texturetoBlit, nullptr, nullptr, &w, &h);
			label->localPos = {(float)(58 - ((w / 2)/3)), label->localPos.y};
			label->calculateScreenPos();

			if (this == App->scene->attackBinding)
				App->input->rebindAction("Attack", ButtonPressed);
			else if (this == App->scene->skillBinding)
				App->input->rebindAction("Skill", ButtonPressed);
			else if (this == App->scene->dashBinding)
				App->input->rebindAction("Dash", ButtonPressed);

		}
	}

	result = UpdateChilds(dt);

	return result;
}

bool Label::Draw()
{
	bool result = true;

	result = App->render->Blit(texturetoBlit, (int)(this->screenPos.x - App->render->camera.x), (int)(this->screenPos.y - App->render->camera.y), nullptr, 0.3, 1, 0, 0, 0, true);

	if (result)
	{
		if ((this == App->scene->attackBinding || this == App->scene->dashBinding || this == App->scene->skillBinding) ||
			this->IsFocused())
			result = DrawChilds();
	}
		

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
	texturetoBlit = App->fonts->Print(text.data(), (ColorEquals({0,0,0,0}, color) ? this->color : color), font, multilabelWidth);
}


