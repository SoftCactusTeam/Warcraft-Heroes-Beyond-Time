#ifndef __THRALL_H__
#define __THRALL_H__

#include "PlayerEntity.h"

class Thrall : public PlayerEntity {
private:
public:
	Thrall(fPoint coor, PLAYER_TYPE type, SDL_Texture* texture);

	bool Update(float dt);
	bool Finish();

<<<<<<< HEAD
	void Collision(COLLIDER_TYPE type);
=======
private:
	void UpdateCollider();
	void UpdateAttackCollider();
	void Attack();
	void UseSkill();
	void UpdateSkillCollider();
	
private:
	Collider* attackCollider = nullptr;
	Collider* skillCollider = nullptr;
	bool attacking = false;
	bool skillOn = false;
>>>>>>> master
};

#endif