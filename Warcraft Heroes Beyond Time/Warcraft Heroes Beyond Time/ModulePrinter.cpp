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
		App->render->Blit(sprite->texture, sprite->pos.x, sprite->pos.y, &sprite->SquaretoBlit, 1, 1, sprite->angle, sprite->pivot.x, sprite->pivot.y);

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

bool ModulePrinter::PrintSprite(iPoint pos, SDL_Texture* texture, SDL_Rect SquaretoBlit, int layer, Pivots pivot, float degangle, iPoint custompivot)
{
	iPoint coordpivot;
	switch (pivot)
	{
		case Pivots::UPPER_LEFT:
		{
			coordpivot = { 0,0 };
			break;
		}
		case Pivots::UPPER_CENTER:
		{
			coordpivot = { SquaretoBlit.w / 2, 0 };
			break;
		}
		case Pivots::UPPER_RIGHT:
		{
			coordpivot = { SquaretoBlit.w, 0 };
			break;
		}
		case Pivots::CENTER_LEFT:
		{
			coordpivot = { 0, SquaretoBlit.h / 2 };
			break;
		}
		case Pivots::CENTER:
		{
			coordpivot = { SquaretoBlit.w / 2, SquaretoBlit.h / 2 };
			break;
		}
		case Pivots::CENTER_RIGHT:
		{
			coordpivot = { SquaretoBlit.w, SquaretoBlit.h / 2 };
			break;
		}
		case Pivots::LOWER_LEFT:
		{
			coordpivot = { 0, SquaretoBlit.h };
			break;
		}
		case Pivots::LOWER_CENTER:
		{
			coordpivot = { SquaretoBlit.w / 2, SquaretoBlit.h };
			break;
		}
		case Pivots::LOWER_RIGHT:
		{
			coordpivot = { SquaretoBlit.w, SquaretoBlit.h };
			break;
		}
		case Pivots::CUSTOM_PIVOT:
		{
			coordpivot = custompivot;
			break;
		}
	}

	Sprite* sprite = new Sprite(pos, texture, SquaretoBlit, layer, coordpivot, degangle);
	SpriteQueue.push(sprite);

	return true;
}