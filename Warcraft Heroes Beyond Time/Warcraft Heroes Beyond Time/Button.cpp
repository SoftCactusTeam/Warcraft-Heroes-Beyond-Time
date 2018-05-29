#include "Application.h"
#include "ModuleRender.h"
#include "Button.h"
#include "Label.h"
#include "FileSystem.h"


Button::Button(fPoint localPos, BType btype, GUIElem* parent, Module* listener) : GUIElem(localPos, listener, Button1, GUIElemType::BUTTON, parent), btype(btype) 
{
	
}

Button::~Button()
{
	DestroyChilds();
}

bool Button::Update(float dt)
{
	bool result = true;

	if (btype == BType::CONTINUE)
	{
		Label* continuelabel = (Label*)childs.front();

		if (continuelabel)
			if (App->fs->isGameSaved())
				continuelabel->EditText(continuelabel->getText(), White);
			else
				continuelabel->EditText(continuelabel->getText(), Grey);
	}

	if(result)
		result = UpdateChilds(dt);

	if (result)
		result = HandleInput(dt);
	
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

