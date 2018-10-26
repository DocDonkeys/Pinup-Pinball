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

	//Positions
	lightRect = { 48, 12, 16, 16 };
	
	//Lights	// CHANGE/FIX: Create a function for this
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

	blueOverKicker.create({ 405, 796 }, { 89, 44, 15, 13 });
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
	int left_flipper[16] = {
		6, 92,
		15, 92,
		47, 112,
		47, 116,
		43, 116,
		5, 107,
		1, 101,
		1, 96,
	};
	
	leftFlipperRect = {0,92,50,26};
	leftFlipper = App->physics->CreateFlipper(SCREEN_WIDTH / 2.8f + 100, SCREEN_HEIGHT - SCREEN_HEIGHT / 11.0f, 
		 9, left_flipper, 16, leftFlipperRect, -45, 0);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	/*if (App->input->GetKey(SDL_SCANCODE_LEFT) == (KEY_DOWN || KEY_REPEAT))
	{
		App->physics->flipper_joint->SetMaxMotorTorque(400.0f);
		App->physics->flipper_joint->SetMotorSpeed(-500.0f);
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == (KEY_UP))
	{
		App->physics->flipper_joint->SetMaxMotorTorque(-20.0f);
		App->physics->flipper_joint->SetMotorSpeed(0.0f);
	}*/

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25));
		circles.getLast()->data->listener = this;
	}

	if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50));
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

	// Draw static map elements -------------------------------------------------	// @Carles	//CHANGE/FIX: Add loops and conditions for drawing
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

	App->renderer->Blit(spriteSheet, (int)greenLeftLight.position.x, (int)greenLeftLight.position.y, &greenLeftLight.rect);
	App->renderer->Blit(spriteSheet, (int)greenRightLight.position.x, (int)greenRightLight.position.y, &greenRightLight.rect);

	App->renderer->Blit(spriteSheet, (int)pegLeft.position.x, (int)pegLeft.position.y, &pegLeft.rect);
	App->renderer->Blit(spriteSheet, (int)pegMiddle.position.x, (int)pegMiddle.position.y, &pegMiddle.rect);
	App->renderer->Blit(spriteSheet, (int)pegRight.position.x, (int)pegRight.position.y, &pegRight.rect);

	App->renderer->Blit(spriteSheet, (int)arrowLeft.position.x, (int)arrowLeft.position.y, &arrowLeft.rect);
	App->renderer->Blit(spriteSheet, (int)arrowMiddle.position.x, (int)arrowMiddle.position.y, &arrowMiddle.rect);
	App->renderer->Blit(spriteSheet, (int)arrowRight.position.x, (int)arrowRight.position.y, &arrowRight.rect);

	App->renderer->Blit(spriteSheet, (int)blueOverKicker.position.x, (int)blueOverKicker.position.y, &blueOverKicker.rect);

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
		App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
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

	// Draw ramps -------------------------------------------------------------	// CHANGE/FIX: Add conditions so ball can draw after this and not before
	App->renderer->Blit(ramps, 0, 0, &fullScreenRect);	// @Carles


	//SUPER F'd UP Blit for left flipper @Dídac   MUST FIX/CHANGE
	int x_, y_;
	App->physics->hardcoded_flipper->GetPosition(x_,y_);
	SDL_Rect l_flipper_rect;
	l_flipper_rect.h = 27;
	l_flipper_rect.w = 50;
	l_flipper_rect.x = 0;
	l_flipper_rect.y = 92;

	App->renderer->Blit(spriteSheet, x_, y_ + l_flipper_rect.y,&l_flipper_rect, 1.0f, App->physics->hardcoded_flipper->GetRotation(), 0, -l_flipper_rect.y);

	iPoint coords;
	leftFlipper.Pbody->GetPosition(coords.x,coords.y);
	App->renderer->Blit(spriteSheet, coords.x, coords.y + leftFlipper.Rect.y, &leftFlipper.Rect, 1.0f, leftFlipper.Pbody->GetRotation(), 0, -leftFlipper.Rect.y);
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	App->audio->PlayFx(bonus_fx);
}
