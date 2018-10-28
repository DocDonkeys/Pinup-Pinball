#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"

//MUST REMOVE THIS MAKE A FUNCTION
#include "ModulePhysics.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	hiscore = 0;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	score = 0;
	multiplier = 1;
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	/*if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		App->physics->flipper_joint->SetMaxMotorTorque(400.0f);
		App->physics->flipper_joint->SetMotorSpeed(-500.0f);
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == (KEY_UP))
	{
		App->physics->flipper_joint->SetMaxMotorTorque(-20.0f);
		App->physics->flipper_joint->SetMotorSpeed(0.0f);
	}*/

	return UPDATE_CONTINUE;
}


void ModulePlayer::AddScore(uint scoreToAdd)
{
	score += scoreToAdd * multiplier;
}

void ModulePlayer::IncreaseMultiplier()
{
	multiplier++;
}


