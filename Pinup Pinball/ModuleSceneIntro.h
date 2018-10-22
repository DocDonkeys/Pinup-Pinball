#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

struct static_element {	// @Carles
	iPoint position;
	SDL_Rect rect;

	void create(iPoint position, SDL_Rect rect) {
		this->position = position;
		this->rect = rect;
	}
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	//PhysBodies
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;
	
	//Textures
	SDL_Texture* circle;	// CHANGE/FIX: Old
	SDL_Texture* box;	// CHANGE/FIX: Old
	SDL_Texture* rick;	// CHANGE/FIX: Old
	SDL_Texture* map;
	SDL_Texture* ramps;
	SDL_Texture* spriteSheet;

	//Lights
	usPoint lightPosList[12];
	SDL_Rect lightRect;

	//Static elements
	static_element missingBumper;

	static_element greenLeftLight;
	static_element greenRightLight;

	static_element pegLeft;
	static_element pegMiddle;
	static_element pegRight;

	static_element arrowLeft;
	static_element arrowMiddle;
	static_element arrowRight;

	static_element blueOverKicker;

	//Rect that covers the full screen
	SDL_Rect fullScreenRect;	// @Carles

	uint bonus_fx;
};
