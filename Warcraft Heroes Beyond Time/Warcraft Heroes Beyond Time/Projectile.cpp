#include "Application.h"
#include "Projectile.h"
#include "ModulePrinter.h"
#include "ModuleColliders.h"

Projectile::Projectile()
{
}

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

	ret = App->printer->PrintSprite(data.pos, (SDL_Texture*)App->projectiles->GetProjectileAtlas(), actualAnim->GetCurrentRect(), data.layer);

	return ret;
}

int Projectile::DecreaseLifePerTime(float dt)
{
	data.life -= 1 * dt;

	if (data.life <= 0)
		App->projectiles->DestroyProjectile(this);

	return data.life;
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
