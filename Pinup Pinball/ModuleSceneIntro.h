#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;
struct b2Body;
struct b2RevoluteJoint;
struct b2PrismaticJoint;

struct static_element {	// @Carles
	iPoint position;
	SDL_Rect rect;

	void create(iPoint position, SDL_Rect rect) {
		this->position = position;
		this->rect = rect;
	}
};


struct flipper
{
	//Bodies for the flipper
	b2Body* Attacher;
	PhysBody* Pbody;
	//Joint of the flipper elements
	b2RevoluteJoint* Joint;
	//Rect for blitting purposes
	SDL_Rect Rect;
};

struct kicker
{
	//Bodies for the kicker
	b2Body* attacher;
	PhysBody* pbody;
	//Joint of the kicker
	b2PrismaticJoint* joint;
	//Rect for blitting purposes
	SDL_Rect rect;
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
	p2List<PhysBody*> walls;

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

	//Dynamic elements
	SDL_Rect ballRect;
	SDL_Rect kickerRect;

	//Wall coordinates
	// Pivot 0, 0
	int mapOutsideWalls[232] = {
		402, 793,	//402, 793,
		400, 423,	//404, 423,
		398, 423,
		397, 438,
		367, 464,
		366, 497,
		398, 526,
		397, 697,
		391, 710,
		373, 721,
		234, 786,
		234, 822,
		166, 822,
		164, 785,
		13, 713,
		4, 699,
		3, 519,
		32, 498,
		31, 462,
		42, 455,
		32, 423,
		55, 411,
		72, 430,
		56, 405,
		28, 420,
		25, 436,
		8, 436,
		3, 427,
		3, 150,
		8, 137,
		19, 129,
		125, 128,
		127, 136,
		128, 195,
		131, 209,
		129, 225,
		116, 249,
		108, 251,
		110, 258,
		100, 272,
		90, 279,
		94, 285,
		100, 279,
		105, 269,
		130, 234,
		141, 202,
		164, 174,
		192, 164,
		224, 161,
		282, 170,
		281, 194,
		222, 183,
		184, 190,
		175, 199,
		175, 219,
		184, 225,
		223, 229,
		252, 226,
		276, 224,
		287, 211,
		294, 202,
		297, 189,
		301, 176,
		291, 156,
		256, 152,
		181, 152,
		159, 156,
		154, 138,
		210, 131,
		263, 130,
		294, 134,
		314, 140,
		327, 150,
		337, 164,
		342, 184,
		348, 191,
		367, 174,
		376, 164,
		389, 143,
		393, 124,
		371, 33,
		35, 44,
		33, 88,
		125, 101,
		138, 111,
		141, 118,
		111, 122,
		73, 118,
		29, 113,
		10, 95,
		9, 44,
		14, 27,
		28, 16,
		43, 13,
		199, 12,
		370, 7,
		388, 12,
		396, 26,
		409, 74,
		420, 120,
		417, 152,
		409, 184,
		403, 211,
		402, 257,
		402, 280,
		428, 297,
		427, 338,
		397, 313,
		396, 324,
		396, 365,
		412, 380,
		420, 392,
		422, 416,
		422, 793,
		412, 797,
		403, 794
	};

	//Rect that covers the full screen
	SDL_Rect fullScreenRect;	// @Carles

	//Flippers @Dídac
	flipper leftFlipper;
	flipper rightFlipper;

	SDL_Rect leftFlipperRect;
	SDL_Rect rightlipperRect;

	//Kicker
	kicker pinballKicker;

	uint bonus_fx;
};
