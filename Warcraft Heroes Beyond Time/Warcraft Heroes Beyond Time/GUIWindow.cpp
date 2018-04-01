#include "GUIWindow.h"
#include "Application.h"
#include "ModuleRender.h"

GUIWindow::GUIWindow(fPoint localPos, SDL_Rect atlasRect, GUIElem* parent, Module* listener) : GUIElem(localPos, listener, atlasRect, GUIElemType::WINDOW, parent)
{
	background.x = 0;
	background.y = 0;
	background.w = 640;
	background.h = 360;
}

GUIWindow::~GUIWindow()
{
	DestroyChilds();
}

bool GUIWindow::Update(float dt)
{
	bool result = true;

	
	if(result)
		result = UpdateChilds(dt);

	return result;
}

bool GUIWindow::Draw()
{
	bool result = true;

	background.x = -App->render->camera.x;
	background.y = -App->render->camera.y;

	result = App->render->DrawQuad(background, 0, 0, 0, 200, true, true);

	if (result)
		result = App->render->Blit(App->gui->getAtlas(), (int)(this->screenPos.x - App->render->camera.x), (int)(this->screenPos.y - App->render->camera.y), &atlasRect);
	
	if (result)
		result = DrawChilds();

	return result;

}