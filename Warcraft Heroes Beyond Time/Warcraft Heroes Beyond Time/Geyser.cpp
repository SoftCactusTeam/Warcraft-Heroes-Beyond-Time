#include "Geyser.h"

#include "Application.h"
#include "ModulePrinter.h"
#include "ModuleColliders.h"

#include "Scene.h"
#include "PlayerEntity.h"

Geyser::Geyser(const GeyserInfo* info, Projectile_type type) : Projectile(info, type)
{
	geyAnims[(uint)GeyserAnimations::explosion].PushBack({ 297,3,49,74 });
	geyAnims[(uint)GeyserAnimations::explosion].PushBack({ 346,3,49,74 });
	geyAnims[(uint)GeyserAnimations::explosion].PushBack({ 395,3,49,74 });
	geyAnims[(uint)GeyserAnimations::explosion].PushBack({ 444,3,49,74 });
	geyAnims[(uint)GeyserAnimations::explosion].PushBack({ 3,79,49,74 });
	geyAnims[(uint)GeyserAnimations::explosion].PushBack({ 52,79,49,74 });
	geyAnims[(uint)GeyserAnimations::explosion].PushBack({ 101,79,49,74 });
	geyAnims[(uint)GeyserAnimations::explosion].PushBack({ 150,79,49,74 });
	geyAnims[(uint)GeyserAnimations::explosion].PushBack({ 199,79,49,74 });
	geyAnims[(uint)GeyserAnimations::explosion].PushBack({ 248,79,49,74 });
	geyAnims[(uint)GeyserAnimations::explosion].PushBack({ 297,79,49,74 });
	geyAnims[(uint)GeyserAnimations::explosion].PushBack({ 346,79,49,74 });
	geyAnims[(uint)GeyserAnimations::explosion].PushBack({ 395,79,49,74 });
	geyAnims[(uint)GeyserAnimations::explosion].PushBack({ 444,79,49,74 });
	geyAnims[(uint)GeyserAnimations::explosion].PushBack({ 3,155,49,74 });
	geyAnims[(uint)GeyserAnimations::explosion].PushBack({ 52,155,49,74 });
	geyAnims[(uint)GeyserAnimations::explosion].PushBack({ 101,155,49,74 });
	geyAnims[(uint)GeyserAnimations::explosion].PushBack({ 150,155,49,74 });
	geyAnims[(uint)GeyserAnimations::explosion].PushBack({ 199,155,49,74 });
	geyAnims[(uint)GeyserAnimations::explosion].PushBack({ 248,155,49,74 });
	geyAnims[(uint)GeyserAnimations::explosion].speedFactor = 9.0f;
	geyAnims[(uint)GeyserAnimations::explosion].loop = false;

	geyAnims[(uint)GeyserAnimations::selection].PushBack({ 297,155,49,74 });
	geyAnims[(uint)GeyserAnimations::selection].PushBack({ 346,155,49,74 });
	geyAnims[(uint)GeyserAnimations::selection].PushBack({ 395,155,49,74 });
	geyAnims[(uint)GeyserAnimations::selection].PushBack({ 444,155,49,74 });
	geyAnims[(uint)GeyserAnimations::selection].speedFactor = 9.0f;

	toData = (GeyserInfo*)info;

	projCollider = *App->colliders->AddEnemyAttackCollider({0, 0, 49, 74}, this, 50, EnemyAttack::E_Attack_Type::GULDAN_BALL).lock();

	actualAnim = &geyAnims[(uint)GeyserAnimations::selection];
	geyser_state = state::following;
	data->layer = 0;
}

Geyser::~Geyser()
{
	App->colliders->deleteCollider(projCollider);
	RELEASE(data);
}

bool Geyser::Update(float dt)
{
	bool ret = true;

	switch (toData->geyser_movement)
	{

	case GeyserInfo::follow_player:

		switch (geyser_state)
		{
		case state::following:

			data->pos = App->scene->player->pos;
			timeUntilExplode += 1.0f * dt;

			if (timeUntilExplode >= TIMEFOLLOWINGPLAYER)
			{
				geyser_state = state::stop;
				timeUntilExplode = 0.0f;
			}

			break;

		case state::stop:

			timeUntilExplode += 1.0f * dt;

			if (timeUntilExplode >= TIMEUNTILEXPLODE)
			{
				data->layer = 5;
				actualAnim = &geyAnims[(uint)GeyserAnimations::explosion];
				geyser_state = state::explosion;
			}
			break;

		case state::explosion:

			if (actualAnim->Finished())
				App->projectiles->DestroyProjectile(this);

			break;
		}

		break;

	case GeyserInfo::stay_in_pos:

		switch (geyser_state)
		{
		case state::following:

			timeUntilExplode += 1.0f * dt;

			if (timeUntilExplode >= TIMEUNTILSTOPGEYSEREXPLODE)
			{
				geyser_state = state::explosion;
				actualAnim = &geyAnims[(uint)GeyserAnimations::explosion];
				timeUntilExplode = 0.0f;
				data->layer = 5;
			}

			break;

		case state::explosion:

			if (actualAnim->Finished())
				App->projectiles->DestroyProjectile(this);

			break;
		}

		break;
	}

	actualAnim->speed = actualAnim->speedFactor * dt;

	return ret;
}

bool Geyser::Draw() const
{
	bool ret = true;

	ret = App->printer->PrintSprite({ (int)data->pos.x - 15,(int)data->pos.y - 20 }, (SDL_Texture*)App->projectiles->GetProjectileAtlas(), actualAnim->GetCurrentFrame(), data->layer, ModulePrinter::Pivots::UPPER_LEFT);

	return ret;
}

void Geyser::OnCollision(Collider* yours, Collider* collideWith)
{
}

void Geyser::OnCollisionContinue(Collider* yours, Collider* collideWith)
{
}

void Geyser::OnCollisionLeave(Collider* yours, Collider* collideWith)
{
}
