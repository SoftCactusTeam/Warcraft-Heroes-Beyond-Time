#include "PortalEntity.h"
#include "StaticEntity.h"
#include "Application.h"
#include "ModulePrinter.h"
#include "Scene.h"
#include "ModuleAudio.h"


PortalEntity::PortalEntity(fPoint coor, STATIC_ENTITY_TYPE type, SDL_Texture * texture) : StaticEntity(coor, texture), type(type)
{
	portal.PushBack({ 1  ,1 ,48,48 });
	portal.PushBack({ 50 ,1 ,48,48 });
	portal.PushBack({ 99 ,1 ,48,48 });
	portal.PushBack({ 148,1 ,48,48 });
	portal.PushBack({ 198,1 ,48,48 });
	portal.PushBack({ 1  ,50,48,48 });
	portal.PushBack({ 50 ,50,48,48 });
	portal.PushBack({ 99 ,50,48,48 });
	portal.PushBack({ 148,50,48,48 });
	portal.PushBack({ 198,50,48,48 });
	portal.speedFactor = 20.0f;

}

bool PortalEntity::Start()
{
	anim = &portal;
	return true;
}

bool PortalEntity::Update(float dt)
{
	if (SDL_RectEquals(&anim->GetCurrentFrame(), &SDL_Rect({ 310, 139, 131, 129 })))
		anim->speedFactor = 0.0f;

	anim->speed = anim->speedFactor * dt;
	return true;
}

bool PortalEntity::Finish()
{
	return true;
}

void PortalEntity::OpenPortal()
{
	App->audio->PlayFx(App->audio->UnlockPortalFX);
	portal.speedFactor = 9.0f;
	locked = false;
}

bool PortalEntity::PlayerNear(fPoint pos)
{
	float norm;
	float result = App->scene->portal->pos.x - pos.x;
	float result2 = result * result;
	float result3 = App->scene->portal->pos.y - pos.y;
	float result4 = result3 * result3;
	float result5 = result2 + result4;
	norm = sqrt(result5);
	if (norm <= 80)
		return true;
	else
		return false;
}

bool PortalEntity::Draw()
{
	bool ret = true;

	ret = App->printer->PrintSprite(iPoint(pos.x, pos.y), texture, anim->GetCurrentFrame(), -1, ModulePrinter::Pivots::CUSTOM_PIVOT, anim->GetCurrentPivot());

	return ret;
}