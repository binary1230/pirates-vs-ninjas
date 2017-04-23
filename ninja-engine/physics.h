#ifndef PHYSICS_H
#define PHYSICS_H

#include "physicsDebugRenderer.h"

#define PIXELS_PER_METER 45

#define PIXELS_TO_METERS(pixels_to_convert) (float(pixels_to_convert) / PIXELS_PER_METER)
#define METERS_TO_PIXELS(meters_to_convert) (float(meters_to_convert) * PIXELS_PER_METER)

class b2World;
struct b2AABB;
class Object;

class PhysicsContactListener : public b2ContactListener
{
	public:
		// void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

		void BeginContact(b2Contact* contact);
		void EndContact(b2Contact* contact);
};

class PhysicsContactInfo
{
	public:
		Object* objA;
		Object* objB;
		b2WorldManifold worldManifold;
};

typedef map<const b2Contact*, PhysicsContactInfo> ContactMappings;
typedef map<const b2Contact*, PhysicsContactInfo>::iterator contact_iter;

class PhysicsManager
{
	DECLARE_SINGLETON_CLASS(PhysicsManager)

	protected:
		float m_fPhysicsSimulatorTimeStep;
		int m_iPhysicsSimulatorIterations;
		b2World* m_pkPhysicsWorld;

		PhysicsDebugRenderer m_kPhysicsDebugRenderer;
		PhysicsContactListener m_kPhysicsContactListener;

		ContactMappings m_currentContacts;
		
		void ProcessCollisions();
		void ProcessCollision(PhysicsContactInfo* pkb2Contact);

		friend class PhysicsContactListener;

	public:
		~PhysicsManager();

		bool Init();
		bool OnWorldInit();

		// helpers
		b2Body* CreateStaticPhysicsBox( float x, float y, float width, float height, bool bSensorOnly = false );
		
		b2Body* CreateDynamicPhysicsBox(	float x, float y, float width, float height, 
											bool bDontAllowRotation = false, float fDensity = 0.1f, bool useRoundedBottom = false);

		b2Body* CreatePhysicsBox(	float x, float y, float width, float height, 
									float density, float restitution, float friction, 
									bool bDontAllowRotation = false, bool bSensorOnly = false, 
									bool useRoundedBottom = false );

		void CreatePolygonWithRoundedEdges(float hx, float hy, b2PolygonShape & shapeOut);

		void UpdatePhysicsBodyPosition(b2Body * body, float x, float y, int width, int height);

		void RemoveFromWorld(b2Body* pkBodyToRemove);
	
		void Shutdown();

		void Update();
		void Draw();
};

#define PHYSICS (PhysicsManager::GetInstance())

#endif // PHYSICS_H

