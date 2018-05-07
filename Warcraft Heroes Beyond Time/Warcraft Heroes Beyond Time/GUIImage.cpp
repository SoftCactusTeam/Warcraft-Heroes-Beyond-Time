#include "GUIImage.h"
#include "Application.h"
#include "ModuleRender.h"


GUIImage::GUIImage(fPoint localPos, SDL_Rect atlasRect, Module* listener, GUIElem* parent) : GUIElem(localPos, listener, atlasRect, GUIElemType::IMAGE, parent) {}

GUIImage::~GUIImage()
{
	DestroyChilds();
}

bool GUIImage::Draw()
{
	bool ret = true;

	ret = App->render->Blit(App->gui->getAtlas(), (int)(this->screenPos.x - App->render->camera.x), (int)(this->screenPos.y - App->render->camera.y), &atlasRect);

	if (ret)
		ret = DrawChilds();

	return ret;
}