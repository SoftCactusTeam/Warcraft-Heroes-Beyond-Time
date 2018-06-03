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
				SDL_SetTextureColorMod(sprite->texture, sprite->color.r, sprite->color.g, sprite->color.b);
				SDL_SetTextureAlphaMod(sprite->texture, sprite->color.a);
				App->render->Blit(sprite->texture, sprite->pos.x - sprite->offset.x, sprite->pos.y - sprite->offset.y, &sprite->SquaretoBlit, 1, sprite->speed, sprite->angle, sprite->pivot.x, sprite->pivot.y, false, sprite->blendMode, sprite->rectSize);
				//SDL_SetTextureColorMod(sprite->texture, 255, 255, 255);
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

bool ModulePrinter::PrintSprite(iPoint pos, SDL_Texture* texture, SDL_Rect SquaretoBlit, int layer, Pivots OFFSET_MODE, iPoint customOffset, Pivots PIVOT_MODE, iPoint customPivot, float degangle, SDL_Color color, SDL_BlendMode blendMode, SDL_Rect rectSize, float speed)
{
	iPoint offset;
	iPoint pivot;
	switch (OFFSET_MODE)
	{
		case Pivots::UPPER_LEFT:
		{
			offset = { 0,0 };
			break;
		}
		case Pivots::UPPER_CENTER:
		{
			offset = { SquaretoBlit.w / 2, 0 };
			break;
		}
		case Pivots::UPPER_RIGHT:
		{
			offset = { SquaretoBlit.w, 0 };
			break;
		}
		case Pivots::CENTER_LEFT:
		{
			offset = { 0, SquaretoBlit.h / 2 };
			break;
		}
		case Pivots::CENTER:
		{
			offset = { SquaretoBlit.w / 2, SquaretoBlit.h / 2 };
			break;
		}
		case Pivots::CENTER_RIGHT:
		{
			offset = { SquaretoBlit.w, SquaretoBlit.h / 2 };
			break;
		}
		case Pivots::LOWER_LEFT:
		{
			offset = { 0, SquaretoBlit.h };
			break;
		}
		case Pivots::LOWER_CENTER:
		{
			offset = { SquaretoBlit.w / 2, SquaretoBlit.h };
			break;
		}
		case Pivots::LOWER_RIGHT:
		{
			offset = { SquaretoBlit.w, SquaretoBlit.h };
			break;
		}
		case Pivots::CUSTOM_PIVOT:
		{
			offset = customOffset;
			break;
		}
	}

	switch (PIVOT_MODE)
	{
		case Pivots::UPPER_LEFT:
		{
			pivot = { 0,0 };
			break;
		}
		case Pivots::UPPER_CENTER:
		{
			pivot = { SquaretoBlit.w / 2, 0 };
			break;
		}
		case Pivots::UPPER_RIGHT:
		{
			pivot = { SquaretoBlit.w, 0 };
			break;
		}
		case Pivots::CENTER_LEFT:
		{
			pivot = { 0, SquaretoBlit.h / 2 };
			break;
		}
		case Pivots::CENTER:
		{
			pivot = { SquaretoBlit.w / 2, SquaretoBlit.h / 2 };
			break;
		}
		case Pivots::CENTER_RIGHT:
		{
			pivot = { SquaretoBlit.w, SquaretoBlit.h / 2 };
			break;
		}
		case Pivots::LOWER_LEFT:
		{
			pivot = { 0, SquaretoBlit.h };
			break;
		}
		case Pivots::LOWER_CENTER:
		{
			pivot = { SquaretoBlit.w / 2, SquaretoBlit.h };
			break;
		}
		case Pivots::LOWER_RIGHT:
		{
			pivot = { SquaretoBlit.w, SquaretoBlit.h };
			break;
		}
		case Pivots::CUSTOM_PIVOT:
		{
			pivot = customPivot;
			break;
		}
	}


	Sprite* sprite = new Sprite(pos, texture, SquaretoBlit, layer, offset, pivot, degangle, speed, color, blendMode, rectSize);
	DrawingQueue.push(sprite);

	return true;
}

bool ModulePrinter::PrintQuad(SDL_Rect rect, SDL_Color color, bool filled, bool use_camera)
{
	Quad* quad = new Quad(rect, color, filled, use_camera);
	DrawingQueue.push(quad);

	return true;
}