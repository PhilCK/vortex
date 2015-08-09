
/*
	Renderable Component
	--------------------
	
	Project:
	Caffeine
	
	Purpose:
	This currently only provides a virtual render function, however in time rendering should
	become more abstracted into here.
 */

#ifndef CAFFEINE_COMPONENTS_RENDERABLE_INCLUDED
#define CAFFEINE_COMPONENTS_RENDERABLE_INCLUDED


#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Application/ApplicationFwd.hpp>
#include <Caffeine/Systems/SystemFwd.hpp>


namespace Caffeine {
namespace Components {
    
namespace RenderableUtils
{
    void SetRenderer(CaffApp::Renderer *renderer);
    void SetTextRenderer(CaffSys::TextRenderer *textRenderer);
    void SetCameraManager(CaffSys::CameraManager *camManager);
    void SetTextureManager(CaffSys::TextureManager *textureManager);
    void SetModelManager(CaffSys::ModelManager *modelManager);
}


class RenderableComponent : public Entity::Component
{
public:

	COMPONENT_RTTI(Renderable)

	explicit					RenderableComponent();
								~RenderableComponent();
				
	void						requestModules();
	virtual void				onRender() {}
	
	bool						onRequest(const CaffSys::Event &data);
	
protected:

	CaffApp::Renderer &			getRenderer() const;
	CaffSys::TextRenderer &		getTextRenderer() const;
	CaffSys::TextureManager &	getTextureManager() const;
	CaffSys::ModelManager &		getModelManager() const;
	CaffSys::CameraManager &	getCameraManager() const;

private:

	CaffSys::EventCallback      m_requestCallBack;
	
//	CaffApp::Renderer			*m_rendererModule;
//	CaffSys::TextRenderer		*m_textRendererModule;
//	CaffSys::TextureManager		*m_textureMgrModule;
//	CaffSys::ModelManager		*m_modelMgrModule;
//	CaffSys::CameraManager		*m_cameraMgrModule;
	
}; // class


} // namespace
} // namespace

#endif // include guard
