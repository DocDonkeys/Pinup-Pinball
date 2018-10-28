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
	remainingBalls = 3;

	debug_font = App->fonts->Load("pinball/fonts/blue_font.png", "! @,_./0123456789$;<&?abcdefghijklmnopqrstuvwxyz", 1);
	yellow_font = App->fonts->Load("pinball/fonts/yellow_font.png", "0123456789abcdefghiklmnoprstuvy©        ", 4);
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");
	App->fonts->UnLoad(debug_font);
	App->fonts->UnLoad(yellow_font);

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
	App->fonts->BlitText(70, 75, yellow_font, "score:");
	sprintf_s(score_number, 8, "%7d", score);
	App->fonts->BlitText(130, 75, yellow_font, score_number);
	//Print Hi Score
	App->fonts->BlitText(40, 55, yellow_font, "hi-score:");
	sprintf_s(hiScore_number, 8, "%7d", hiScore);
	App->fonts->BlitText(130, 55, yellow_font, hiScore_number);

	//Print Multiplier
	App->fonts->BlitText(150, 100, yellow_font, "multiplier");
	sprintf_s(multiplier_number, 8, "%7d", multiplier);
	App->fonts->BlitText(210, 100, yellow_font, multiplier_number);

	//Print Balls
	App->fonts->BlitText(150, 115, yellow_font, "balls");
	sprintf_s(balls_number, 8, "%7d", remainingBalls);
	App->fonts->BlitText(160, 115, yellow_font, balls_number);

	if (remainingBalls == 0)
	{
		App->fonts->BlitText(SCREEN_WIDTH / 2 - 45, SCREEN_HEIGHT / 2, yellow_font, "game over");
		App->fonts->BlitText(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 20, yellow_font, "press space to restart");
	}
	return UPDATE_CONTINUE;
}


void ModulePlayer::AddScore(uint scoreToAdd)
{
	score += scoreToAdd * multiplier;
}

void ModulePlayer::IncreaseMultiplier()
{
	if (multiplier < 100)
		multiplier++;
}

void ModulePlayer::AddBall()
{
	remainingBalls++;
}

void ModulePlayer::LoseBall()
{
	if (remainingBalls > 0)
	remainingBalls--;
}


