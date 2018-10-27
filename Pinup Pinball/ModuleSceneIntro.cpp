#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = map = ramps = spriteSheet = NULL;

	// @Carles
	fullScreenRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

	//Sprite sheet positions	// CHANGE/FIX: Create a function for this
	lightRect = { 48, 12, 16, 16 };
	ballRect = { 0, 0, 18, 18 };
	kickerRect = { 82, 24, 6, 34 };

	//Lights (left to right)	// CHANGE/FIX: Create a function for this
	ushort i = 0;
	lightPosList[i++] = { 35, 132 };	// Top left
	lightPosList[i++] = { 60, 132 };
	lightPosList[i++] = { 85, 132 };
	lightPosList[i++] = { 109, 132 };

	lightPosList[i++] = { 183, 236 };	// Top
	lightPosList[i++] = { 208, 236 };
	lightPosList[i++] = { 233, 236 };
	lightPosList[i++] = { 258, 236 };

	lightPosList[i++] = { 42, 472 };	// Sides
	lightPosList[i++] = { 342, 472 };

	lightPosList[i++] = { 37, 572 };	// Down
	lightPosList[i++] = { 347, 572 };

	//Static elements	// CHANGE/FIX: Create a function for this
	missingBumper.create({ 389, 142 }, { 20, 0, 27, 29 });
	greenLeftLight.create({ 47, 427 }, { 49, 0, 5, 5 });
	greenRightLight.create({ 407, 304 }, { 55, 0, 5, 5 });

	pegLeft.create({ 20, 515 }, { 62, 0, 10, 10 });
	pegMiddle.create({ 195, 759 }, { 73, 0, 10, 10 });
	pegRight.create({ 370, 515 }, { 84, 0, 10, 10 });

	arrowLeft.create({ 149, 231 }, { 0, 31, 16, 26 });
	arrowMiddle.create({ 320, 200 }, { 17, 31, 14, 26 });
	arrowRight.create({ 354, 210 }, { 32, 30, 20, 27 });

	tunnelLeft.create({ 29, 407 }, { 3, 94, 43, 46 });
	tunnelRight.create({ 397, 279 }, { 78, 94, 32, 85 });

	overLeftKicker.create({ 0, 428 }, { 25, 146, 36, 33 });
	overRightKicker.create({ 405, 796 }, { 89, 44, 15, 13 });
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("pinball/wheel.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");

	map = App->textures->Load("pinball/sprites/map.png");	// @Carles
	ramps = App->textures->Load("pinball/sprites/ramps.png");
	spriteSheet = App->textures->Load("pinball/sprites/sprite_sheet.png");

	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	//Flipper Chain Change/Fix: we should have all chains in a module or chains.h @Dídac
	// Pivot 0, 0
	int left_flipper[16] = {
		6, 62,
		15, 62,
		47, 82,
		47, 86,
		43, 86,
		5, 77,
		1, 71,
		1, 66,
	};

	int right_flipper[16] = {
		105, 61,
		72, 82,
		71, 85,
		75, 87,
		115, 78,
		119, 72,
		119, 66,
		113, 61
	};
	
	leftFlipperRect = {0,62,50,26};
	leftFlipper = App->physics->CreateFlipper(SCREEN_WIDTH / 2.8f -5, SCREEN_HEIGHT - SCREEN_HEIGHT / 11.0f -2, 
											  9, left_flipper, 16, leftFlipperRect, -45, 0, LEFT_FLIPPER);

	rightFlipperRect = { 70,62,50,26 };
	rightFlipper = App->physics->CreateFlipper(SCREEN_WIDTH / 2.8f + 100, SCREEN_HEIGHT - SCREEN_HEIGHT / 11.0f -2,
											   9, right_flipper, 16, rightFlipperRect, 0, 45 , RIGHT_FLIPPER);

	//Kicker Creation
	pinballKicker = App->physics->CreateKicker(SCREEN_WIDTH - SCREEN_WIDTH / 25 + 1, SCREEN_HEIGHT - 21, 9, kickerRect,0.0f,1.0f,-0.43f,1.0f);
	
	// DELETE MAP WALLS
	/*App->physics->world->DestroyBody(outsideWallsList.getLast()->data->body);
	App->physics->world->DestroyBody(topLeftWallsList.getLast()->data->body);
	App->physics->world->DestroyBody(downLeftWallsList.getLast()->data->body);
	App->physics->world->DestroyBody(downRightWallsList.getLast()->data->body);
	outsideWallsList.clear();
	topLeftWallsList.clear();
	downLeftWallsList.clear();
	downRightWallsList.clear();*/

	sensorList.add(App->physics->CreateRectangleSensor(300, 300, 50, 50, b2_staticBody, collision_type::LIGHT_TOP_1));

	outsideWallsList.add(App->physics->CreateChain(0, 0, outsideWalls, 231, b2_staticBody));
	topLeftWallsList.add(App->physics->CreateChain(0, 0, topLeftWalls, 32, b2_staticBody));
	downLeftWallsList.add(App->physics->CreateChain(0, 0, downLeftWalls, 18, b2_staticBody));
	downRightWallsList.add(App->physics->CreateChain(0, 0, downRightWalls, 18, b2_staticBody));

	smallTopWallsList.add(App->physics->CreateRectangle(55, 165, 3, 23, b2_staticBody));
	smallTopWallsList.add(App->physics->CreateRectangle(80, 165, 4, 23, b2_staticBody));
	smallTopWallsList.add(App->physics->CreateRectangle(105, 165, 4, 23, b2_staticBody));

	//rampWalls.add(App->physics->CreateChain(0, 0, mapRampWalls, ?, b2_staticBody));

	//Bumpers
	bumperLeftProjection = App->physics->CreateRectangle(10,470,40,20, b2_staticBody,collision_type::LEFT_KICKER, 4.0f);
	bumperLeftProjection->listener = this;
	
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	//CHANGE/FIX: CLEAR ALL LISTS

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	//Fix @Dídac
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		App->physics->FlipperSetMaxMotorTorque(leftFlipper, 25.0f);
		App->physics->FlipperSetMotorSpeed(leftFlipper, -25.0f);
	}
	else if (App->input->GetKey(SDL_SCANCODE_LEFT) == (KEY_UP))
	{
		App->physics->FlipperSetMaxMotorTorque(leftFlipper, 10.0f);
		App->physics->FlipperSetMotorSpeed(leftFlipper, 25.0f);
	}
	else if (App->input->GetKey(SDL_SCANCODE_LEFT) == (KEY_IDLE) && leftFlipper.Joint->GetJointAngle() * RADTODEG >= -45)
	{
		App->physics->FlipperSetMaxMotorTorque(leftFlipper, 0.0f);
		App->physics->FlipperSetMotorSpeed(leftFlipper, 0.0f);
	}
	//Fix @Dídac
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		App->physics->FlipperSetMaxMotorTorque(rightFlipper, 25.0f);
		App->physics->FlipperSetMotorSpeed(rightFlipper, 25.0f);
	}
	else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == (KEY_UP))
	{
		App->physics->FlipperSetMaxMotorTorque(rightFlipper, 10.0f);
		App->physics->FlipperSetMotorSpeed(rightFlipper, -25.0f);
	}
	else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == (KEY_IDLE) && rightFlipper.Joint->GetJointAngle() * RADTODEG <= 0)
	{
		App->physics->FlipperSetMaxMotorTorque(rightFlipper, 0.0f);
		App->physics->FlipperSetMotorSpeed(rightFlipper, 0.0f);
	}

	//Kicker control @Dídac
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
	{
		App->physics->KickerSetMaxMotorForce(pinballKicker, -1.0f);
	}
	else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_IDLE)
	{
		App->physics->KickerSetMaxMotorForce(pinballKicker, (pinballKicker.pbody->body->GetPosition().y) + 5.0f);
		App->physics->KickerSetMotorSpeed(pinballKicker, -15.0f);
	}

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 9));
		circles.getLast()->data->listener = this;
	}

	if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50));
		boxes.getLast()->data->listener = this;
	}

	if(App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		// Pivot 0, 0
		int rick_head[64] = {
			14, 36,
			42, 40,
			40, 0,
			75, 30,
			88, 4,
			94, 39,
			111, 36,
			104, 58,
			107, 62,
			117, 67,
			109, 73,
			110, 85,
			106, 91,
			109, 99,
			103, 104,
			100, 115,
			106, 121,
			103, 125,
			98, 126,
			95, 137,
			83, 147,
			67, 147,
			53, 140,
			46, 132,
			34, 136,
			38, 126,
			23, 123,
			30, 114,
			10, 102,
			29, 90,
			0, 75,
			30, 62
		};

		ricks.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), rick_head, 64));
	}

	// Draw map -----------------------------------------------------------------	// @Carles
	App->renderer->Blit(map, 0, 0, &fullScreenRect);

	// Draw UNDER-ball static elements	// @Carles	//CHANGE/FIX: Add loops and conditions for drawing
	ushort i = 0;

	App->renderer->Blit(spriteSheet, (int)lightPosList[i].x, (int)lightPosList[i].y, &lightRect);	i++;
	App->renderer->Blit(spriteSheet, (int)lightPosList[i].x, (int)lightPosList[i].y, &lightRect);	i++;
	App->renderer->Blit(spriteSheet, (int)lightPosList[i].x, (int)lightPosList[i].y, &lightRect);	i++;
	App->renderer->Blit(spriteSheet, (int)lightPosList[i].x, (int)lightPosList[i].y, &lightRect);	i++;
	App->renderer->Blit(spriteSheet, (int)lightPosList[i].x, (int)lightPosList[i].y, &lightRect);	i++;
	App->renderer->Blit(spriteSheet, (int)lightPosList[i].x, (int)lightPosList[i].y, &lightRect);	i++;
	App->renderer->Blit(spriteSheet, (int)lightPosList[i].x, (int)lightPosList[i].y, &lightRect);	i++;
	App->renderer->Blit(spriteSheet, (int)lightPosList[i].x, (int)lightPosList[i].y, &lightRect);	i++;
	App->renderer->Blit(spriteSheet, (int)lightPosList[i].x, (int)lightPosList[i].y, &lightRect);	i++;
	App->renderer->Blit(spriteSheet, (int)lightPosList[i].x, (int)lightPosList[i].y, &lightRect);	i++;
	App->renderer->Blit(spriteSheet, (int)lightPosList[i].x, (int)lightPosList[i].y, &lightRect);	i++;
	App->renderer->Blit(spriteSheet, (int)lightPosList[i].x, (int)lightPosList[i].y, &lightRect);	i++;

	App->renderer->Blit(spriteSheet, (int)missingBumper.position.x, (int)missingBumper.position.y, &missingBumper.rect);

	App->renderer->Blit(spriteSheet, (int)pegLeft.position.x, (int)pegLeft.position.y, &pegLeft.rect);
	App->renderer->Blit(spriteSheet, (int)pegMiddle.position.x, (int)pegMiddle.position.y, &pegMiddle.rect);
	App->renderer->Blit(spriteSheet, (int)pegRight.position.x, (int)pegRight.position.y, &pegRight.rect);

	App->renderer->Blit(spriteSheet, (int)arrowLeft.position.x, (int)arrowLeft.position.y, &arrowLeft.rect);
	App->renderer->Blit(spriteSheet, (int)arrowMiddle.position.x, (int)arrowMiddle.position.y, &arrowMiddle.rect);
	App->renderer->Blit(spriteSheet, (int)arrowRight.position.x, (int)arrowRight.position.y, &arrowRight.rect);

	// Prepare for raycast ------------------------------------------------------

	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(spriteSheet, x, y, &ballRect, 1.0f /*c->data->GetRotation()*/);
		c = c->next;
	}

	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = ricks.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	// Draw OVER-ball static elements	// @Carles	//CHANGE/FIX: Add loops and conditions for drawing
	App->renderer->Blit(spriteSheet, (int)greenLeftLight.position.x, (int)greenLeftLight.position.y, &greenLeftLight.rect);
	App->renderer->Blit(spriteSheet, (int)greenRightLight.position.x, (int)greenRightLight.position.y, &greenRightLight.rect);

	App->renderer->Blit(spriteSheet, (int)tunnelLeft.position.x, (int)tunnelLeft.position.y, &tunnelLeft.rect);
	App->renderer->Blit(spriteSheet, (int)tunnelRight.position.x, (int)tunnelRight.position.y, &tunnelRight.rect);
	
	//Draw the kicker
	iPoint coords;
	pinballKicker.pbody->GetPosition(coords.x, coords.y);
	App->renderer->Blit(spriteSheet, coords.x, coords.y, &pinballKicker.rect);

	App->renderer->Blit(spriteSheet, (int)overLeftKicker.position.x, (int)overLeftKicker.position.y, &overLeftKicker.rect);
	App->renderer->Blit(spriteSheet, (int)overRightKicker.position.x, (int)overRightKicker.position.y, &overRightKicker.rect);


	// Draw ramps -------------------------------------------------------------	// CHANGE/FIX: Add conditions so ball can draw after this and not before
	App->renderer->Blit(ramps, 0, 0, &fullScreenRect);	// @Carles

	
	//Draw the Flippers
	leftFlipper.Pbody->GetPosition(coords.x,coords.y);
	App->renderer->Blit(spriteSheet, coords.x + leftFlipper.Rect.x, coords.y + leftFlipper.Rect.y, &leftFlipper.Rect, 1.0f,
						leftFlipper.Pbody->GetRotation(), -leftFlipper.Rect.x, -leftFlipper.Rect.y);

	rightFlipper.Pbody->GetPosition(coords.x, coords.y);
	App->renderer->Blit(spriteSheet, coords.x + rightFlipper.Rect.x, coords.y + rightFlipper.Rect.y, &rightFlipper.Rect, 1.0f,
						rightFlipper.Pbody->GetRotation(), -rightFlipper.Rect.x, -rightFlipper.Rect.y);
	
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
<<<<<<< HEAD
	if (bodyB != nullptr && bodyB->sensor == true) {
		p2List_item<PhysBody*>* sensor;

		for (sensor = App->scene_intro->sensorList.getFirst(); sensor != nullptr; sensor = sensor->next) {
			circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 9));
		}
	}
	else if (bodyB->collision == collision_type::LEFT_KICKER) {
		
	}

	App->audio->PlayFx(bonus_fx);
}
