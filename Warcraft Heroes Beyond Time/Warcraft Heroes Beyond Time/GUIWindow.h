#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "GUIElem.h"

//atlasRect defines---------------------------

#define StoneWindow {324, 472, 255, 296}
#define WoodWindow  {1129, 48, 249, 286}
//----------------------------------

class GUIWindow : public GUIElem
{
public:

	GUIWindow() {}
	GUIWindow(fPoint localPos, SDL_Rect atlasRect, GUIElem* parent = nullptr, Module* listener = nullptr);

	virtual ~GUIWindow();

	bool Update(float dt);

	bool Draw();

private:
	SDL_Rect background;
};


#endif