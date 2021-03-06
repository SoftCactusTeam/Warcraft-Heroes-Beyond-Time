#ifndef __LABEL_H__
#define __LABEL_H__

#include <string>
#include "GUIElem.h"
#include "Globals.h"
#include "ModuleAudio.h"

struct LabelInfo 
{
	char* text = nullptr;
	char* fontName = nullptr;
	SDL_Color color = Black;
	uint multilabelWidth = 500;
};

class Label : public GUIElem
{
public:
	Label() {}
	Label(fPoint position, LabelInfo& info, GUIElem* parent, Module* listener);
	~Label();
	bool Update(float dt);
	bool Draw();
	bool MouseHover() const;
	void EditText(std::string text, SDL_Color color = {0,0,0,0});

	inline const char* getText() const
	{
		return text.data();
	}

	inline const SDL_Texture* getTexturetoBlit() const
	{
		return texturetoBlit;
	}

	inline void setLocalPos(fPoint newPos) 
	{
		localPos = {newPos.x != -1 ? newPos.x : localPos.x, newPos.y != -1 ? newPos.y : localPos.y };
		calculateScreenPos();
	}

	void ResetRebind();

public:
	static int ButtonPressed;
	static bool waitingBindInput;
private:

	void Focus()
	{
		focused = true;
		App->audio->HaltFX(App->audio->ButtonHovered);
		App->audio->PlayFx(App->audio->ButtonHovered);
	}

	std::string text;
	TTF_Font* font = nullptr;
	SDL_Texture* texturetoBlit = nullptr;
	SDL_Color color;
	uint multilabelWidth = 0;
};

#endif