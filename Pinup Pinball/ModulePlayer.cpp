#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModuleFont.h"

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

	debug_font = App->fonts->Load("pinball/blue_font.png", "! @,_./0123456789$;<&?abcdefghijklmnopqrstuvwxyz", 1);
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");
	App->fonts->UnLoad(debug_font);
	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	if (score > hiScore)
	{
		hiScore = score;
	}
	//Print Score
	App->fonts->BlitText(50, 60, debug_font, "score:");
	sprintf_s(score_number, 8, "%7d", score);
	App->fonts->BlitText(110, 60, debug_font, score_number);
	//Ptint Hi Score
	App->fonts->BlitText(50, 80, debug_font, "hi-score:");
	sprintf_s(hiScore_number, 8, "%7d", hiScore);
	App->fonts->BlitText(110, 80, debug_font, hiScore_number);

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


