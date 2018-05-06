#include "GUIImage.h"
#include "Application.h"
#include "ModuleRender.h"


GUIImage::GUIImage(fPoint localPos, SDL_Rect atlasRect, Module* listener, GUIElem* parent) : GUIElem(localPos, listener, atlasRect, GUIElemType::IMAGE, parent)
{	
	
}

GUIImage::~GUIImage()
{
	DestroyChilds();
}

bool GUIImage::Draw()
{
	bool ret = true;

	if (!SDL_RectEquals(&atlasRect, &SDL_Rect({0,912,640,360})))
		ret = App->render->Blit(App->gui->GetAtlas(), (int)(this->screenPos.x - App->render->camera.x), (int)(this->screenPos.y - App->render->camera.y), &atlasRect);
	else
		ret = App->render->Blit(App->gui->GetBloodAtlas(), (int)(this->screenPos.x - App->render->camera.x), (int)(this->screenPos.y - App->render->camera.y), &atlasRect);

	if (ret)
		ret = DrawChilds();

	return ret;
}

void GUIImage::SetColor(SDL_Color color)
{
	SDL_SetTextureColorMod(App->gui->GetBloodAtlas(), color.r, color.g, color.b);
	SDL_SetTextureAlphaMod(App->gui->GetBloodAtlas(), color.a);
}
