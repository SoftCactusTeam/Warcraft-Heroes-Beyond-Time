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

	minValue = screenPos.x + 8;
	maxValue = screenPos.x + atlasRect.w - 10;

	switch (stype)
	{
		case SliderType::MUSIC_VOLUME:
			this->smobilepos = (App->audio->MusicVolumePercent * (maxValue - minValue)) / 100 + minValue - 4;
			this->valuePercent = App->audio->MusicVolumePercent;
			break;
		case SliderType::FX_VOLUME:
			this->smobilepos = (App->audio->FXVolumePercent * (maxValue - minValue)) / 100 + minValue - 4;
			this->valuePercent = App->audio->FXVolumePercent;
			break;
	}
}

bool Slider::Update(float dt)
{
	bool ret = true;

	ret = HandleInput();
	
	if(ret)
		ret = UpdateChilds(dt);

	return ret;
}

bool Slider::Draw()
{
	bool ret = true;

	if (ret)
		ret = App->render->Blit(App->gui->getAtlas(), (int)(screenPos.x - App->render->camera.x), (int)(screenPos.y - App->render->camera.y), &atlasRect);

	if (ret)
		ret = App->render->Blit(App->gui->getAtlas(), (int)(smobilepos - App->render->camera.x), (int)(screenPos.y - 5 - App->render->camera.y), &sliderMobileRect);

	if (ret)
		ret = DrawChilds();

	return ret;
}

bool Slider::HandleInput()
{
	bool ret = true;
	
	int x, y;
	App->input->GetMousePosition(x, y);

	if (MouseHover() && (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN))
		focused = true;
	else if (focused && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
		focused = false;
	
	if(focused)
	{
		if (x <= minValue)
			smobilepos = minValue - 4;
		else if (x >= maxValue)
			smobilepos = maxValue - 4;
		else
			smobilepos = x - 4;

		valuePercent = (smobilepos + 4 - minValue) * 100 / (maxValue - minValue);

		Label* label;
		std::list<GUIElem*>::iterator it;
		for (it = childs.begin(); it != childs.end(); ++it)
		{
			label = (Label*)(*it);
			label->EditText(std::to_string(valuePercent), White);
			break;
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

bool Slider::MouseHover() const
{
	int x, y;
	App->input->GetMousePosition(x, y);

	bool result = false;

	//if collides
	if (!(x < minValue ||
		x > maxValue ||
		y < screenPos.y ||
		y > screenPos.y + atlasRect.h))
	{
		result = true;
	}
	return result;
}

