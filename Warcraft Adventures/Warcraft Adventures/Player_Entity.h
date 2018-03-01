#include "Entity.h"

enum PLAYER_STATES {

};

enum PLAYER_ACTIONS {

};

class PlayerPJ : public DinamicEntity {
public:
	PlayerPJ(iPoint coor, ENTITY_TYPE type, SDL_Texture* texture, PLAYER_TYPE character);
	void DoAction(PLAYER_ACTIONS action, FIXED_ANGLE angle);
	void UseConsumable();

	void Start();
	void Finish();
	void Draw();

	void Update();
	void Move();

public:
	PLAYER_STATES state;
	CONSUMABLE_TYPE consumable;
	PLAYER_TYPE character_name;
	
	int skillCooldown;
};