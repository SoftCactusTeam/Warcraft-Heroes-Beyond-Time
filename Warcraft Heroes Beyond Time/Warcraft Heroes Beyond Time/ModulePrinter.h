#ifndef __MODULEPRINTER_H__
#define __MODULEPRINTER_H__

#include "Module.h"
#include "Application.h"
#include "p2Point.h"

#include <queue>

class SDL_Texture;

enum Layers
{
	FLOOR = -2,
	FLOOR_EFFECTS = -1,
	ENTITIES = 0,
	ROOFS = 1,
	ARCHER_EFFECTS_ARROWS = 2
};


class DrawingElem
{
public:
	enum class DElemType
	{
		NONE = -1,
		SPRITE,
		QUAD
	} type = DElemType::NONE;
	
public:

	DrawingElem(DElemType type) : type(type) {}
};

class Sprite : public DrawingElem
{
public:

	iPoint pos = { 0,0 };
	SDL_Texture* texture = nullptr;
	SDL_Rect SquaretoBlit = { 0,0,0,0 };
	int layer = 0;
	iPoint pivot = { 0,0 };
	float angle = 0.0f;
	int distToFeet = 0;
	SDL_Color color = { 0,0,0,0 };
	iPoint offset = {0,0};
	SDL_BlendMode blendMode = SDL_BLENDMODE_NONE;
	SDL_Rect rectSize = { 0, 0, 0, 0 };
	float speed = 1.0f;

public:

	Sprite(iPoint& pos, SDL_Texture* texture, SDL_Rect& SquaretoBlit, int layer, iPoint offset, iPoint pivot, float angle, float speed, SDL_Color color, SDL_BlendMode blendMode, SDL_Rect rectSize) : DrawingElem(DrawingElem::DElemType::SPRITE), pos(pos), texture(texture), SquaretoBlit(SquaretoBlit), distToFeet(distToFeet), layer(layer), offset(offset), pivot(pivot), angle(angle), color(color)
	{
		this->blendMode = blendMode;
		this->rectSize = rectSize;
		this->speed = speed;
	}

};

class Quad : public DrawingElem
{
public:

	SDL_Rect rect;
	SDL_Color color;
	bool filled;
	bool use_camera;

public:

	Quad(SDL_Rect rect, SDL_Color color, bool filled, bool use_camera) : DrawingElem(DrawingElem::DElemType::QUAD), rect(rect), color(color), filled(filled), use_camera(use_camera) {}

};

class Compare
{
public:
	bool operator () (DrawingElem* first, DrawingElem* second)
	{
		if (first->type == DrawingElem::DElemType::SPRITE && second->type == DrawingElem::DElemType::SPRITE)
		{
			Sprite* first_Sprite = (Sprite*)first;
			Sprite* second_Sprite = (Sprite*)second;
			if (first_Sprite->layer != second_Sprite->layer)
				return first_Sprite->layer > second_Sprite->layer;
			else
				return first_Sprite->pos.y + first_Sprite->SquaretoBlit.h > second_Sprite->pos.y + second_Sprite->SquaretoBlit.h;
		}
		else if (first->type == DrawingElem::DElemType::QUAD && second->type == DrawingElem::DElemType::QUAD)
		{
			Quad* firstQuad = (Quad*) first;
			Quad* secondQuad = (Quad*)second;
			return firstQuad->rect.y + firstQuad->rect.h > secondQuad->rect.y + secondQuad->rect.h;
		}
		else if (first->type == DrawingElem::DElemType::QUAD && second->type == DrawingElem::DElemType::SPRITE)
		{
			return true;
		}
		else if (first->type == DrawingElem::DElemType::SPRITE && second->type == DrawingElem::DElemType::QUAD)
		{
			return false;
		}
		
	}
};


class ModulePrinter : public Module
{
public:

	ModulePrinter()
	{
		name = "printer";
	}

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

public:

	enum class Pivots
	{
		UPPER_LEFT,
		UPPER_CENTER,
		UPPER_RIGHT,
		CENTER_LEFT,
		CENTER,
		CENTER_RIGHT,
		LOWER_LEFT,
		LOWER_CENTER,
		LOWER_RIGHT,
		CUSTOM_PIVOT
	};

	//Note: Angle required is in degrees, in clockwise direction
	bool PrintSprite(iPoint pos, SDL_Texture* texture, SDL_Rect SquaretoBlit, int layer = 0, Pivots OFFSET_MODE = Pivots::UPPER_LEFT, iPoint customOffset = { 0,0 }, Pivots PIVOT_MODE = Pivots::UPPER_LEFT, iPoint customPivot = { 0,0 }, float degangle = 0.0f, SDL_Color color = { 255,255,255,255 }, SDL_BlendMode blendMode = SDL_BLENDMODE_NONE, SDL_Rect rectSize = { 0, 0, 0, 0 }, float speed = 1.0f);
	
	bool PrintQuad(SDL_Rect rect, SDL_Color color, bool filled = false, bool use_camera = false);

private:

	std::priority_queue<DrawingElem*, std::vector<DrawingElem*>, Compare> DrawingQueue;
};



#endif