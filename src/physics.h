#ifndef PHYSICS_H
#define PHYSICS_H

#define PIXELS_PER_METER 50

#define PIXELS_TO_METERS(pixels_to_convert) (float(pixels_to_convert) / PIXELS_PER_METER)
#define METERS_TO_PIXELS(meters_to_convert) (float(meters_to_convert) * PIXELS_PER_METER)

class b2World;
struct b2AABB;
class Object;

// -----------------------------------------------------------------------

/*class PhysicsDebugRenderer : public b2DebugDraw
{
public:
	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

	void SubmitVertex( float x, float y );
	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
	void DrawXForm(const b2XForm& xf);

	void Transform(float &x, float &y);
};*/

void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);
void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
void DrawAABB(b2AABB* aabb, const b2Color& color);

// -----------------------------------------------------------------------

class PhysicsContactListener : public b2ContactListener
{
	public:
		void Add(const b2Contact* point);
		void Persist(const b2Contact* point);
		void Remove(const b2Contact* point);
		void Result(const b2ContactResult* point);
};

class PhysicsContact
{
	public:
		Object* objA;
		Object* objB;
		b2WorldManifold worldManifold;
};

class PhysicsManager
{
	DECLARE_SINGLETON_CLASS(PhysicsManager)

	protected:
		float m_fPhysicsSimulatorTimeStep;
		int m_iPhysicsSimulatorIterations;
		b2World* m_pkPhysicsWorld;

		// PhysicsDebugRenderer m_kPhysicsDebugRenderer;
		PhysicsContactListener m_kPhysicsContactListener;

		std::vector<PhysicsContact> m_kContacts;

		friend class PhysicsContactListener;

		void Reportb2Contact(const b2Contact* pkb2Contact);
		
		void HandleCollisions();
		void ProcessCollision(PhysicsContact* pkb2Contact);

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

