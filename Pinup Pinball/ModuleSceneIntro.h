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

enum flipper_type
{
	LEFT_FLIPPER = 0,
	RIGHT_FLIPPER,
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

	p2List<PhysBody*> smallTopWallsList;

	p2List<PhysBody*> outsideWallsList;
	p2List<PhysBody*> topLeftWallsList;
	p2List<PhysBody*> downLeftWallsList;
	p2List<PhysBody*> downRightWallsList;
	p2List<PhysBody*> rampWallsList;

	p2List<PhysBody*> sensorList;

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

	static_element tunnelLeft;
	static_element tunnelRight;

	static_element overLeftKicker;
	static_element overRightKicker;

	//Dynamic elements
	SDL_Rect ballRect;
	SDL_Rect kickerRect;

	//Wall coordinates (Pivot 0, 0)
	int outsideWalls[231] = {
		402, 793,
		400, 423,
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
		45, 452,
		32, 423,
		55, 411,
		72, 430,
		56, 405,
		28, 420,
		25, 500,
		3, 500,
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
		181, 153,
		159, 156,
		154, 134,
		210, 130,
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
		422, 1500,
		403, 1500
	};

	int topLeftWalls[32] = {
		32, 380,
		27, 380,
		27, 151,
		32, 151,
		32, 222,
		40, 241,
		46, 242,
		44, 250,
		62, 293,
		82, 301,
		80, 308,
		68, 305,
		54, 290,
		49, 280,
		34, 281,
		32, 379
	};

	int downLeftWalls[18] = {
	27, 539,
	32, 539,
	32, 636,
	37, 644,
	144, 731,
	127, 742,
	31, 693,
	27, 688,
	27, 540
	};

	int downRightWalls[18] = {
	368, 539,
	372, 539,
	372, 688,
	369, 692,
	270, 742,
	253, 731,
	360, 648,
	367, 637,
	368, 540
	};

	//Rect that covers the full screen
	SDL_Rect fullScreenRect;	// @Carles

	//Flippers @Dídac
	flipper leftFlipper;
	flipper rightFlipper;

	SDL_Rect leftFlipperRect;
	SDL_Rect rightFlipperRect;

	//Kicker
	kicker pinballKicker;

	//Rectangular Bumpers
	PhysBody* bumperLeftProjection;
	
	//Circular Bumpers
	PhysBody* bumperCRampBlocker;
	PhysBody* bumperTopRedLeft;
	PhysBody* bumperDownRedCenter;
	PhysBody* bumperTopBlueRight;

	//Chain Bumpers
	PhysBody* bumperBigLeft;
	PhysBody* bumper_hugger_left;

	PhysBody* bumperBigRight;
	PhysBody* bumper_hugger_right;

	//Chains of the Chain Bumpers
	int left_bumper[8] = {
		78, 580,
		120, 658,
		110, 662,
		68, 583
	};

	// Pivot 0, 0
	int left_bumper_h[28] = {
		68, 583,
		78, 580,
		71, 567,
		66, 566,
		62, 566,
		59, 568,
		56, 572,
		56, 630,
		111, 677,
		119, 677,
		124, 672,
		124, 664,
		120, 658,
		110, 662
	};


	// Pivot 0, 0
	int right_bumper[8] = {
		332, 582,
		322, 578,
		280, 657,
		289, 661
	};

	// Pivot 0, 0
	int right_bumper_h[28] = {
		332, 582,
		322, 578,
		329, 567,
		335, 566,
		340, 567,
		344, 573,
		344, 630,
		289, 677,
		282, 677,
		278, 674,
		276, 670,
		276, 664,
		280, 657,
		289, 661
	};

	uint bonus_fx;
};
