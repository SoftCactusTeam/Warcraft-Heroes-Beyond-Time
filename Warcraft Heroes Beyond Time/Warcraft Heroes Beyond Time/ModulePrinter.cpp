#include "ModulePrinter.h"
#include "ModuleRender.h"

#include "Brofiler\Brofiler.h"

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
	BROFILER_CATEGORY("Printer Blit", Profiler::Color::Azure);

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

bool ModulePrinter::PrintSprite(iPoint pos, SDL_Texture* texture, SDL_Rect SquaretoBlit, int layer)
{
	Sprite* sprite = new Sprite(pos, texture, SquaretoBlit, layer);
	SpriteQueue.push(sprite);

	return true;
}