
#ifndef CAFFEINE_COMPONENTS_MATERIAL_INCLUDED
#define CAFFEINE_COMPONENTS_MATERIAL_INCLUDED


#include <Caffeine/Platform.hpp>
#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Entity/Entity.hpp>
#include <Caffeine/Systems/ModelManager.hpp>
#include <Caffeine/Utilities/Logging.hpp>
#include <Caffeine/Application/Renderer.hpp>
#include <Caffeine/Math/Vector.hpp>
#include <Caffeine/Math/Matrix.hpp>


namespace Caffeine {
namespace Components {


class MaterialComponent : public CaffEnt::Component
{
protected:

	explicit MaterialComponent()
	: CaffEnt::Component()
	, m_model(nullptr)
	{
	}

public:

	COMPONENT_RTTI(MaterialComponent)
	
	~MaterialComponent() {}
	
	
    virtual void		setWVPMat(const Math::Matrix44 &mat)			 { WARNING_LOG("Material Component: No setWVPMat() function defined for this material.");				}
    virtual void		setWorldMat(const Math::Matrix44 &mat)			 { WARNING_LOG("Material Component: No setWorldMat() function defined for this material.");				}
    virtual void		setViewMat(const Math::Matrix44 &mat)			 { WARNING_LOG("Material Component: No setViewMat() function defined for this material.");				}
    virtual void		setProjectionMat(const Math::Matrix44 &mat)		 { WARNING_LOG("Material Component: No setProjectionMat() function defined for this material.");		}
	
    virtual void		setRGB(const Math::Vector3 &vec)				 { WARNING_LOG("Material Component: No setRGB() function defined for this material.");					}
    virtual void		setRGBA(const Math::Vector4 &vec)				 { WARNING_LOG("Material Component: No setRGBA() function defined for this material.");					}
	
    virtual void		setDiffuseMap(const CaffApp::TextureID texID)    { WARNING_LOG("Material Component: No setDiffuseMap() function defined for this material.");			}
	virtual void		setDiffuseMapScale(const Math::Vector2 &vec)	 { WARNING_LOG("Material Component: No setDiffuseMapScale() function defined for this material.");		}
	
    virtual void		setNormalMap(const CaffApp::TextureID texID)     { WARNING_LOG("Material Component: No setNormalMap() function defined for this material.");			}
	virtual void		setNormalMapScale(const Math::Vector2 &vec)		 { WARNING_LOG("Material Component: No setNormalMapScale() function defined for this material.");		}
	
    virtual void		setSpecularMap(const CaffApp::TextureID texID)   { WARNING_LOG("Material Component: No setSpecularMap() function defined for this material.");			}
	virtual void		setSpecularMapScale(const Math::Vector2 &vec)	 { WARNING_LOG("Material Component: No setSpecularMapScale() function defined for this material.");		}
	
	
	inline bool setModelData(const std::string &model)
	{		
		m_model = &(getOwner().getModelManager().getModelData(model));
		
		return hasModelData();
	}
	
protected:

	inline bool hasModelData() const { return m_model != nullptr; }

	inline CaffSys::ModelData * getModelData() const
	{
		if(!hasModelData())
		{
			WARNING_LOG("Material Component: No model data found.");
		}
	
		return m_model;
	}

private:

	CaffSys::ModelData * m_model;
	
};



class FullbrightMaterialComponent : public MaterialComponent
{
public:

	explicit			FullbrightMaterialComponent();

	void				onParams(const CaffUtil::Param &params) override;
	void				onStart() override;

	inline void			setRGBA(const Math::Vector4 &vec) override				{ m_rgba = vec;				}
	inline void			setDiffuseMap(const CaffApp::TextureID texID) override	{ m_diffuseMapID = texID;	}
	
private:

	void				onRender() override;
	
private:

	Math::Vector2		m_diffuseScale;
	Math::Vector4		m_rgba;
	std::string			m_textureName;
	CaffApp::TextureID  m_diffuseMapID;
};



class BlinnPhongMaterialComponent : public MaterialComponent
{
public:

	explicit		BlinnPhongMaterialComponent();

	void			onParams(const CaffUtil::Param &params) override;
	void			onStart() override;

	inline void		setRGBA(const Math::Vector4 &vec) override { m_rgba = vec; }
	inline void		setDiffuseMap(const CaffApp::TextureID texID) override { m_diffuseMapID = texID; }

private:

	void			onRender() override;

private:

	Math::Vector2		m_diffuseScale;
	Math::Vector4		m_rgba;
	std::string			m_textureName;
	CaffApp::TextureID	m_diffuseMapID;
};


} // namespace
} // namespace



#endif // include guard
