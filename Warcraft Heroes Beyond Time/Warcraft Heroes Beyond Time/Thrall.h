#ifndef __THRALL_H__
#define __THRALL_H__

#include "PlayerEntity.h"


class Collider;

class Thrall : public PlayerEntity {
private:
public:
	Thrall(fPoint coor, PLAYER_TYPE type, SDL_Texture* texture);

	bool Update(float dt);
	bool Finish();
	void Collision(Collider* collideWith);

private:
	void UpdateCollider();
	void UpdateAttackCollider();
	void Attack();
	
private:
	Collider * attackCollider = nullptr;
	bool attacking = false;
};

#endif