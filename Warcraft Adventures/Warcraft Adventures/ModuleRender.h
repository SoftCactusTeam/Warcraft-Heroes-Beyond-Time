#ifndef __RENDER_H__
#define __RENDER_H__

#include "Module.h"

#include "p2Point.h"

#include "SDL/include/SDL.h"

class Render : public Module
{
public:

	Render();
	virtual ~Render();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();

	void SetViewPort(const SDL_Rect& rect);
	void ResetViewPort();
	bool Blit(const SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, float speed = 1.0f, double angle = 0, int pivot_x = INT_MAX, int pivot_y = INT_MAX) const;
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool use_camera = true) const;
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;
	bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;
	void SetBackgroundColor(SDL_Color color);

public:

	SDL_Renderer*	renderer;
	SDL_Rect		camera;
	SDL_Rect		viewport;
	SDL_Color		background;
	bool			vsync = false;
	
};

#endif