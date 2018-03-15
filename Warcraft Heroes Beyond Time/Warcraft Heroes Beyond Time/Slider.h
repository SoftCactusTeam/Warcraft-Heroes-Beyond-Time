#ifndef __SLIDER_H__
#define __SLIDER_H__

#include "GUIElem.h"

//atlasRect defines---------------------------

#define sliderbar {207,151,366,23}
#define slidermobile {580,144,15,38}

#define sliderdeadzone 12

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
	virtual ~Slider() {}

	bool Update(float dt);
	bool HandleInput();

protected:
	SliderType stype = SliderType::NO_STYPE;
	uint valuePercent;

	float smobilepos;
	SDL_Rect sliderMobileRect = slidermobile;
	bool focused = false;
};

struct SliderInfo
{
	Slider::SliderType type = Slider::SliderType::NO_STYPE;
};

#endif