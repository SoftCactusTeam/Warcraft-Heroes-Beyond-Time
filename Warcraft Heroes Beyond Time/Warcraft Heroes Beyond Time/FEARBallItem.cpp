#include "FEARBallItem.h"
#include "ModuleRender.h"
#include "ModuleColliders.h"
#include "Scene.h"
#include "PlayerEntity.h"
#include "ModulePrinter.h"

bool FEARBallItem::Start()
{
	speed_growing_radius = 300.0f;
	angular_vel = 100.0f;
	radius = 50;
	ball_col = App->colliders->AddPlayerAttackCollider({ 0, 0, 20, 20 }, App->scene->player, 0, PlayerAttack::P_Attack_Type::FEARBALL_ITEM);
	return true;
}

bool FEARBallItem::Act(ModuleItems::ItemEvent event, float dt)
{
	//Here you manage your item depending of the event you receive.
	switch (event)
	{
	case ModuleItems::ItemEvent::UPDATE:
		if(ball_col.expired())
			ball_col = App->colliders->AddPlayerAttackCollider({ 0, 0, 20, 20 }, App->scene->player, 0, PlayerAttack::P_Attack_Type::FEARBALL_ITEM);
		ball_counter += dt;
		angle = angular_vel * ball_counter;
		if (angle > 360.0f)
		{
			ball_counter = 0;
		}

		//Mov A
		if ((int)ball_counter % 2 == 0 && radius > 50)
		{
			go_down();
		}
		else if ((int)ball_counter % 2 > 0 && radius < 150)
		{
			go_up();
		}

		//Mov B
		/*if (radius == 30)
		{
			ball_away = true;
		}
		else if (radius == 100)
		{
			ball_away = false;
		}

		if (ball_away)
		{
			go_up();
		}
		else
		{
			go_down();
		}*/

		(*ball_col.lock())->rectArea.x = Ball_pos.x = cos(angle*PI / 180) * radius + 0;
		(*ball_col.lock())->rectArea.y = Ball_pos.y = sin(angle*PI / 180) * radius + 0;
		break;
	}
	return true;
}

bool FEARBallItem::Draw()
{
	//Use the ModulePrinter to print all the stuff.
	iPoint Draw_pos = { (int)(App->scene->player->pos.x + Ball_pos.x), (int)(App->scene->player->pos.y + Ball_pos.y) };
	App->printer->PrintSprite(Draw_pos, App->items->getItemsTexture(), STICK_ITEM, 0, ModulePrinter::Pivots::UPPER_LEFT, { 0,0 }, ModulePrinter::Pivots::CENTER, { 0,0 });
	return true;
}

bool FEARBallItem::printIconOnScreen(iPoint pos)
{
	//The GUI uses this method, fill it in all the items.
	return App->render->Blit(App->items->getItemsTexture(), pos.x, pos.y, &SDL_Rect(STICK_ITEM), 1, 0);
}

void FEARBallItem::go_up()
{
	radius += speed_growing_radius * App->dt;
}

void FEARBallItem::go_down()
{
	radius -= speed_growing_radius * App->dt;
}