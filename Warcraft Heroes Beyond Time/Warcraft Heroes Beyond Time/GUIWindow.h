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
	void UnFocusChilds();

	//0 for transparent, 255 for full opaque
	void setOpacity(Uint8 amount);

private:
	bool AnyChildFocused();
	void FocusNextChild();
	void FocusPrevChild();
	bool checkVerticalInputs(float dt);
	bool checkHorizontalInputs(float dt);

private:
	SDL_Rect background;
	float counter = 0.0f;
	float minicounter = 0.0f;
public: 
	bool blackBackground = false;
	bool vertical = true;
	bool menu = true;
	
};


#endif