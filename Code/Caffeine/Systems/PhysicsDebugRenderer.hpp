
#ifndef CAFFEINE_SYSTEMS_PHYSICS_DEBUG_RENDERER_INCLUDED
#define CAFFEINE_SYSTEMS_PHYSICS_DEBUG_RENDERER_INCLUDED

#include <Caffeine/Platform.hpp>
#include <LinearMath/btIDebugDraw.h>

namespace Caffeine {
namespace Systems {

class PhysicsDebugRenderer : public btIDebugDraw
{
public:

	explicit			PhysicsDebugRenderer();
	virtual				~PhysicsDebugRenderer();
	
	void				drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) override;
	void				drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color) override;
	
	void				reportErrorWarning(const char* warningString) override;
	void				draw3dText(const btVector3 &location, const char *textString) override;
	void				setDebugMode(int debugMode) override;
    int					getDebugMode() const override;
	
private:

	int m_debugMode;
	
}; // class

} // namespace
} // namespace

#endif // include guard
