
#ifndef CAFFEINE_COMPONENTS_LIGHT_INCLUDED
#define CAFFEINE_COMPONENTS_LIGHT_INCLUDED

#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Application/ApplicationFwd.hpp>

namespace Caffeine {
namespace Components {

namespace LightUtils {
	void UploadShaderInfo(CaffApp::Renderer &renderer);
}

class DirectionalLightComponent : public Entity::Component
{
public:

	COMPONENT_RTTI(DirectionalLightComponent)

	explicit 					DirectionalLightComponent();
								~DirectionalLightComponent();

	void 						onParams(const CaffUtil::Param &params) override;

	inline CaffMath::Vector3 	getColor() const { return m_color; }
	inline void 				setColor(const CaffMath::Vector3 &vec) { m_color = vec; }

	inline CaffMath::Vector3 	getAmbient() const { return m_ambient; }
	inline void					setAmbient(const CaffMath::Vector3 &vec) { m_ambient = vec; }

	inline float 				getIntensity() const { return m_intensity; }
	inline void 				setIntensity(float i) { m_intensity = i; }

private:

	const uint32_t 				m_id;

	CaffMath::Vector3 			m_color;
	CaffMath::Vector3			m_ambient;

	float 						m_intensity;

}; // class

class PointLightComponent : public Entity::Component
{
public:

	COMPONENT_RTTI(PointLightComponent)

	explicit 					PointLightComponent();
								~PointLightComponent();

	void 						onParams(const CaffUtil::Param &params) override;

	inline CaffMath::Vector3 	getColor() const { return m_color; }
	inline void 				setColor(const CaffMath::Vector3 &vec) { m_color = vec; }

	inline float 				getIntensity() const { return m_intensity; }
	inline void 				setIntensity(float val) { m_intensity = val; }

	inline float 				getRadius() const { return m_radius; }
	inline void 				setRadius(float val) { m_radius = val; }
	
private:

	const uint32_t 				m_id;

	CaffMath::Vector3 			m_color;

	float 						m_intensity;
	float 						m_radius;

}; // class


} // namespace
} // namespace

#endif