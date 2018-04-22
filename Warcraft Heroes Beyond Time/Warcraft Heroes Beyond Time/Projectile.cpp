#include "Application.h"
#include "Projectile.h"
#include "ModulePrinter.h"
#include "ModuleColliders.h"

#include "p2Defs.h"

#include <math.h>

Projectile::Projectile()
{
}

Projectile::Projectile(const ProjectileInfo& info, Projectile_type type) : data(info), projType(type)
{
}

Projectile::~Projectile()
{
}

bool Projectile::Update(float dt)
{
	return true;
}

bool Projectile::Draw() const
{
	bool ret = true;

	ret = App->printer->PrintSprite({ (int)data.pos.x,(int)data.pos.y }, (SDL_Texture*)App->projectiles->GetProjectileAtlas(), actualAnim->GetCurrentFrame(), data.layer);

	return ret;
}

int Projectile::DecreaseLifePerTime(float dt)
{
	data.life -= 1 * dt;

	if (data.life <= 0)
		App->projectiles->DestroyProjectile(this);

	return data.life;
}

fPoint Projectile::RotateAround(fPoint pointToRotate, fPoint rotationPivot, double angle, float radius) const
{
	int norm = pointToRotate.DistanceTo(rotationPivot);

	float b = sin(DEG_2_RAD(angle)) * norm;

	fPoint toReturn = fPoint(rotationPivot.x + norm, rotationPivot.y + b);

	return toReturn;
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
