#ifndef PHYSICS_H
#define PHYSICS_H

#define PIXELS_PER_METER 50

#define PIXELS_TO_METERS(pixels_to_convert) (float(pixels_to_convert) / PIXELS_PER_METER)
#define METERS_TO_PIXELS(meters_to_convert) (float(meters_to_convert) * PIXELS_PER_METER)

class b2World;
struct b2AABB;
class Object;

class PhysicsContactListener : public b2ContactListener
{
	public:
		void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

		void BeginContact(b2Contact* contact);
		void EndContact(b2Contact* contact);
};

class PhysicsContact
{
	public:
		Object* objA;
		Object* objB;
		b2WorldManifold worldManifold;
};

typedef map<const b2Contact*, PhysicsContact> CollisionMapping;
typedef map<const b2Contact*, PhysicsContact>::iterator collision_iter;

class PhysicsManager
{
	DECLARE_SINGLETON_CLASS(PhysicsManager)

	protected:
		float m_fPhysicsSimulatorTimeStep;
		int m_iPhysicsSimulatorIterations;
		b2World* m_pkPhysicsWorld;

		// PhysicsDebugRenderer m_kPhysicsDebugRenderer;
		PhysicsContactListener m_kPhysicsContactListener;

		std::vector<PhysicsContact> m_kHardCollisions;

		CollisionMapping sensorMappings;

		friend class PhysicsContactListener;

		void ReportRealCollision(const b2Contact* pkb2Contact);

		void BeginSensorCollision(const b2Contact* pkb2Contact);
		void EndSensorCollision(const b2Contact* pkb2Contact);
		
		void HandleCollisions();
		void ProcessHardCollision(PhysicsContact* pkb2Contact);
		void ProcessSensorCollision(PhysicsContact* pkb2Contact);

		bool bDrawDebugBoxes;

	public:
		~PhysicsManager();

		bool Init();
		bool OnWorldInit();

		// helpers
		b2Body* CreateStaticPhysicsBox( float x, float y, float width, float height, bool bSensorOnly = false );
		b2Body* CreateDynamicPhysicsBox( float x, float y, float width, float height, bool bDontAllowRotation = false, float fDensity = 0.1f );

		b2Body* CreatePhysicsBox( float x, float y, float width, float height, float density, float restitution, float friction, bool bDontAllowRotation = false, bool bSensorOnly = false );

		void RemoveFromWorld(b2Body* pkBodyToRemove);
	
		void SetDrawDebug(bool bVal)	{ bDrawDebugBoxes = bVal; }
		bool GetDrawDebug() const		{ return bDrawDebugBoxes; }

		void Shutdown();

		void Update();
		void Draw();
};

#define PHYSICS (PhysicsManager::GetInstance())

#endif // PHYSICS_H

