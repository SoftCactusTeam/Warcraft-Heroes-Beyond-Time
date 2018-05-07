#ifndef __GUIIMAGE_H__
#define __GUIIMAGE_H__

#include "GUIElem.h"

class GUIImage : public GUIElem
{
public:
	GUIImage() {}
	GUIImage(fPoint localPos, SDL_Rect atlasRect, Module* listener, GUIElem* parent = nullptr);
	virtual ~GUIImage();

	bool Draw();
};

#endif