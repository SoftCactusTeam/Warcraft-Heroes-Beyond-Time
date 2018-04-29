#include "Geyser.h"

#include "Application.h"
#include "ModulePrinter.h"

Geyser::Geyser(const GeyserInfo* info, Projectile_type type) : Projectile(info, type)
{
	geyAnims[(uint)GeyserAnimations::moving_anim].PushBack({ 297,3,49,74 });
	geyAnims[(uint)GeyserAnimations::moving_anim].PushBack({ 346,3,49,74 });
	geyAnims[(uint)GeyserAnimations::moving_anim].PushBack({ 395,3,49,74 });
	geyAnims[(uint)GeyserAnimations::moving_anim].PushBack({ 444,3,49,74 });
	geyAnims[(uint)GeyserAnimations::moving_anim].PushBack({ 3,79,49,74 });
	geyAnims[(uint)GeyserAnimations::moving_anim].PushBack({ 52,79,49,74 });
	geyAnims[(uint)GeyserAnimations::moving_anim].PushBack({ 101,79,49,74 });
	geyAnims[(uint)GeyserAnimations::moving_anim].PushBack({ 150,79,49,74 });
	geyAnims[(uint)GeyserAnimations::moving_anim].PushBack({ 199,79,49,74 });
	geyAnims[(uint)GeyserAnimations::moving_anim].PushBack({ 248,79,49,74 });
	geyAnims[(uint)GeyserAnimations::moving_anim].PushBack({ 297,79,49,74 });
	geyAnims[(uint)GeyserAnimations::moving_anim].PushBack({ 346,79,49,74 });
	geyAnims[(uint)GeyserAnimations::moving_anim].PushBack({ 395,79,49,74 });
	geyAnims[(uint)GeyserAnimations::moving_anim].PushBack({ 444,79,49,74 });
	geyAnims[(uint)GeyserAnimations::moving_anim].PushBack({ 3,155,49,74 });
	geyAnims[(uint)GeyserAnimations::moving_anim].PushBack({ 52,155,49,74 });
	geyAnims[(uint)GeyserAnimations::moving_anim].PushBack({ 101,155,49,74 });
	geyAnims[(uint)GeyserAnimations::moving_anim].PushBack({ 150,155,49,74 });
	geyAnims[(uint)GeyserAnimations::moving_anim].PushBack({ 199,155,49,74 });
	geyAnims[(uint)GeyserAnimations::moving_anim].PushBack({ 248,155,49,74 });
	geyAnims[(uint)GeyserAnimations::moving_anim].PushBack({ 297,155,49,74 });
	geyAnims[(uint)GeyserAnimations::moving_anim].PushBack({ 346,155,49,74 });
	geyAnims[(uint)GeyserAnimations::moving_anim].PushBack({ 395,155,49,74 });
	geyAnims[(uint)GeyserAnimations::moving_anim].PushBack({ 444,155,49,74 });
	geyAnims[(uint)GeyserAnimations::moving_anim].speedFactor = 9.0f;

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

	actualAnim->speed = actualAnim->speedFactor * dt;

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
