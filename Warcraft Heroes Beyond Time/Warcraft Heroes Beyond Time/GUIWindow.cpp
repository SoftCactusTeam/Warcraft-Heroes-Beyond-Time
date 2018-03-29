#include "GUIWindow.h"
#include "Application.h"
#include "ModuleRender.h"

GUIWindow::GUIWindow(fPoint localPos, SDL_Rect atlasRect, GUIElem* parent, Module* listener) : GUIElem(localPos, listener, atlasRect, GUIElemType::WINDOW, parent){}

GUIWindow::~GUIWindow()
{
	DestroyChilds();
}

bool GUIWindow::Update(float dt)
{
	bool result = true;

	result = App->render->Blit(App->gui->getAtlas(), (int)(this->screenPos.x - App->render->camera.x), (int)(this->screenPos.y - App->render->camera.y), &atlasRect);
	
	if(result)
		result = UpdateChilds(dt);

	return result;
}