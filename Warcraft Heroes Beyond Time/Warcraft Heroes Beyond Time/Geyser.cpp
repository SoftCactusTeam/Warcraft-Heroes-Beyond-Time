#include "Geyser.h"

#include "Application.h"
#include "ModulePrinter.h"

Geyser::Geyser(const GeyserInfo* info, Projectile_type type) : Projectile(info, type)
{
	geyAnims[(uint)GeyserAnimations::moving_anim].PushBack({ 21,33,14,14 });

	actualAnim = &geyAnims[(uint)GeyserAnimations::moving_anim];

	toData = (GeyserInfo*)info;
}

Geyser::~Geyser()
{
	RELEASE(data);
}

bool Geyser::Update(float dt)
{
	bool ret = true;

	return ret;
}

bool Geyser::Draw() const
{
	bool ret = true;

	ret = App->printer->PrintSprite({ (int)data->pos.x - 7,(int)data->pos.y - 7 }, (SDL_Texture*)App->projectiles->GetProjectileAtlas(), actualAnim->GetCurrentFrame(), data->layer, ModulePrinter::Pivots::UPPER_LEFT);

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
