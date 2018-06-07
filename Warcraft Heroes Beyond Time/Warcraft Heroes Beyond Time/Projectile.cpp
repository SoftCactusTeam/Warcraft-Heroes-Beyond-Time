#include "Application.h"
#include "Projectile.h"
#include "ModulePrinter.h"
#include "ModuleColliders.h"

#include "p2Defs.h"

#include <math.h>

Projectile::Projectile()
{
}

Projectile::Projectile(const ProjectileInfo* info, Projectile_type type) : data((ProjectileInfo*)info), projType(type)
{
}

Projectile::~Projectile()
{
	App->colliders->deleteColliderbyOwner(this);
}

bool Projectile::Update(float dt)
{
	return true;
}

bool Projectile::Draw() const
{
	bool ret = true;

	if(data)
		ret = App->printer->PrintSprite({ (int)data->pos.x,(int)data->pos.y }, (SDL_Texture*)App->projectiles->GetProjectileAtlas(), actualAnim->GetCurrentFrame(), data->layer);

	return ret;
}

int Projectile::DecreaseLifePerTime(float dt)
{
	data->life -= 1 * dt;

	if (data->life <= 0)
		App->projectiles->DestroyProjectile(this);

	return data->life;
}

fPoint Projectile::RotateAround(fPoint pointToRotate, fPoint rotationPivot, double angle, double radius) const
{
	angle = DEG_2_RAD(angle);

	fPoint toReturn;
	
	fPoint difference(pointToRotate.x - rotationPivot.x, pointToRotate.y - rotationPivot.y);

	difference.x *= radius;
	difference.y *= radius;
	
	int norm = pointToRotate.DistanceTo(rotationPivot);

	difference.x /= norm;
	difference.y /= norm;

	toReturn.x = cos(angle) * difference.x - sin(angle) * difference.y;
	toReturn.y = sin(angle) * difference.x + cos(angle) * difference.y;

	return fPoint(toReturn.x + rotationPivot.x, toReturn.y + rotationPivot.y);
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
