#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "ModuleSceneIntro.h"
#include "p2Point.h"
#include "math.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	debug = false;
	//Stuff made by us
	body_clicked = nullptr;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	// needed to create joints like mouse joint
	b2BodyDef bd;
	ground = world->CreateBody(&bd);

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if(c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			if(pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}

	return UPDATE_CONTINUE;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius, b2BodyType bodyType, collision_type collision)
{
	b2BodyDef body;
	body.type = bodyType;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;
	pbody->collision = collision;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height, b2BodyType bodyType, collision_type collision)
{
	b2BodyDef body;
	body.type = bodyType;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;
	pbody->collision = collision;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height, b2BodyType bodyType, collision_type collision)
{
	b2BodyDef body;
	body.type = bodyType;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;
	pbody->sensor = true;
	pbody->collision = collision;

	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size, b2BodyType bodyType, collision_type collision)
{
	b2BodyDef body;
	body.type = bodyType;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for(uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;
	pbody->collision = collision;

	return pbody;
}

flipper ModulePhysics::CreateFlipper(int posX, int posY,int att_diameter, int flipper_chain[], int chain_size,
									 SDL_Rect flipper_rect, int lowerAngle, int upperAngle, flipper_type f_type)
{
	flipper f;
	iPoint PbodyDef;
	
	if (f_type == LEFT_FLIPPER)
	{
		PbodyDef.x = posX - (flipper_rect.w / 5);
		PbodyDef.y = posY - (flipper_rect.y + flipper_rect.h / 3);
	}
	else 
	{
		PbodyDef.x = posX - (2 * flipper_rect.w + 10);
		PbodyDef.y = posY - (flipper_rect.y + flipper_rect.h / 3);
	}
	
	f.Attacher = CreateAttacherBody(posX,posY,att_diameter);
	f.Pbody = CreateFlipperPbody(PbodyDef.x, PbodyDef.y, flipper_chain, chain_size);
	f.Rect = flipper_rect;
	f.Joint = CreateFlipperJoint(f,lowerAngle,upperAngle);

	return f;
}

b2Body* ModulePhysics::CreateAttacherBody(int x, int y,int diameter)
{
	b2Body* flipper_attacher; //body to Ret

	b2BodyDef flipper_attacher_body;
	flipper_attacher_body.type = b2_staticBody;
	flipper_attacher_body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	flipper_attacher = world->CreateBody(&flipper_attacher_body);

	b2CircleShape flipper_attacher_shape;
	flipper_attacher_shape.m_radius = PIXEL_TO_METERS(diameter) * 0.5f;

	b2FixtureDef flipper_attacher_fixture;
	flipper_attacher_fixture.shape = &flipper_attacher_shape;
	flipper_attacher->CreateFixture(&flipper_attacher_fixture);

	return flipper_attacher;
}

PhysBody* ModulePhysics::CreateFlipperPbody(int x, int y, int* points, int size)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box; //Dídac

	//b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for (uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	//shape.CreateLoop(p, size / 2);
	box.Set(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &box;
	//@Dídac
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

b2RevoluteJoint* ModulePhysics::CreateFlipperJoint(const flipper &f, int lowerAngle, int upperAngle)
{
	//Initialize the joint
	b2RevoluteJointDef jointDef;
	jointDef.Initialize(f.Attacher, f.Pbody->body, f.Attacher->GetWorldCenter());

	jointDef.collideConnected = false;
	//SET the limits for the joint (this will limit the angle of the flipper)
	jointDef.enableLimit = true;
	jointDef.lowerAngle = lowerAngle * DEGTORAD;
	jointDef.upperAngle = upperAngle * DEGTORAD;

	//Activate the motor ESSENTIAL STEP for flipper rotation
	jointDef.enableMotor = true;

	//Create the joint
	return (b2RevoluteJoint*)world->CreateJoint(&jointDef);
}

b2PrismaticJoint* ModulePhysics::CreatePrismaticJoint(b2Body* bodyA, b2Body* bodyB, float32 world_Axis_x,float32 world_Axis_y,
													  float32 lower_translation, float32 upper_translation)
{
	b2PrismaticJointDef jointDef;
	b2Vec2 worldAxis(world_Axis_x, world_Axis_y);
	jointDef.Initialize(bodyA, bodyB, bodyA->GetWorldCenter(), worldAxis);

	jointDef.enableLimit = true;
	jointDef.lowerTranslation = lower_translation;
	jointDef.upperTranslation = upper_translation;

	jointDef.enableMotor = true;
	jointDef.maxMotorForce = 0.0f;	//Initialize always on 0
	jointDef.motorSpeed = 0.0f;     //Initialize always on 0

	return (b2PrismaticJoint*)world->CreateJoint(&jointDef);
}

void ModulePhysics::FlipperSetMaxMotorTorque(flipper &f, float32 MaxTorque)
{
	f.Joint->SetMaxMotorTorque(MaxTorque);
}

void ModulePhysics::FlipperSetMotorSpeed(flipper &f, float32 MotorSpeed)
{
	f.Joint->SetMotorSpeed(MotorSpeed);
}

void ModulePhysics::KickerSetMaxMotorForce(kicker k, float32 MaxForce)
{
	k.joint->SetMaxMotorForce(MaxForce);
}

void ModulePhysics::KickerSetMotorSpeed(kicker k, float32 MaxSpeed)
{
	k.joint->SetMotorSpeed(MaxSpeed);
}
// 
update_status ModulePhysics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	
	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					//v = b->GetWorldPoint(shape->m_vertices[0]);
					//App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
			}

			
			PhysBody* bodyAux = (PhysBody*)f->GetBody()->GetUserData();
			
			// TODO 1: If mouse button 1 is pressed ...
			// test if the current body contains mouse position
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN 
				&& bodyAux !=nullptr
				&& bodyAux->Contains(App->input->GetMouseX(), App->input->GetMouseY()) )
			{
				body_clicked = bodyAux->body;
				break;
			}
		}
	}

	// If a body was selected we will attach a mouse joint to it
	// so we can pull it around 
	// TODO 2: If a body was selected, create a mouse joint
	// using mouse_joint class property

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN
		&& body_clicked != nullptr)
	{
		b2MouseJointDef def;
		def.bodyA = ground;
		def.bodyB = body_clicked;
		def.target = { PIXEL_TO_METERS((float32)App->input->GetMouseX()), PIXEL_TO_METERS((float32)App->input->GetMouseY()) };
		def.dampingRatio = 0.5f;
		def.frequencyHz = 2.0f;
		def.maxForce = 100.0f * body_clicked->GetMass();
		mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);
	}

	// TODO 3: If the player keeps pressing the mouse button, update
	// target position and draw a red line between both anchor points

		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && body_clicked != nullptr)
		{
			//The mouse Joint previously created (def) is in the world JointList
			
				//UPDATE THE TARGET
				mouse_joint->SetTarget({ PIXEL_TO_METERS((float32)App->input->GetMouseX()), PIXEL_TO_METERS((float32)App->input->GetMouseY()) });

			//Draw the line between both points
				App->renderer->DrawLine(METERS_TO_PIXELS(mouse_joint->GetAnchorB().x), METERS_TO_PIXELS(mouse_joint->GetAnchorB().y),
					METERS_TO_PIXELS(mouse_joint->GetTarget().x), METERS_TO_PIXELS( mouse_joint->GetTarget().y),
					255, 0, 0, 255);
			
		}
		
	// TODO 4: If the player releases the mouse button, destroy the joint
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && mouse_joint != nullptr)
			{
				world->DestroyJoint(mouse_joint);
				mouse_joint = nullptr;
				body_clicked = nullptr;
			}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if(physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if(physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}