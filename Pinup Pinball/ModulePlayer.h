#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

#include "Box2D/Box2D/Common/b2Settings.h"
class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

	void AddScore(uint scoreToAdd);
	void IncreaseMultiplier();
	void AddBall();
	void LoseBall();

public:
	uint32 score;
	uint32 hiScore;
	uint multiplier;

	ushort remainingBalls;

	int debug_font = -1;
	int yellow_font = -1;

	//Chars
	char score_number[8];
	char score_text[7];

	char hiScore_number[8];
	char hiScore_text[8];

	char multiplier_number[4];
	char multiplier_text[11];

	char balls_number[4];
	char balls_text[6];
};