#ifndef __SLIDER_H__
#define __SLIDER_H__

#include "GUIElem.h"
#include "ModuleAudio.h"

//atlasRect defines---------------------------

#define sliderbar {145, 76, 258, 18}
#define sliderbar_focused {145,104,258,21}

#define slidermobile {407, 71,11,28}
#define slidermobile_focused {407,100,11,28}

#define sliderdeadzone 10

//----------------------------------



class Slider : public GUIElem
{
public:
	enum class SliderType
	{
		NO_STYPE = -1,
		MUSIC_VOLUME,
		FX_VOLUME
	};


public:

	Slider() {}
	Slider(fPoint localPos, Module* listener, SDL_Rect atlasRect, SliderInfo sInfo, GUIElemType type, GUIElem* parent = nullptr);
	virtual ~Slider();

	bool Update(float dt);
	bool Draw();

	bool HandleInput(float dt);
	bool MouseHover() const;

protected:
	SliderType stype = SliderType::NO_STYPE;
	uint valuePercent;

	float smobilepos;
	SDL_Rect sliderMobileRect = slidermobile;

	uint minValue = 0;
	uint maxValue = 0;

	float counter = 0;

	void Focus()
	{
		focused = true;
		App->audio->HaltFX(App->audio->ButtonHovered);
		App->audio->PlayFx(App->audio->ButtonHovered);
	}
};

struct SliderInfo
{
	Slider::SliderType type = Slider::SliderType::NO_STYPE;
};

#endif