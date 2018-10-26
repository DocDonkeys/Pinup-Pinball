#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"

//MUST REMOVE THIS MAKE A FUNCTION
#include "ModulePhysics.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
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



