#include "App.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "Fonts.h"
#include "InputBox.h"

InputBox::InputBox(fPoint localPos, InputBoxInfo& info, Module* listener, GUIElem* parent) : GUIElem(localPos, listener, InputBox1, GUIElemType::INPUTBOX, parent)
{
	font = Application->fonts->getFontbyName(info.fontName);
	color = info.color;
}

InputBox::~InputBox(){}

bool InputBox::Update(float dt)
{
	bool ret = false;

	if (ReadyToWrite)
	{
		if (Application->input->IsTextReady())
		{
			text += Application->input->GetText();
			SDL_DestroyTexture(texturetoBlit);
			texturetoBlit = Application->fonts->Print(text.data(), color, font);
			Application->input->SetTextReadyFalse();
		}
		else if (Application->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN && text.size() > 0)
		{
			text.pop_back();
			SDL_DestroyTexture(texturetoBlit);
			texturetoBlit = Application->fonts->Print(text.data(), color, font);
		}
	}

	ret = Application->render->Blit(texturetoBlit, (int)(this->screenPos.x + Application->render->camera.x), (int)(this->screenPos.y + Application->render->camera.y));

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
	text.clear();
	SDL_DestroyTexture(texturetoBlit);
	texturetoBlit = Application->fonts->Print(text.data(), color, font);
}