#include "GUIImage.h"
#include "Application.h"
#include "ModuleRender.h"


GUIImage::GUIImage(fPoint localPos, SDL_Rect atlasRect, Module* listener, GUIElem* parent) : GUIElem(localPos, listener, atlasRect, GUIElemType::IMAGE, parent) {}

GUIImage::~GUIImage()
{
	DestroyChilds();
}

bool GUIImage::Update(float dt)
{
	if (focused && !childs.empty() && !childs.front()->IsFocused())
		childs.front()->Focus();
	else if(!focused && !childs.empty() && childs.front()->IsFocused())
		childs.front()->UnFocus();




	return true;
}

bool GUIImage::Draw()
{
	bool ret = true;

	SDL_Texture* atlas = App->gui->getAtlas();
	SDL_SetTextureAlphaMod(atlas, opacity);

	if(!symbol)
		ret = App->render->Blit(atlas, (int)(this->screenPos.x - App->render->camera.x), (int)(this->screenPos.y - App->render->camera.y), &atlasRect);
	else if(focused)
		ret = App->render->Blit(atlas, (int)(this->screenPos.x - App->render->camera.x), (int)(this->screenPos.y - App->render->camera.y), &atlasRect);

	SDL_SetTextureAlphaMod(atlas, 255);

	if (ret)
		ret = DrawChilds();

	return ret;
}