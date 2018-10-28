#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = map = ramps = spriteSheet = nullptr;

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
	greenLight[0].create({ 47, 425 }, { 49, 0, 5, 5 });
	greenLight[1].create({ 407, 304 }, { 55, 0, 5, 5 });

	pegs[0].create({ 20, 515 }, { 62, 0, 10, 10 });
	pegs[1].create({ 195, 759 }, { 73, 0, 10, 10 });
	pegs[2].create({ 370, 515 }, { 84, 0, 10, 10 });

	arrows[0].create({ 149, 231 }, { 0, 31, 16, 26 });
	arrows[1].create({ 320, 200 }, { 17, 31, 14, 26 });
	arrows[2].create({ 354, 210 }, { 32, 30, 20, 27 });

	tunnels[0].create({ 29, 407 }, { 3, 94, 43, 46 });
	tunnels[1].create({ 397, 279 }, { 78, 94, 32, 85 });

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


	ball_collision_fx = App->audio->LoadFx("pinball/audio/ball_collision.wav");
	ball_lost_fx = App->audio->LoadFx("pinball/audio/ball_lost.wav");
	big_bumper_left_fx = App->audio->LoadFx("pinball/audio/big_bumper_left.wav");
	big_bumper_right_fx = App->audio->LoadFx("pinball/audio/big_bumper_right.wav");
	flipper_top_fx = App->audio->LoadFx("pinball/audio/flipper_top.wav");
	flipper_bottom_fx = App->audio->LoadFx("pinball/audio/flipper_bottom.wav");
	game_over_fx = App->audio->LoadFx("pinball/audio/game_over.wav");
	lat_light_light_up_fx = App->audio->LoadFx("pinball/audio/lat_light_light_up.wav");
	light_lights_up_fx = App->audio->LoadFx("pinball/audio/light_lights_up.wav");
	left_kicker_fx = App->audio->LoadFx("pinball/audio/left_kicker.wav");
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
	// ALSO DISABLE SOME SENSORS

	generalWallsList.add(App->physics->CreateChain(0, 0, outsideWalls, 231, b2_staticBody));
	generalWallsList.add(App->physics->CreateChain(0, 0, topLeftWalls, 32, b2_staticBody));
	generalWallsList.add(App->physics->CreateChain(0, 0, downLeftWalls, 18, b2_staticBody));
	generalWallsList.add(App->physics->CreateChain(0, 0, downRightWalls, 18, b2_staticBody));

	smallTopWallsList.add(App->physics->CreateRectangle(55, 165, 3, 23, b2_staticBody));
	smallTopWallsList.add(App->physics->CreateRectangle(80, 165, 4, 23, b2_staticBody));
	smallTopWallsList.add(App->physics->CreateRectangle(105, 165, 4, 23, b2_staticBody));

	//Sensors
	sensorList.add(App->physics->CreateRectangleSensor(43, 166, 7, 19, b2_staticBody, collision_type::LIGHT_TOP_LEFT_1));
	sensorList.add(App->physics->CreateRectangleSensor(67, 166, 7, 19, b2_staticBody, collision_type::LIGHT_TOP_LEFT_2));
	sensorList.add(App->physics->CreateRectangleSensor(93, 166, 7, 19, b2_staticBody, collision_type::LIGHT_TOP_LEFT_3));
	sensorList.add(App->physics->CreateRectangleSensor(117, 166, 7, 19, b2_staticBody, collision_type::LIGHT_TOP_LEFT_4));

	sensorList.add(App->physics->CreateRectangleSensor(190, 228, 13, 2, b2_staticBody, collision_type::LIGHT_TOP_1));
	sensorList.add(App->physics->CreateRectangleSensor(214, 231, 13, 2, b2_staticBody, collision_type::LIGHT_TOP_2));
	sensorList.add(App->physics->CreateRectangleSensor(239, 231, 13, 2, b2_staticBody, collision_type::LIGHT_TOP_3));
	sensorList.add(App->physics->CreateRectangleSensor(264, 228, 13, 2, b2_staticBody, collision_type::LIGHT_TOP_4));

	sensorList.add(App->physics->CreateRectangleSensor(36, 480, 3, 14, b2_staticBody, collision_type::LIGHT_LEFT));
	sensorList.add(App->physics->CreateRectangleSensor(364, 480, 3, 14, b2_staticBody, collision_type::LIGHT_RIGHT));

	sensorList.add(App->physics->CreateRectangleSensor(45, 606, 7, 19, b2_staticBody, collision_type::LIGHT_DOWN_LEFT));
	sensorList.add(App->physics->CreateRectangleSensor(355, 606, 7, 19, b2_staticBody, collision_type::LIGHT_DOWN_RIGHT));

	sensorList.add(App->physics->CreateRectangleSensor(91, 108, 6, 15, b2_staticBody, collision_type::RAMP_ACTIVATE));
	sensorList.add(App->physics->CreateRectangleSensor(178, 143, 6, 15, b2_staticBody, collision_type::RAMP_ACTIVATE));
	sensorList.add(App->physics->CreateRectangleSensor(262, 180, 6, 16, b2_staticBody, collision_type::RAMP_ACTIVATE));

	sensorList.add(App->physics->CreateRectangleSensor(60, 105, 6, 15, b2_staticBody, collision_type::RAMP_DEACTIVATE));
	sensorList.add(App->physics->CreateRectangleSensor(210, 175, 6, 16, b2_staticBody, collision_type::RAMP_DEACTIVATE));
	sensorList.add(App->physics->CreateRectangleSensor(250, 143, 6, 15, b2_staticBody, collision_type::RAMP_DEACTIVATE));

	sensorList.add(App->physics->CreateRectangleSensor(250, 25, 6, 15, b2_staticBody, collision_type::THIRD_RAMP));

	sensorList.add(App->physics->CreateRectangleSensor(48, 422, 18, 4, b2_staticBody, collision_type::TUNNEL_LEFT));
	sensorList.add(App->physics->CreateRectangleSensor(420, 315, 4, 18, b2_staticBody, collision_type::TUNNEL_RIGHT));

	sensorList.add(App->physics->CreateRectangleSensor(199, 812, 64, 8, b2_staticBody, collision_type::LOSE_BALL));

	bumper_hugger_left = App->physics->CreateChain(0, 0, left_bumper_h, 28, b2_staticBody);
	bumper_hugger_right = App->physics->CreateChain(0, 0, right_bumper_h, 28, b2_staticBody);

	//Adding Bumpers
	bumpersList.add(App->physics->CreateChain(0, 0, left_bumper, 8, b2_staticBody, collision_type::BUMPER_LEFT, 1.75f));
	bumpersList.add(App->physics->CreateChain(0, 0, right_bumper, 8, b2_staticBody, collision_type::BUMPER_RIGHT, 1.75f));

	bumpersList.add(App->physics->CreateRectangle(10, 470, 40, 20, b2_staticBody, collision_type::LEFT_KICKER, 4.0f));

	bumpersList.add(App->physics->CreateCircle(61, 212, 10, b2_staticBody, collision_type::SMALL_BUMPER, 0.75f));	//Red Left
	bumpersList.add(App->physics->CreateCircle(102, 217, 8, b2_staticBody, collision_type::SMALL_BUMPER, 0.75f));	//Blue Right
	bumpersList.add(App->physics->CreateCircle(77, 250, 10, b2_staticBody, collision_type::SMALL_BUMPER, 0.75f));	//Red Middle

	topRightBumper = App->physics->CreateCircle(400, 153, 11, b2_staticBody, collision_type::SMALL_BUMPER, 0.75f);	//Top Right	//CHANGE/FIX: DELETE

	//Adding Pegs
	pegsList.add(App->physics->CreateCircle(25, 520, 5, b2_staticBody, collision_type::PEG_LEFT, 0.75f));
	pegsList.add(App->physics->CreateCircle(200, 764, 5, b2_staticBody, collision_type::PEG_MIDDLE, 0.75f));
	pegsList.add(App->physics->CreateCircle(375, 520, 5, b2_staticBody, collision_type::PEG_RIGHT, 0.75f));

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
		App->audio->PlayFx(flipper_top_fx);
	}
	else if (App->input->GetKey(SDL_SCANCODE_LEFT) == (KEY_UP))
	{
		App->physics->FlipperSetMaxMotorTorque(leftFlipper, 10.0f);
		App->physics->FlipperSetMotorSpeed(leftFlipper, 25.0f);
		App->audio->PlayFx(flipper_bottom_fx);
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
		App->audio->PlayFx(flipper_top_fx);
	}
	else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == (KEY_UP))
	{
		App->physics->FlipperSetMaxMotorTorque(rightFlipper, 10.0f);
		App->physics->FlipperSetMotorSpeed(rightFlipper, -25.0f);
		App->audio->PlayFx(flipper_bottom_fx);
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

	// Ramp logic
	if (mustCreateRamps == true) {
		CreateRamps();
		mustCreateRamps = false;
	}
	else if (mustDeleteRamps == true) {
		DeleteRamps();
		mustDeleteRamps = false;
	}

	// Tunnel teleport
	if (sensorFlags.tunnels[0] == true && tunnelTimer < SDL_GetTicks() - 2000) {
		
		TeleportBall(collision_type::TUNNEL_RIGHT);
		sensorFlags.tunnels[0] = false;
	}
	else if (sensorFlags.tunnels[1] == true && tunnelTimer < SDL_GetTicks() - 2000) {

		TeleportBall(collision_type::TUNNEL_LEFT);
		sensorFlags.tunnels[1] = false;
	}

	// Draw map -----------------------------------------------------------------	// @Carles
	App->renderer->Blit(map, 0, 0, &fullScreenRect);

	// UNDER-ball static elements: Draw and management	// @Carles	//CHANGE/FIX: Add loops and conditions for drawing
	int i = 0;
	p2List_item<PhysBody*>* tmpBody;

	if (topRightBumper != nullptr && topRightBumper->mustDestroy == true) {
		topRightBumper->mustDestroy = false;
		App->physics->world->DestroyBody(topRightBumper->body);
		delete topRightBumper;
		topRightBumper == nullptr;
	}
	if (mustCreateTopRightBumper == true) {
		topRightBumper = App->physics->CreateCircle(400, 153, 11, b2_staticBody, collision_type::SMALL_BUMPER, 0.75f);
		mustCreateTopRightBumper = false;
	}

	if (sensorFlags.thirdRamp == true) {
		App->renderer->Blit(spriteSheet, (int)missingBumper.position.x, (int)missingBumper.position.y, &missingBumper.rect);
	}

	for (i = 0; i < 3; i++) {
		if (sensorFlags.arrows[i] == true) {
			App->renderer->Blit(spriteSheet, (int)arrows[i].position.x, (int)arrows[i].position.y, &arrows[i].rect);
		}
	}

	for (i = 0; i < 4; i++) {
		if (sensorFlags.lightsTopLeft[i] == true) {
			App->renderer->Blit(spriteSheet, (int)lightPosList[i].x, (int)lightPosList[i].y, &lightRect);
		}
		if (sensorFlags.lightsTop[i] == true) {
			App->renderer->Blit(spriteSheet, (int)lightPosList[i + 4].x, (int)lightPosList[i + 4].y, &lightRect);
		}
	}

	for (i = 0; i < 2; i++) {
		if (sensorFlags.lightsMiddle[i] == true) {
			App->renderer->Blit(spriteSheet, (int)lightPosList[i + 8].x, (int)lightPosList[i + 8].y, &lightRect);
		}
		if (sensorFlags.lightsDown[i] == true) {
			App->renderer->Blit(spriteSheet, (int)lightPosList[i + 10].x, (int)lightPosList[i + 10].y, &lightRect);
		}
	}

	for (i = 0; i < 3; i++) {
		if (sensorFlags.lightsDown[i] == true) {
			App->renderer->Blit(spriteSheet, (int)lightPosList[i + 10].x, (int)lightPosList[i + 10].y, &lightRect);
		}
		if (sensorFlags.pegs[i] == true) {
			App->renderer->Blit(spriteSheet, (int)pegs[i].position.x, (int)pegs[i].position.y, &pegs[i].rect);
		}
	}

	for (p2List_item<PhysBody*>* currentPeg = pegsList.getFirst(); currentPeg != nullptr; currentPeg = tmpBody) {
		if (currentPeg->data->mustDestroy == true) {
			currentPeg->data->mustDestroy = false;
			App->physics->world->DestroyBody(currentPeg->data->body);

			tmpBody = currentPeg->next;
			pegsList.del(currentPeg);
		}
		else {
			tmpBody = currentPeg->next;
		}
	}

	if (mustRestorePegs == true) {
		for (p2List_item<PhysBody*>* currentPeg = pegsList.getFirst(); currentPeg != nullptr; currentPeg = tmpBody) {
			App->physics->world->DestroyBody(currentPeg->data->body);
			tmpBody = currentPeg->next;
			pegsList.del(currentPeg);
		}
		pegsList.clear();

		pegsList.add(App->physics->CreateCircle(25, 520, 5, b2_staticBody, collision_type::PEG_LEFT, 0.75f));
		pegsList.add(App->physics->CreateCircle(200, 764, 5, b2_staticBody, collision_type::PEG_MIDDLE, 0.75f));
		pegsList.add(App->physics->CreateCircle(375, 520, 5, b2_staticBody, collision_type::PEG_RIGHT, 0.75f));

		mustRestorePegs = false;

		for (i = 0; i < 3; i++)
			sensorFlags.pegs[i] = false;
	}

	// OVER-ball static elements: Draw and management (RAMPS ACTIVE)
	if (sensorFlags.activatedRamps == true) {
		App->renderer->Blit(spriteSheet, (int)tunnels[0].position.x, (int)tunnels[0].position.y, &tunnels[0].rect);
		App->renderer->Blit(spriteSheet, (int)tunnels[1].position.x, (int)tunnels[1].position.y, &tunnels[1].rect);

		if (sensorFlags.tunnels[0] == true) {
			App->renderer->Blit(spriteSheet, (int)greenLight[1].position.x, (int)greenLight[1].position.y, &greenLight[1].rect);
		}
		if (sensorFlags.tunnels[1] == true) {
			App->renderer->Blit(spriteSheet, (int)greenLight[0].position.x, (int)greenLight[0].position.y, &greenLight[0].rect);
		}

		App->renderer->Blit(ramps, 0, 0, &fullScreenRect);
	}

	// Prepare for raycast ------------------------------------------------------
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		if (c->data->mustDestroy == true) {
			c->data->mustDestroy = false;
			App->physics->world->DestroyBody(c->data->body);
			tmpBody = c;
			c = c->next;
			circles.del(tmpBody);
		}
		else {
			int x, y;
			c->data->GetPosition(x, y);
			App->renderer->Blit(spriteSheet, x, y, &ballRect, 1.0f /*c->data->GetRotation()*/);
			c = c->next;
		}
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

	//Draw the kicker
	iPoint coords;
	pinballKicker.pbody->GetPosition(coords.x, coords.y);
	App->renderer->Blit(spriteSheet, coords.x, coords.y, &pinballKicker.rect);

	App->renderer->Blit(spriteSheet, (int)overLeftKicker.position.x, (int)overLeftKicker.position.y, &overLeftKicker.rect);
	App->renderer->Blit(spriteSheet, (int)overRightKicker.position.x, (int)overRightKicker.position.y, &overRightKicker.rect);

	// OVER-ball static elements: Draw and management (RAMPS NOT ACTIVE)	//CHANGE/FIX: Add loops and conditions for drawing, also copypasted above
	if (sensorFlags.activatedRamps == false) {
		App->renderer->Blit(spriteSheet, (int)tunnels[0].position.x, (int)tunnels[0].position.y, &tunnels[0].rect);
		App->renderer->Blit(spriteSheet, (int)tunnels[1].position.x, (int)tunnels[1].position.y, &tunnels[1].rect);

		if (sensorFlags.tunnels[0] == true) {
			App->renderer->Blit(spriteSheet, (int)greenLight[1].position.x, (int)greenLight[1].position.y, &greenLight[1].rect);
		}
		if (sensorFlags.tunnels[1] == true) {
			App->renderer->Blit(spriteSheet, (int)greenLight[0].position.x, (int)greenLight[0].position.y, &greenLight[0].rect);
		}

		App->renderer->Blit(ramps, 0, 0, &fullScreenRect);
	}
	
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
	if (bodyB != nullptr) {
		if (bodyB->collision <= collision_type::LIGHT_DOWN_RIGHT) {
			switch (bodyB->collision) {	// CHANGE/FIX: TURN INTO FUNCTION
			case collision_type::LIGHT_TOP_LEFT_1:
				if (sensorFlags.activatedRamps == false) {
					sensorFlags.lightsTopLeft[0] = true;
					App->player->AddScore(scoreRewards.topLeftLight);
					CheckThirdRamp();
				}
				break;
			case collision_type::LIGHT_TOP_LEFT_2:
				if (sensorFlags.activatedRamps == false) {
					sensorFlags.lightsTopLeft[1] = true;
					App->player->AddScore(scoreRewards.topLeftLight);
					CheckThirdRamp();
				}
				break;
			case collision_type::LIGHT_TOP_LEFT_3:
				if (sensorFlags.activatedRamps == false) {
					sensorFlags.lightsTopLeft[2] = true;
					App->player->AddScore(scoreRewards.topLeftLight);
					CheckThirdRamp();
				}
				break;
			case collision_type::LIGHT_TOP_LEFT_4:
				if (sensorFlags.activatedRamps == false) {
					sensorFlags.lightsTopLeft[3] = true;
					App->player->AddScore(scoreRewards.topLeftLight);
					CheckThirdRamp();
				}
				break;
			case collision_type::LIGHT_TOP_1:
				sensorFlags.lightsTop[0] = true;
				App->audio->PlayFx(light_lights_up_fx);
				App->player->AddScore(scoreRewards.buttonLight);
				CheckMultiplier();
				break;
			case collision_type::LIGHT_TOP_2:
				sensorFlags.lightsTop[1] = true;
				App->audio->PlayFx(light_lights_up_fx);
				App->player->AddScore(scoreRewards.buttonLight);
				CheckMultiplier();
				break;
			case collision_type::LIGHT_TOP_3:
				sensorFlags.lightsTop[2] = true;
				App->audio->PlayFx(light_lights_up_fx);
				App->player->AddScore(scoreRewards.buttonLight);
				CheckMultiplier();
				break;
			case collision_type::LIGHT_TOP_4:
				sensorFlags.lightsTop[3] = true;
				App->audio->PlayFx(light_lights_up_fx);
				App->player->AddScore(scoreRewards.buttonLight);
				CheckMultiplier();
				break;
			case collision_type::LIGHT_LEFT:
				sensorFlags.lightsMiddle[0] = true;
				App->audio->PlayFx(lat_light_light_up_fx);
				App->player->AddScore(scoreRewards.buttonLight);
				CheckRampEventStart();
				break;
			case collision_type::LIGHT_RIGHT:
				sensorFlags.lightsMiddle[1] = true;
				App->audio->PlayFx(lat_light_light_up_fx);
				App->player->AddScore(scoreRewards.buttonLight);
				CheckRampEventStart();
				break;
			case collision_type::LIGHT_DOWN_LEFT:
				App->player->AddScore(scoreRewards.pegScore);
				if (sensorFlags.rampDone[0] == true) {
					if (sensorFlags.lightsDown[0] == false) {
						sensorFlags.lightsDown[0] = true;
					}
					else {
						RestorePegs(collision_type::LIGHT_DOWN_LEFT);
						sensorFlags.lightsDown[0] = false;
					}

					sensorFlags.rampDone[0] = false;
				}
				break;
			case collision_type::LIGHT_DOWN_RIGHT:
				App->player->AddScore(scoreRewards.pegScore);
				if (sensorFlags.rampDone[1] == true) {
					if (sensorFlags.lightsDown[1] == false) {
						sensorFlags.lightsDown[1] = true;
					}
					else {
						RestorePegs(collision_type::LIGHT_DOWN_RIGHT);
						sensorFlags.lightsDown[1] = false;
					}

					sensorFlags.rampDone[1] = false;
				}
				break;
			default:
				break;
			}
		}
		else {
			switch (bodyB->collision) {	// CHANGE/FIX: TURN INTO FUNCTION
			case collision_type::RAMP_ACTIVATE:
				if (sensorFlags.activatedRamps == false) {
					mustCreateRamps = true;
					sensorFlags.activatedRamps = true;
					App->player->AddScore(scoreRewards.enterRamp);
				};
				break;
			case collision_type::RAMP_DEACTIVATE:
				if (sensorFlags.activatedRamps == true) {
					mustDeleteRamps = true;
					sensorFlags.activatedRamps = false;
				}
				break;
			case collision_type::RAMP_LEFT_FINISH:
				sensorFlags.rampDone[0] = true;

				if (sensorFlags.arrows[1] == true) {
					sensorFlags.arrows[1] = false;
					sensorFlags.rampEventDone[0] = true;
					CheckRampEventEnd();
				}
				if (sensorFlags.activatedRamps == true) {
					mustDeleteRamps = true;
					sensorFlags.activatedRamps = false;
				}
				break;
			case collision_type::RAMP_RIGHT_FINISH:
				sensorFlags.rampDone[1] = true;

				if (sensorFlags.arrows[0] == true) {
					sensorFlags.arrows[0] = false;
					sensorFlags.rampEventDone[1] = true;
					CheckRampEventEnd();
				}
				if (sensorFlags.activatedRamps == true) {
					mustDeleteRamps = true;
					sensorFlags.activatedRamps = false;
				}
				break;
			case collision_type::THIRD_RAMP:
				sensorFlags.thirdRamp = false;
				sensorFlags.arrows[2] = false;
				mustCreateTopRightBumper = true;
				App->player->AddScore(scoreRewards.thirdRamp);
				break;
			case collision_type::LEFT_KICKER:
				App->player->AddScore(scoreRewards.leftKicker);
				App->audio->PlayFx(left_kicker_fx);
				break;
			case collision_type::TUNNEL_LEFT:
				bodyA->mustDestroy = true;
				sensorFlags.tunnels[0] = true;
				tunnelTimer = SDL_GetTicks();
				App->player->AddScore(scoreRewards.tunnel);
				break;
			case collision_type::TUNNEL_RIGHT:
				bodyA->mustDestroy = true;
				sensorFlags.tunnels[1] = true;
				tunnelTimer = SDL_GetTicks();
				App->player->AddScore(scoreRewards.tunnel);
				break;
			case collision_type::BUMPER_LEFT:
				App->audio->PlayFx(big_bumper_left_fx);
				App->player->AddScore(scoreRewards.bumper);
				break;
			case collision_type::BUMPER_RIGHT:
				App->audio->PlayFx(big_bumper_right_fx);
				App->player->AddScore(scoreRewards.bumper);
				break;
			case collision_type::SMALL_BUMPER:
				App->player->AddScore(scoreRewards.smallBumper);
				break;
			case collision_type::PEG_LEFT:
				sensorFlags.pegs[0] = true;
				bodyB->mustDestroy = true;
				break;
			case collision_type::PEG_MIDDLE:
				sensorFlags.pegs[1] = true;
				bodyB->mustDestroy = true;
				break;
			case collision_type::PEG_RIGHT:
				sensorFlags.pegs[2] = true;
				bodyB->mustDestroy = true;
				break;
			case collision_type::LOSE_BALL:
				bodyA->mustDestroy = true;
				RestorePegs(collision_type::LOSE_BALL);
				App->audio->PlayFx(ball_lost_fx);
				break;
			default:
				break;
			}
		}
			App->audio->PlayFx(ball_collision_fx);
	}
}

