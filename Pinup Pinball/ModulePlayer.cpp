#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"

//MUST REMOVE THIS MAKE A FUNCTION
#include "ModulePhysics.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	hiScore = 0;
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
	if (score > hiScore)
	{
		hiScore = score;
	}

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


