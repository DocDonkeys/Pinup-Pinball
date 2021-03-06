#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

struct flipper;  // So we don't need to include headers
struct kicker;   // """""""""""""""""""""""""""""""""
struct SDL_Rect; // """""""""""""""""""""""""""""""""
enum flipper_type;

enum class collision_type {	//@Carles
	NONE = -1,

	LIGHT_TOP_LEFT_1,
	LIGHT_TOP_LEFT_2,
	LIGHT_TOP_LEFT_3,	// FIX/CHANGE: Disable on ramp map
	LIGHT_TOP_LEFT_4,	// FIX/CHANGE: Disable on ramp map

	LIGHT_TOP_1,
	LIGHT_TOP_2,
	LIGHT_TOP_3,
	LIGHT_TOP_4,

	LIGHT_LEFT,
	LIGHT_RIGHT,

	LIGHT_DOWN_LEFT,
	LIGHT_DOWN_RIGHT,

	ARROW_LEFT,
	ARROW_MIDDLE,
	ARROW_RIGHT,

	RAMP_ACTIVATE,
	RAMP_DEACTIVATE,

	RAMP_LEFT_FINISH,
	RAMP_RIGHT_FINISH,
	THIRD_RAMP,

	LEFT_KICKER,

	TUNNEL_LEFT,
	TUNNEL_RIGHT,	// FIX/CHANGE: Disable on ramp map

	BUMPER_LEFT,
	BUMPER_RIGHT,
	SMALL_BUMPER,

	PEG_LEFT,
	PEG_MIDDLE,
	PEG_RIGHT,

	LOSE_BALL,

	MAX_COLLISIONS
};

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

	bool mustDestroy = false;
	bool sensor = false;
	collision_type collision = collision_type::NONE;

public:
	int width, height;
	b2Body* body;
	Module* listener;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius, b2BodyType = b2_dynamicBody, collision_type = collision_type::NONE, float32 fixture_restitution = 0.3f);
	PhysBody* CreateRectangle(int x, int y, int width, int height, b2BodyType = b2_dynamicBody, collision_type = collision_type::NONE, float32 fixture_restitution = 0.0f);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height, b2BodyType = b2_staticBody, collision_type = collision_type::NONE);
	PhysBody* CreateChain(int x, int y, int* points, int size, b2BodyType = b2_dynamicBody, collision_type = collision_type::NONE, float32 fixture_restitution = 0.0f);

	//@D�dac
	//flipper  CreateFlipper();
	b2Body* CreateAttacherBody(int x, int y, int diameter);
	PhysBody* CreateFlipperPbody(int x, int y, int* points, int size);
	b2RevoluteJoint* CreateFlipperJoint(const flipper &f, int lowerAngle, int upperAngle);
	flipper CreateFlipper(int posX, int posY, int att_diameter, int flipper_chain[], int chain_size,
						  SDL_Rect flipper_rect, int lowerAngle, int upperAngle, flipper_type f_type);
	//Flipper movement
	void FlipperSetMaxMotorTorque(flipper &f, float32 MaxTorque);
	void FlipperSetMotorSpeed(flipper &f, float32 MotorSpeed);

	//Kicker
	b2PrismaticJoint* CreatePrismaticJoint(b2Body* bodyA, b2Body* bodyB, float32 world_Axis_x, float32 world_Axis_y,
										   float32 lower_translation, float32 upper_translation);
	kicker CreateKicker(int posX, int posY, int att_diameter,SDL_Rect rect, float32 axisX, float32 axisY, 
						float32 low_translation,float32 upp_translation);
	//KICKER movement
	void KickerSetMaxMotorForce(kicker k, float32 MaxForce);
	void KickerSetMotorSpeed(kicker k, float32 MaxSpeed);
	
	// b2ContactListener ---
	void BeginContact(b2Contact* contact);

	//Get the body clicked of the mouse_joint
	b2Body* GetBodyClicked();
	void DestroyMouseJoint();

public:
	b2World* GetWorld() const {
		return world;
	}
	bool GetDebug() const {
		return debug;
	}

private:
	b2World* world;	// IMPROVE: Add functions to physics instead of using world on other modules
	bool debug;
	b2MouseJoint* mouse_joint;
	b2Body* ground;
	//Vars created by me
	b2Body* body_clicked;
};