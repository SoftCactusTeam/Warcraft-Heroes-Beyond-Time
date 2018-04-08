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

	SDL_Rect frames[MAX_FRAMES];
	iPoint pivots[MAX_FRAMES];

public:
	float speed = 0.0f;
	float speedFactor = 0.0f;
	bool loop = true;

public:

	void PushBack(const SDL_Rect& rect, iPoint pivot = {0,0})
	{
		pivots[last_frame] = pivot;
		frames[last_frame++] = rect;
	}

	SDL_Rect& GetCurrentFrame()
	{
		current_frame += speed;
		if (current_frame >= last_frame)
		{
			loops++;
			current_frame = (loop) ? 0.0f : last_frame - 1;
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

// -----------------------------------------------------------------------------------------------------------------------------------------
// -------------------------------------------------- ADVANCED ANIMATION -------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------

struct KeyFrame
{
	SDL_Rect animationRect;
	uint timeOfLive;
};

class AdvancedAnimation
{
public:
	void AddFrame(SDL_Rect rect, int timeOfLive = -1, int concretePositon = -1)
	{
		KeyFrame* newFrame = new KeyFrame();
		newFrame->animationRect = rect;
		newFrame->timeOfLive = timeOfLive;
		if (timeOfLive == -1)
			newFrame->timeOfLive = generalSpeed;
		else
			newFrame->timeOfLive = timeOfLive;
		if (concretePositon == -1)
			animationFrames.push_back(newFrame);
		else {
			if (concretePositon < animationFrames.capacity())
			{
				std::vector<KeyFrame*> newVectorFrame(1, newFrame);
				animationFrames.insert(animationFrames.begin() + concretePositon, newVectorFrame.begin(), newVectorFrame.end());
			}
			else
				animationFrames.push_back(newFrame);
		}
	}

	SDL_Rect GetCurrentFrame()
	{
		if (generalSpeed == 0)
			return animationFrames[currentFrame]->animationRect;
		if (currentTime < SDL_GetTicks()) {
			currentFrame++;
			if (currentFrame >= animationFrames.size())
				if (loop)
					currentFrame = 0;
			currentTime = SDL_GetTicks() + animationFrames[currentFrame]->timeOfLive;
		}
		return animationFrames[currentFrame]->animationRect;
	}

	bool Finished() const
	{
		if (currentFrame >= animationFrames.size() - 1)
			return true;
		else
			return false;
	}

	void CleanAnimation() {
		for (int i = 0; i < animationFrames.size(); i++) {
			delete animationFrames[i];
		}
		animationFrames.clear();
	}

	void Reset() { currentFrame = 0; }
	void Start(int newGeneralSpeed) { generalSpeed = newGeneralSpeed; }
	void Stop() { generalSpeed = 0; }

private:
	std::vector <KeyFrame*> animationFrames;
	uint currentFrame = 0;
	uint currentTime = 0;

public:
	uint generalSpeed = 0;
	bool loop = true;
};

#endif