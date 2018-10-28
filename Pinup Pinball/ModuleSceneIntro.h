#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "ChainCoordinates.h"

class PhysBody;
struct b2Body;
struct b2RevoluteJoint;
struct b2PrismaticJoint;
enum class collision_type;

struct static_element {	// @Carles
	iPoint position;
	SDL_Rect rect;

	void create(iPoint position, SDL_Rect rect) {
		this->position = position;
		this->rect = rect;
	}
};

struct sensor_flags {	// @Carles
	bool lightsTopLeft[4];
	bool lightsTop[4];
	bool lightsMiddle[2];
	bool lightsDown[2];

	bool activatedRamps;
	bool rampDone[2];
	bool rampEventDone[2];
	bool thirdRamp;

	bool tunnels[2];
	bool arrows[3];
	bool pegs[3];

	sensor_flags() {
		for (int i = 0; i < 4; i++) {
			lightsTopLeft[i] = false;
			lightsTop[i] = false;
		}

		activatedRamps = false;
		thirdRamp = false;

		for (int i = 0; i < 2; i++) {
			rampDone[i] = false;
			rampEventDone[i] = false;
			lightsMiddle[i] = false;
			lightsDown[i] = false;
			tunnels[i] = false;
		}

		for (int i = 0; i < 3; i++) {
			arrows[i] = false;
			pegs[i] = false;
		}
	}
};

struct score_rewards {	// @Carles
	ushort leftKicker = 5;
	ushort topLeftLight = 3;
	ushort bumper = 5;
	ushort smallBumper = 44;
	ushort tunnel = 33;
	ushort pegScore = 9;
	ushort buttonLight = 11;
	ushort enterRamp = 444;
	ushort thirdRamp = 100000;
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

public:	//@Carles

	// Sensor logic events
	void checkLightCollisions(PhysBody* bodyA, PhysBody* bodyB);
	void checkOtherCollisions(PhysBody* bodyA, PhysBody* bodyB);
	void CheckThirdRamp();
	void CheckMultiplier();
	void CheckRampEventStart();
	void CheckRampEventEnd();
	void RestorePegs(collision_type collType);
	void TeleportBall(collision_type collType);

	// Create/Delete Ramps
	void CreateRamps();
	void DeleteRamps();
	
	// Starting Allocations
	void AllocStaticElements();
	void AllocWalls();
	void AllocSensors();
	void AllocBumpers();

	// Update Cycle
	void UnderBallElements();
	void TopRightBumperLogic();
	void ArrowsLogic();
	void LightsLogic();
	void PegsLogic();
	void FlipperLogic();
	void KickerLogic();
	void RampsLogic();
	void TunnelsLogic();
	void DynamicElements();
	void OverBallElements();

public:
	//PhysBodies
	p2List<PhysBody*> circles;

	p2List<PhysBody*> smallTopWallsList;
	p2List<PhysBody*> generalWallsList;
	p2List<PhysBody*> rampWallsList;
	p2List<PhysBody*> bumpersList;
	p2List<PhysBody*> pegsList;

	PhysBody* topRightBumper = nullptr;

	// Sensors
	p2List<PhysBody*> sensorList;
	sensor_flags sensorFlags;

	//Textures
	SDL_Texture* map = nullptr;
	SDL_Texture* ramps = nullptr;
	SDL_Texture* spriteSheet = nullptr;

	//Lights
	usPoint lightPosList[12];
	SDL_Rect lightRect;

	//Static elements
	static_element missingBumper;
	static_element greenLight[2];
	static_element pegs[3];
	static_element arrows[3];
	static_element tunnels[2];
	static_element overLeftKicker;
	static_element overRightKicker;

	//Dynamic elements
	SDL_Rect ballRect;
	SDL_Rect kickerRect;

	//Score
	score_rewards scoreRewards;

	//Flags
	bool mustCreateRamps = false;
	bool mustDeleteRamps = false;
	bool mustCreateTopRightBumper = false;
	bool mustRestorePegs = false;

	//Tunnel
	int tunnelTimer = 0;
	ushort tunnelTime = 2000;

	//Wall coordinates (Pivot 0, 0)
	wall_coordinates wallCoordinates;

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
	PhysBody* bumperLeftProjection = nullptr;
	
	//Circular Bumpers
	PhysBody* bumperCRampBlocker = nullptr;
	PhysBody* bumperTopRedLeft = nullptr;
	PhysBody* bumperDownRedCenter = nullptr;
	PhysBody* bumperTopBlueRight = nullptr;

	//Chain Bumpers
	PhysBody* bumperBigLeft = nullptr;
	PhysBody* bumperHuggerLeft = nullptr;

	PhysBody* bumperBigRight = nullptr;
	PhysBody* bumperHuggerRight = nullptr;

	bumper_coordinates bumperCoordinates;

	//SFX
	uint ball_collision_fx;
	uint ball_lost_fx;
	uint big_bumper_left_fx;
	uint big_bumper_right_fx;
	uint flipper_top_fx;
	uint flipper_bottom_fx;
	uint game_over_fx;
	uint lat_light_light_up_fx;
	uint light_lights_up_fx;
	uint left_kicker_fx;
	uint tunnel_in_out_fx;
	uint ramp_entrance_fx;
	uint ramp_exit_fx;
	uint top_left_bumper_fx;
	uint peg_consumed_fx;
	
};
