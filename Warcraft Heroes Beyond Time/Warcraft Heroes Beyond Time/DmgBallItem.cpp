#include "DMGBallItem.h"
#include "ModuleRender.h"
#include "ModuleColliders.h"
#include "Scene.h"
#include "PlayerEntity.h"
#include "ModulePrinter.h"

bool DMGBallItem::Start()
{
	angular_vel = 250.0f;
	ball_col = App->colliders->AddPlayerAttackCollider({ 0, 0, 20, 20 }, App->scene->player, 20, PlayerAttack::P_Attack_Type::DMGBALL_ITEM);
	return true;
}

bool DMGBallItem::Act(ModuleItems::ItemEvent event, float dt)
{
	//Here you manage your item depending of the event you receive.
	switch (event)
	{
	case ModuleItems::ItemEvent::UPDATE:
		ball_counter += dt;
		angle = angular_vel * ball_counter;
		if (angle > 360)
		{
			ball_counter = 0;
		}
		ball_col->rectArea.x = Ball_pos.x = cos(angle*PI/180) * radius + 20;
		ball_col->rectArea.y = Ball_pos.y = sin(angle*PI/180) * radius + 20;
		break;
	}
	return true;
}

bool DMGBallItem::Draw()
{
	//Use the ModulePrinter to print all the stuff.
	iPoint Draw_pos = { (int)(App->scene->player->pos.x + Ball_pos.x), (int)(App->scene->player->pos.y + Ball_pos.y) };
	App->printer->PrintSprite(Draw_pos, App->items->getItemsTexture(), CLOACK_ICON, 0, ModulePrinter::Pivots::CENTER, angle*2);
	return true;
}

bool DMGBallItem::printIconOnScreen(iPoint pos)
{
	//The GUI uses this method, fill it in all the items.
	return App->render->Blit(App->items->getItemsTexture(), pos.x, pos.y, &SDL_Rect(CLOACK_ICON), 1, 0);
}

