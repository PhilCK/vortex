
#include <Caffeine/Systems/PhysicsDebugRenderer.hpp>
#include <Caffeine/Systems/DebugRenderer.hpp>
#include <Caffeine/Utilities/Logging.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Caffeine {
namespace Systems {


PhysicsDebugRenderer::PhysicsDebugRenderer()
{
	int drawModes(btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawContactPoints);

	setDebugMode(drawModes);
}



PhysicsDebugRenderer::~PhysicsDebugRenderer()
{
}



void PhysicsDebugRenderer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color)
{
	const glm::vec3 start(from.x(), from.y(), from.z());
	const glm::vec3 end(to.x(), to.y(), to.z());
	const glm::vec3 lineColor(color.x(), color.y(), color.z());

	DebugRenderer::DrawLine(start, end, lineColor);
}



void PhysicsDebugRenderer::drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color)
{
//	const glm::vec3 center(PointOnB.x(), PointOnB.y(), PointOnB.z());
//	const glm::vec3 cubeSize(0.1f, 0.1f, 0.1f);
//	const glm::vec3 cubeColor(color.x(), color.y(), color.z());
//
//	DebugRenderer::DrawCube(center, cubeSize, cubeColor);
}



void PhysicsDebugRenderer::reportErrorWarning(const char *warningString)
{
	// TODO: DebugLog better for this.
	std::cout << "[BulletPhysics Warning]: " << warningString << std::endl;
}



void PhysicsDebugRenderer::draw3dText(const btVector3 &location, const char *textString)
{
}



void PhysicsDebugRenderer::setDebugMode(int debugMode) { m_debugMode = debugMode; }



int PhysicsDebugRenderer::getDebugMode() const { return m_debugMode; }


} // namespace
} // namespace
