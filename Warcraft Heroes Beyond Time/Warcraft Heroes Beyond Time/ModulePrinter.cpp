#include "ModulePrinter.h"
#include "ModuleRender.h"

bool ModulePrinter::Awake(pugi::xml_node&)
{
	return true;
}
bool ModulePrinter::Start()
{
	return true;
}
bool ModulePrinter::PreUpdate()
{
	return true;
}
bool ModulePrinter::Update(float dt)
{
	return true;
}
bool ModulePrinter::PostUpdate()
{
	while (!SpriteQueue.empty())
	{
		Sprite* sprite = SpriteQueue.top();
		App->render->Blit(sprite->texture, sprite->pos.x, sprite->pos.y, &sprite->SquaretoBlit);

		SpriteQueue.pop();
		delete sprite;
	}
	return true;
}
bool ModulePrinter::CleanUp()
{
	while (!SpriteQueue.empty())
	{
		Sprite* sprite = SpriteQueue.top();
		SpriteQueue.pop();
		delete sprite;
	}
	return true;
}

bool ModulePrinter::PrintSprite(iPoint pos, SDL_Texture* texture, SDL_Rect SquaretoBlit)
{
	Sprite* sprite = new Sprite(pos, texture, SquaretoBlit);
	SpriteQueue.push(sprite);

	return true;
}