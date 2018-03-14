#include "Slider.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "Label.h"
#include <stdlib.h>
#include "ModuleAudio.h"

Slider::Slider(fPoint localPos, Module* listener, SDL_Rect atlasRect, SliderInfo sInfo, GUIElemType type, GUIElem* parent) : GUIElem(localPos, listener, atlasRect, type, parent)
{
	stype = sInfo.type;
	
	switch (stype)
	{
		case SliderType::MUSIC_VOLUME:
			this->smobilepos = (atlasRect.w - 2 * sliderdeadzone) * App->audio->MusicVolumePercent / 100 + screenPos.x + sliderdeadzone / 2;
			this->valuePercent = App->audio->MusicVolumePercent;
			break;
		case SliderType::FX_VOLUME:
			this->smobilepos = (atlasRect.w - 2 * sliderdeadzone) * App->audio->FXVolumePercent / 100 + screenPos.x + sliderdeadzone / 2;
			this->valuePercent = App->audio->FXVolumePercent;
			break;
	}
	
}


bool Slider::Update(float dt)
{
	bool ret = true;


	ret = HandleInput();

	if(ret)
		ret = App->render->Blit(App->gui->getAtlas(), (int)(screenPos.x + App->render->camera.x), (int)(screenPos.y + App->render->camera.y), &atlasRect);

	if (ret)
		ret = App->render->Blit(App->gui->getAtlas(), (int)(smobilepos + App->render->camera.x), (int)(screenPos.y - 7 + App->render->camera.y), &sliderMobileRect);
	
	if(ret)
		ret = UpdateChilds(dt);

	return ret;
}

bool Slider::HandleInput()
{
	bool ret = true;
	uint minvalue = screenPos.x + sliderdeadzone;
	uint maxvalue = screenPos.x + atlasRect.w - sliderdeadzone;
	int x, y;
	App->input->GetMousePosition(x, y);

	if (MouseHover() && (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN))
		focused = true;
	else if (focused && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
		focused = false;
	
	if(focused)
	{
		if (x <= minvalue)
			smobilepos = minvalue - 15 / 2;
		else if (x >= maxvalue)
			smobilepos = maxvalue - 15 / 2;
		else
			smobilepos = x - 15 / 2;

		valuePercent = (smobilepos + 15/2 - minvalue) * 100 / (maxvalue - minvalue);

		Label* label;
		std::list<GUIElem*>::iterator it;
		for (it = childs.begin(); it != childs.end(); ++it)
		{
			label = (Label*)(*it);
			label->EditText(std::to_string(valuePercent), White);
		}
		
		switch(stype)
		{
			case SliderType::MUSIC_VOLUME:
				App->audio->setMusicVolume(valuePercent);
				break;
			case SliderType::FX_VOLUME:
				App->audio->setFXVolume(valuePercent);
				break;
		}
		

	}



	return ret;
}