void ModuleSceneIntro::CheckMultiplier()	//@Carles
{
	if (sensorFlags.lightsTop[0] == true && sensorFlags.lightsTop[1] == true && sensorFlags.lightsTop[2] == true && sensorFlags.lightsTop[3] == true) {
		App->player->IncreaseMultiplier(); //This does multiplier++ but controlled by the player who owns the score

		for (int i = 0; i < 4; i++) {
			sensorFlags.lightsTop[i] = false;
		}
	}
}

void ModuleSceneIntro::CheckThirdRamp()	//@Carles
{
	if (sensorFlags.lightsTopLeft[0] == true && sensorFlags.lightsTopLeft[1] == true && sensorFlags.lightsTopLeft[2] == true && sensorFlags.lightsTopLeft[3] == true) {
		if (sensorFlags.thirdRamp == true) {
			App->player->IncreaseMultiplier();
		}
		else {
			sensorFlags.thirdRamp = true;
			sensorFlags.arrows[2] = true;
			
			topRightBumper->mustDestroy = true;
		}

		for (int i = 0; i < 4; i++) {
			sensorFlags.lightsTopLeft[i] = false;
		}
	}
}

void ModuleSceneIntro::CheckRampEventStart()	//@Carles
{
	if (sensorFlags.lightsMiddle[0] == true && sensorFlags.lightsMiddle[1] == true && sensorFlags.arrows[1] == false && sensorFlags.arrows[2] == false) {
		for (int i = 0; i < 2; i++) {
			sensorFlags.arrows[i] = true;
		}
	}
}

