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

	while (!DrawingQueue.empty())
	{
		DrawingElem* delem = DrawingQueue.top();
		switch (delem->type)
		{
			case DrawingElem::DElemType::SPRITE:
			{
				Sprite* sprite = (Sprite*)delem;
				App->render->Blit(sprite->texture, sprite->pos.x - sprite->pivot.x, sprite->pos.y - sprite->pivot.y, &sprite->SquaretoBlit, 1, 1, sprite->angle, sprite->pivot.x, sprite->pivot.y);
				break;
			}
			case DrawingElem::DElemType::QUAD:
			{
				Quad* quad = (Quad*)delem;
				App->render->DrawQuad(quad->rect, quad->color.r, quad->color.g, quad->color.b, quad->color.a, quad->filled, quad->use_camera);
			}
		}
	
		DrawingQueue.pop();
		delete delem;
	}
	return true;
}

bool ModulePrinter::CleanUp()
{
	while (!DrawingQueue.empty())
	{
		DrawingElem* delem = DrawingQueue.top();
		DrawingQueue.pop();
		delete delem;
	}
	return DrawingQueue.empty();
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
	DrawingQueue.push(sprite);

	return true;
}

bool ModulePrinter::PrintQuad(SDL_Rect rect, SDL_Color color, bool filled, bool use_camera)
{
	Quad* quad = new Quad(rect, color, filled, use_camera);
	DrawingQueue.push(quad);

	return true;
}