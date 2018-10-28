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

public:
	uint32 score;
	uint32 hiScore;
	uint multiplier;

	int debug_font = -1;

	//Chars
	char score_number[8];
	char score_text[7];

	char hiScore_number[8];
	char hiScore_text[8];
};