void ModuleSceneIntro::CheckRampEventEnd()	//@Carles
{
	if (sensorFlags.rampEventDone[0] == true && sensorFlags.rampEventDone[1] == true) {
		//do event	//CHANGE/FIX

		for (int i = 0; i < 2; i++) {
			sensorFlags.lightsMiddle[i] = false;
			sensorFlags.rampEventDone[i] = false;
		}

		sensorFlags.arrows[0] = false;
		sensorFlags.arrows[1] = false;
	}
}

void ModuleSceneIntro::RestorePegs(collision_type collType)
{
	uint i = 0;
	for (p2List_item<PhysBody*>* tmp = pegsList.getFirst(); tmp != nullptr; tmp = tmp->next) {
		tmp->data->mustDestroy = true;
		i++;
	}

	if (collType != collision_type::LOSE_BALL) {
		for (i; i < 3; i++) {
			App->player->AddScore(scoreRewards.pegScore);
		}
	}

	mustRestorePegs = true;
}

void ModuleSceneIntro::TeleportBall(collision_type collType)
{
	// DOOM vector: b2Vec2 leftTunnelSpeed(3, 3);

	if (collType == collision_type::TUNNEL_LEFT) {
		circles.add(App->physics->CreateCircle(55, 439, 9));
		b2Vec2 leftTunnelSpeed(4.0f, 4.0f);
		circles.getLast()->data->body->SetLinearVelocity(leftTunnelSpeed);
	}
	if (collType == collision_type::TUNNEL_RIGHT) {
		circles.add(App->physics->CreateCircle(408, 303, 9));
		b2Vec2 rightTunnelSpeed(-5.0f, -4.0f);
		circles.getLast()->data->body->SetLinearVelocity(rightTunnelSpeed);
	}
	
	circles.getLast()->data->listener = this;
}

