#ifndef __ChestEntity_H__
#define __ChestEntity_H__

#include "Entity.h"

class Chest_Entiy : public StaticEntity {
public:
	Chest_Entiy(iPoint coor, ENTITY_TYPE type, SDL_Texture* texture);

	void Start();
	void Finish();
	void Draw();

	void Interaction();
};

#endif