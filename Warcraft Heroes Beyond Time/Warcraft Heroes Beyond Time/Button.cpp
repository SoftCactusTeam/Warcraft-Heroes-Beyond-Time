#include "Application.h"
#include "ModuleRender.h"
#include "Button.h"


Button::Button(fPoint localPos, BType btype, GUIElem* parent, Module* listener) : GUIElem(localPos, listener, Button1, GUIElemType::BUTTON, parent), btype(btype) {}

Button::~Button()
{
	DestroyChilds();
}

bool Button::Update(float dt)
{
	bool result = true;

	if(result)
		result = UpdateChilds(dt);

	if (result)
		result = HandleInput();
	
	return result;
}

bool Button::Draw()
{
	bool result = true;

	result = App->render->Blit(App->gui->getAtlas(), (int)(this->screenPos.x - App->render->camera.x), (int)(this->screenPos.y - App->render->camera.y), &atlasRect);

	if (result)
		result = DrawChilds();
	
	return result;
}

void Button::MoveChilds(fPoint dist)
{
	std::list<GUIElem*>::iterator it;
	for (it = childs.begin(); it != childs.end(); ++it)
	{
		(*it)->Move(dist);
	}
}