void ModuleSceneIntro::CreateRamps()	//@Carles
{
	for (p2List_item<PhysBody*>* tmp = generalWallsList.getFirst(); tmp != nullptr; tmp = tmp->next) {
		App->physics->world->DestroyBody(tmp->data->body);
	}
	generalWallsList.clear();
	
	for (p2List_item<PhysBody*>* tmp = smallTopWallsList.getFirst(); tmp != nullptr; tmp = tmp->next) {
		App->physics->world->DestroyBody(tmp->data->body);
	}
	smallTopWallsList.clear();

	uint i = 0;
	for (p2List_item<PhysBody*>* tmp = bumpersList.getLast(); i < 2; i++) {
		App->physics->world->DestroyBody(tmp->data->body);
		tmp = tmp->prev;
		bumpersList.del(tmp->next);
	}

	sensorList.add(App->physics->CreateRectangleSensor(50, 556, 12, 10, b2_staticBody, collision_type::RAMP_LEFT_FINISH));
	sensorList.add(App->physics->CreateRectangleSensor(355, 580, 12, 10, b2_staticBody, collision_type::RAMP_RIGHT_FINISH));

	rampWallsList.add(App->physics->CreateChain(0, 0, leftRampWalls, 52, b2_staticBody));
	rampWallsList.add(App->physics->CreateChain(0, 0, rightRampWalls, 26, b2_staticBody));
}

