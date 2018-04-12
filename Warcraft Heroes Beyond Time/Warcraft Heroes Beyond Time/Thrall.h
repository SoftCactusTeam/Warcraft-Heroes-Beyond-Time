#ifndef __THRALL_H__
#define __THRALL_H__

#include "PlayerEntity.h"


class Collider;

class Thrall : public PlayerEntity {
private:
public:
	Thrall(fPoint coor, PLAYER_TYPE type, SDL_Texture* texture);

	bool Update(float dt);
	bool PostUpdate();
	bool Finish();
	void Collision(Collider* collideWith);

private:
	void UpdateCollider();
	void UpdateAttackCollider();
	void Attack();
	void UseSkill();
	void UpdateSkillCollider();

private:
	Collider * attackCollider = nullptr;
	Collider* skillCollider = nullptr;
	bool attacking = false;
	bool skillOn = false;

public:

	//items
	std::list<iPoint> wcpaper;
	std::list<Collider*> paper_collider;
	
	//time
	float time = 0.0f;
	int cont = 0;
};

#endif