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

fPoint Projectile::RotateAround(fPoint pointToRotate, fPoint rotationPivot, double angle, double radius) const
{
	angle = DEG_2_RAD(angle);

	fPoint toReturn;

	if (pointToRotate.x > rotationPivot.x && pointToRotate.y < rotationPivot.y)
	{
		toReturn.x = cos(angle) * (pointToRotate.x + radius - rotationPivot.x) - sin(angle) * (pointToRotate.y - radius - rotationPivot.y) + rotationPivot.x;
		toReturn.y = sin(angle) * (pointToRotate.x + radius - rotationPivot.x) + cos(angle) * (pointToRotate.y - radius - rotationPivot.y) + rotationPivot.y;
	}
	else if (pointToRotate.x < rotationPivot.x && pointToRotate.y < rotationPivot.y)
	{
		toReturn.x = cos(angle) * (pointToRotate.x - radius - rotationPivot.x) - sin(angle) * (pointToRotate.y - radius - rotationPivot.y) + rotationPivot.x;
		toReturn.y = sin(angle) * (pointToRotate.x - radius - rotationPivot.x) + cos(angle) * (pointToRotate.y - radius - rotationPivot.y) + rotationPivot.y;
	}
	else if (pointToRotate.x < rotationPivot.x && pointToRotate.y > rotationPivot.y)
	{
		toReturn.x = cos(angle) * (pointToRotate.x - radius - rotationPivot.x) - sin(angle) * (pointToRotate.y + radius - rotationPivot.y) + rotationPivot.x;
		toReturn.y = sin(angle) * (pointToRotate.x - radius - rotationPivot.x) + cos(angle) * (pointToRotate.y + radius - rotationPivot.y) + rotationPivot.y;
	}
	else if (pointToRotate.x > rotationPivot.x && pointToRotate.y > rotationPivot.y)
	{
		toReturn.x = cos(angle) * (pointToRotate.x + radius - rotationPivot.x) - sin(angle) * (pointToRotate.y + radius - rotationPivot.y) + rotationPivot.x;
		toReturn.y = sin(angle) * (pointToRotate.x + radius - rotationPivot.x) + cos(angle) * (pointToRotate.y + radius - rotationPivot.y) + rotationPivot.y;
	}
	else
	{
		toReturn.x = cos(angle) * (pointToRotate.x - rotationPivot.x) - sin(angle) * (pointToRotate.y - rotationPivot.y) + rotationPivot.x;
		toReturn.y = sin(angle) * (pointToRotate.x - rotationPivot.x) + cos(angle) * (pointToRotate.y - rotationPivot.y) + rotationPivot.y;
	}

	return fPoint(toReturn.x, toReturn.y);
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
