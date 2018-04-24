#ifndef __EFFECTS_ELEM_H__
#define __EFFECTS_ELEM_H__

#include "p2Point.h"
#include "ModuleEffects.h"
#include "Animation.h"
#include <list>

class Module;
class ModuleEffects;

class EffectsElem
{
public:

protected:
	fPoint pos = { 0.0f, 0.0f };
	float timer = 0.0f;
	Animation effectAnim;

public:

public:
	EffectsElem() {}
	virtual ~EffectsElem() {};

	EffectsElem(fPoint pos, Animation effectAnim);
	bool DrawEffects();
	bool UpdateEffects(float dt);
	void MoveEffect(fPoint pos);

};

#endif // __EFFECTS_ELEM_H__