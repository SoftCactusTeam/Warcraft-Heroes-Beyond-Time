#include "Application.h"
#include "ModuleRender.h"
#include "Button.h"


Button::Button(fPoint localPos, GUIElem* parent, Module* listener) : GUIElem(localPos, listener, Button1, GUIElemType::BUTTON, parent) {}

Button::~Button()
{
	std::list<GUIElem*>::iterator it;
	for (it = childs.begin(); it != childs.end(); ++it)
	{
		delete (*it);
	}
	childs.clear();
}

bool Button::Update(float dt)
{
	bool result = true;

	result = App->render->Blit(App->gui->getAtlas(), (int)(this->screenPos.x + App->render->camera.x), (int)(this->screenPos.y + App->render->camera.y), &atlasRect);
	if(result)
		result = UpdateChilds(dt);

	if (result)
		HandleInput();
	
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

