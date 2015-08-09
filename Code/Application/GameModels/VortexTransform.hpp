
#ifndef VORTEX_TRANSFORM_INCLUDED
#define VORTEX_TRANSFORM_INCLUDED


#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>


//#define FWD_DEC_EVENT(type)
//
//FWD_DEC_EVENT(VortexMove)
//
//namespace VortexMove
//{
//    enum ENUM : uint32_t;
//}
//
//#define EVENT(event, ...) namespace event { enum ENUM : uint32_t { __VA_ARGS__ }; }
//
//EVENT(VortexMove, OBJ_ID, POSITION)



//enum class VortexEvent : uint32_t;


/*
	For a given depth/angle it will calculate the entities position on the vortex.
	It will also fire an event if you are outside the bounderies of the vortex.
*/
class VortexTransform : public CaffEnt::Component
{
public:


	COMPONENT_RTTI(VortexTransform)
	
	
	// ** Component Interface ** //
	
	inline void				setRadius(const float radius)	{ m_vortexRadius = radius;	}
	inline float			getRadius() const				{ return m_vortexRadius;	}
	
	inline void				setAngle(const float radians)	{ m_angle = radians;		}
	inline float			getAngle() const				{ return m_angle;			}
	
	void					setDepth(const float depth)		{ m_depth = depth;			}
	inline float			getDepth() const				{ return m_depth;			}
	
	CaffMath::Vector3		getPosition() const;
	
	void					setDeathZone(const float nearZone, const float farZone);
	bool					isInZone() const;
	inline float			getNearZone()	{ return m_nearZone; }
	inline float			getFarZone()	{ return m_farZone;  }

	
private:


	// ** Engine Hooks ** //
	
	void					onParams(const CaffUtil::Param &params) override;
	void					onStart() override;
	void					onThink(const float dt) override;


private:


	float					m_depth                 = 0.f;
	float					m_angle                 = 0.f;
	float					m_vortexRadius          = 5.f;
	float					m_nearZone              = 20.f;
	float					m_farZone               = -20.f;
	bool					m_destroyOnOutOfBounds	= true;
    

}; // class


COMPONENT_FACTORY_INTERFACE(VortexTransform)


#endif // include guard
