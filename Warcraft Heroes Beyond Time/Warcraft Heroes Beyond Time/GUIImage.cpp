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

	SDL_Texture* atlas = App->gui->getAtlas();
	SDL_SetTextureAlphaMod(atlas, opacity);
	ret = App->render->Blit(atlas, (int)(this->screenPos.x - App->render->camera.x), (int)(this->screenPos.y - App->render->camera.y), &atlasRect);
	SDL_SetTextureAlphaMod(atlas, 255);

	if (ret)
		ret = DrawChilds();

	return ret;
}