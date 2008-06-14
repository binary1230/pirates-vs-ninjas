#ifndef PHYSICS_H
#define PHYSICS_H

#define PIXELS_PER_METER 50

#define PIXELS_TO_METERS(pixels_to_convert) (float(pixels_to_convert) / PIXELS_PER_METER)
#define METERS_TO_PIXELS(meters_to_convert) (float(meters_to_convert) * PIXELS_PER_METER)

class b2World;
struct b2AABB;

// -----------------------------------------------------------------------

class PhysicsDebugRenderer : public b2DebugDraw
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
};

void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);
void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
void DrawAABB(b2AABB* aabb, const b2Color& color);

// -----------------------------------------------------------------------

class PhysicsManager
{
	DECLARE_SINGLETON_CLASS(PhysicsManager)

	protected:
		float m_fPhysicsSimulatorTimeStep;
		int m_iPhysicsSimulatorIterations;
		b2World* m_pkPhysicsWorld;

		PhysicsDebugRenderer m_kPhysicsDebugRenderer;

	public:
		~PhysicsManager();

		bool Init();
		b2Body* CreatePhysicsBox( float x, float y, float width, float height );
		void Shutdown();

		void Update();
		void Draw();
};

#define PHYSICS (PhysicsManager::GetInstance())

#endif // PHYSICS_H