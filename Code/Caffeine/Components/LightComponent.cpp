
#include <Caffeine/Components/LightComponent.hpp>
#include <Caffeine/Components/TransformComponent.hpp>
#include <Caffeine/Entity/Entity.hpp>
#include <Caffeine/Application/Renderer.hpp>
#include <Caffeine/Utilities/Timer.hpp>

#include <cstdio>
#include <vector>

namespace {

	//TODO: Move this into the state
	std::map<uint32_t, CaffComp::DirectionalLightComponent*> directionalLights;
	std::map<uint32_t, CaffComp::PointLightComponent*> pointLights;

	uint32_t nextId = 0;
}



namespace Caffeine {
namespace Components {
namespace LightUtils {



void UploadShaderInfo(CaffApp::Renderer &renderer)
{
	auto getTransform = [](CaffEnt::Component *c) 
	{
		const CaffEnt::Entity &owner = c->getOwner();
		const TransformComponent *transform = owner.getComponent<TransformComponent>();
		assert(transform);

		return transform;	
	};

	//uint32_t dId = 0;

	for(const auto &it : directionalLights)
	{
		const auto &l = it.second;

		CaffMath::Vector3 ambient = { 0.f, 0.f, 0.f };

		for(const auto &it : directionalLights)
		{
			const auto &l = it.second;

			const CaffMath::Vector3 direction = getTransform(l)->getForwardVec();
			const CaffMath::Vector3 color = CaffMath::Vector3Scale(l->getColor(), l->getIntensity());
			
			//TODO: Multiple directional lights!
			renderer.setShader3f("directionalLight.direction", &direction.x);
			renderer.setShader3f("directionalLight.color", &color.x);

			ambient = CaffMath::Vector3ComponentAdd(ambient, l->getAmbient());
		}

		renderer.setShader3f("ambientLight", &ambient.x);

		uint32_t pId = 0;

		const CaffMath::Vector3 position = getTransform(l)->getPosition();
		const CaffMath::Vector3 color = CaffMath::Vector3Scale(l->getColor(), l->getIntensity());

		//TODO: There's gotta be a better way to do this :/
		char buffer[64];

		std::sprintf(buffer, "pointLights[%u].position", pId);
		renderer.setShader3f(buffer, &position.x);

		std::sprintf(buffer, "pointLights[%u].color", pId);
		renderer.setShader3f(buffer, &color.x);

		pId++;
	}
}
	
	
	
} // namespace



DirectionalLightComponent::DirectionalLightComponent()
: Entity::Component()
, m_id(nextId++)
, m_color(CaffMath::Vector3Init(1.f, 1.f, 1.f))
, m_ambient(CaffMath::Vector3Init(0.f, 0.f, 0.f))
, m_intensity(1.f)
{
	directionalLights[m_id] = this;
}



DirectionalLightComponent::~DirectionalLightComponent()
{
	auto it = directionalLights.find(m_id);
	directionalLights.erase(it);
}



void DirectionalLightComponent::onParams(const CaffUtil::Param &params)
{
	enum { X = 0, Y, Z };

	const std::array<float, 3> color = params["color"].asFloat3({m_color.x, m_color.y, m_color.z});
	m_color = CaffMath::Vector3Init(color.at(X), color.at(Y), color.at(Z));

	const std::array<float, 3> ambient = params["ambient"].asFloat3({m_ambient.x, m_ambient.y, m_ambient.z});
	m_ambient = CaffMath::Vector3Init(ambient.at(X), ambient.at(Y), ambient.at(Z));

	m_intensity = params["intensity"].asFloat(m_intensity);
}



PointLightComponent::PointLightComponent()
: Entity::Component()
, m_id(nextId++)
, m_color(CaffMath::Vector3Init(1.f, 1.f, 1.f))
, m_intensity(1.f)
, m_radius(10.f)
{
	pointLights[m_id] = this;
}



PointLightComponent::~PointLightComponent()
{
	auto it = pointLights.find(m_id);
	pointLights.erase(it);
}



void PointLightComponent::onParams(const CaffUtil::Param &params)
{
	const std::array<float, 3> color = params["color"].asFloat3({m_color.x, m_color.y, m_color.z});

	enum { X = 0, Y, Z };
	m_color = CaffMath::Vector3Init(color.at(X), color.at(Y), color.at(Z));

	m_intensity = params["intensity"].asFloat(m_intensity);
	m_radius	= params["radius"].asFloat(m_radius);
}



} // namespace
} // namespace
