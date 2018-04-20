#include "Application.h"
#include "Projectile.h"
#include "ModulePrinter.h"
#include "ModuleColliders.h"

Projectile::Projectile(const ProjectileInfo& info, Projectile_type type) : data(info), type(type)
{
}

Projectile::~Projectile()
{
}

bool Projectile::Update(float dt)
{
	return true;
}

bool Projectile::Draw()
{
	bool ret = true;

	ret = App->printer->PrintSprite(data.pos, (SDL_Texture*)App->projectiles->GetProjectileAtlas(), data.rectToBlit, data.layer);

	return ret;
}

void Projectile::OnCollision(Collider* yours, Collider* collideWith)
{
}

void Projectile::OnCollisionContinue(Collider* yours, Collider* collideWith)
{
}

void Projectile::OnCollisionLeave(Collider * yours, Collider * collideWith)
{
}
