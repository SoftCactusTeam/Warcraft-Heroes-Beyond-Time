#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "Fonts.h"
#include "InputBox.h"

InputBox::InputBox(fPoint localPos, InputBoxInfo& info, Module* listener, GUIElem* parent) : GUIElem(localPos, listener, {0,0,0,0}, GUIElemType::INPUTBOX, parent)
{
	font = App->fonts->getFontbyName(info.fontName);
	color = info.color;
}

InputBox::~InputBox(){}

bool InputBox::Update(float dt)
{
	bool ret = false;

	if (ReadyToWrite)
	{
		if (App->input->IsTextReady())
		{
			text += App->input->GetText();
			SDL_DestroyTexture(texturetoBlit);
			texturetoBlit = App->fonts->Print(text.data(), color, font);
			App->input->SetTextReadyFalse();
		}
		else if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN && text.size() > 0)
		{
			text.pop_back();
			SDL_DestroyTexture(texturetoBlit);
			texturetoBlit = App->fonts->Print(text.data(), color, font);
		}
	}

	ret = App->render->Blit(texturetoBlit, (int)(this->screenPos.x - App->render->camera.x), (int)(this->screenPos.y - App->render->camera.y));

	UpdateChilds(dt);


	ret = true;
	return ret;
}

void InputBox::EnableInput()
{
	ReadyToWrite = true;
	SDL_StartTextInput();
}

void InputBox::DisableInput()
{
	ReadyToWrite = false;
	SDL_StopTextInput();
}

void InputBox::ClearBox()
{
	if(!text.empty())
		text.clear();

	if(texturetoBlit)
		SDL_DestroyTexture(texturetoBlit);

	texturetoBlit = App->fonts->Print(text.data(), color, font);
}

