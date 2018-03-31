#ifndef __SLIDER_H__
#define __SLIDER_H__

#include "GUIElem.h"

//atlasRect defines---------------------------

#define sliderbar {145,104,258,21}
#define slidermobile {406,100,11,31}

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
	virtual ~Slider() {}

	bool Update(float dt);
	bool Draw();

	bool HandleInput();
	bool MouseHover() const;

protected:
	SliderType stype = SliderType::NO_STYPE;
	uint valuePercent;

	float smobilepos;
	SDL_Rect sliderMobileRect = slidermobile;
	bool focused = false;

	uint minValue = 0;
	uint maxValue = 0;


};

struct SliderInfo
{
	Slider::SliderType type = Slider::SliderType::NO_STYPE;
};

#endif