void ModuleSceneIntro::DeleteRamps()	//@Carles
{
	for (p2List_item<PhysBody*>* tmp = rampWallsList.getFirst(); tmp != nullptr; tmp = tmp->next) {
		App->physics->world->DestroyBody(tmp->data->body);
	}
	rampWallsList.clear();

	uint i = 0;
	for (p2List_item<PhysBody*>* tmp = sensorList.getLast(); i < 2; i++) {
		App->physics->world->DestroyBody(tmp->data->body);
		tmp = tmp->prev;
		sensorList.del(tmp->next);
	}

	generalWallsList.add(App->physics->CreateChain(0, 0, outsideWalls, 231, b2_staticBody));
	generalWallsList.add(App->physics->CreateChain(0, 0, topLeftWalls, 32, b2_staticBody));
	generalWallsList.add(App->physics->CreateChain(0, 0, downLeftWalls, 18, b2_staticBody));
	generalWallsList.add(App->physics->CreateChain(0, 0, downRightWalls, 18, b2_staticBody));

	smallTopWallsList.add(App->physics->CreateRectangle(55, 165, 3, 23, b2_staticBody));
	smallTopWallsList.add(App->physics->CreateRectangle(80, 165, 4, 23, b2_staticBody));
	smallTopWallsList.add(App->physics->CreateRectangle(105, 165, 4, 23, b2_staticBody));

	bumpersList.add(App->physics->CreateCircle(102, 217, 8, b2_staticBody, collision_type::SMALL_BUMPER, 0.75f));	//Blue Right
	bumpersList.add(App->physics->CreateCircle(77, 250, 10, b2_staticBody, collision_type::SMALL_BUMPER, 0.75f));	//Red Middle
}