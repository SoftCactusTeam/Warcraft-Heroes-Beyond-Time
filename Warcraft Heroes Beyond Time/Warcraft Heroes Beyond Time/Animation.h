#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include <vector>

#include "SDL/include/SDL_timer.h"
#include "SDL/include/SDL_rect.h"

#include "p2Point.h"

#include "Globals.h"

#define MAX_FRAMES 45

class Animation
{
private:
	int loops = 0;
	float current_frame = 0.0f;
	int last_frame = 0;
	bool firstTimeInFrame = true;

	SDL_Rect frames[MAX_FRAMES];
	iPoint pivots[MAX_FRAMES];

public:
	float speed = 0.0f;
	float speedFactor = 0.0f;
	bool loop = true;

public:

	uint getFrames()
	{
		return last_frame;
	}

	void PushBack(const SDL_Rect& rect, iPoint pivot = {0,0})
	{
		pivots[last_frame] = pivot;
		frames[last_frame++] = rect;
	}

	SDL_Rect& GetCurrentFrame()
	{
		int prev_frame = (int)current_frame;
		current_frame += speed;

		if (current_frame >= last_frame)
		{
			loops++;
			current_frame = (loop) ? 0.0f : last_frame - 1;
		}

		if (firstTimeInFrame)
			firstTimeInFrame = false;
		else if (current_frame == 0.0f)
			firstTimeInFrame = true;
		else if (!firstTimeInFrame && (int)current_frame > prev_frame)
			firstTimeInFrame = true;


		return frames[(int)current_frame];
	}

	iPoint GetCurrentPivot() const
	{
		return pivots[(int)current_frame];
	}

	SDL_Rect& GetCurrentRect()
	{
		return frames[(int)current_frame];
	}

	bool Finished() const
	{
		return loops > 0;
	}

	bool FirstTimeInFrame()
	{
		return firstTimeInFrame;
	}

	void Reset()
	{
		loops = 0;
		current_frame = 0;
	}

	void Start(float speedFactor)
	{
		this->speedFactor = speedFactor;
	}

	void Stop()
	{
		speedFactor = 0.0f;
		speed = 0.0f;
	}
};

class TimeBasedAnimation
{
private:
	int loops = 0;
	float current_frame = 0.0f;
	int last_frame = 0;

	SDL_Rect frames[MAX_FRAMES];
	iPoint pivots[MAX_FRAMES];

public:
	
	float life = 0.0f;
	float curr_frame_time = 0.0f;
	bool loop = true;

public:

	uint getFrames()
	{
		return last_frame;
	}

	void PushBack(const SDL_Rect& rect, iPoint pivot = { 0,0 })
	{
		pivots[last_frame] = pivot;
		frames[last_frame++] = rect;
	}

	SDL_Rect& GetCurrentFrame(float dt)
	{
		curr_frame_time += dt;
		
		if (curr_frame_time > life / last_frame)
		{
			curr_frame_time = 0;
			current_frame += 1;

			if (current_frame >= last_frame)
			{
				loops++;
				current_frame = (loop) ? 0.0f : last_frame - 1;
			}
		}

		return frames[(int)current_frame];
	}

	iPoint GetCurrentPivot() const
	{
		return pivots[(int)current_frame];
	}

	SDL_Rect& GetCurrentRect()
	{
		return frames[(int)current_frame];
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void Reset()
	{
		loops = 0;
		current_frame = 0;
		curr_frame_time = 0;
	}
};



#endif