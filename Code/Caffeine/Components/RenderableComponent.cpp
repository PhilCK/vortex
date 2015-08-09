
// TODO: Kill me, this component is no longer used. *I THINK

#include <Caffeine/Components/RenderableComponent.hpp>
#include <Caffeine/Systems/EventManager.hpp>
#include <Caffeine/Systems/Event.hpp>


namespace {
    
    CaffApp::Renderer       *renderer(nullptr);
    CaffSys::TextRenderer   *textRenderer(nullptr);
    CaffSys::CameraManager  *cameraManager(nullptr);
    CaffSys::TextureManager *textureManager(nullptr);
    CaffSys::ModelManager   *modelManager(nullptr);
    
}


namespace Caffeine {
namespace Components {


//namespace RenderableUtils
//{
//    void SetRenderer(CaffApp::Renderer *setRenderer)                    { renderer = setRenderer;               }
//    void SetTextRenderer(CaffSys::TextRenderer *setTextRenderer)        { textRenderer = setTextRenderer;       }
//    void SetCameraManager(CaffSys::CameraManager *setCamManager)        { cameraManager = setCamManager;        }
//    void SetTextureManager(CaffSys::TextureManager *setTextureManager)  { textureManager = setTextureManager;   }
//    void SetModelManager(CaffSys::ModelManager *setModelManager)        { modelManager = setModelManager;       }
//}    


RenderableComponent::RenderableComponent()
: Entity::Component()
, m_requestCallBack()
//, m_rendererModule(nullptr)
//, m_textRendererModule(nullptr)
//, m_textureMgrModule(nullptr)
//, m_modelMgrModule(nullptr)
//, m_cameraMgrModule(nullptr)
{
	m_requestCallBack = std::bind(&RenderableComponent::onRequest, this, std::placeholders::_1);
}



RenderableComponent::~RenderableComponent()
{
}



void RenderableComponent::requestModules()
{
//	const CaffEnt::Entity *owner = getOwner();
//	assert(owner);
//
//	CaffSys::EventManager *evtMgr = owner->getEventManager();
//	assert(evtMgr);
//	
//	// Register for modules access
//	evtMgr->addController(&m_requestCallBack, CaffEvent::MODULE_TEX_MANAGER_ACCESS_MSG);
//	evtMgr->addController(&m_requestCallBack, CaffEvent::MODULE_CAM_MANAGER_ACCESS_MSG);
//	evtMgr->addController(&m_requestCallBack, CaffEvent::MODULE_RENDERER_ACCESS_MSG);
//	evtMgr->addController(&m_requestCallBack, CaffEvent::MODULE_TEXT_RENDERER_ACCESS_MSG);
//	evtMgr->addController(&m_requestCallBack, CaffEvent::MODULE_MODEL_MANAGER_ACCESS_MSG);
//	
//	// Ask for access.
//	evtMgr->sendInstantEvent(CaffEvent::MODULE_TEX_MANAGER_REQUEST_MSG);
//	evtMgr->sendInstantEvent(CaffEvent::MODULE_CAM_MANAGER_REQUEST_MSG);
//	evtMgr->sendInstantEvent(CaffEvent::MODULE_RENDERER_REQUEST_MSG);
//	evtMgr->sendInstantEvent(CaffEvent::MODULE_TEXT_RENDERER_REQUEST_MSG);
//	evtMgr->sendInstantEvent(CaffEvent::MODULE_MODEL_MANAGER_REQUEST_MSG);
}



bool RenderableComponent::onRequest(const CaffSys::Event &data)
{
//	CaffEnt::Entity *owner = getOwner();
//	assert(owner);
//
//	CaffSys::EventManager *evtMgr = owner->getEventManager();
//	assert(evtMgr);
//
//	if(data == CaffEvent::MODULE_TEX_MANAGER_ACCESS_MSG)
//	{
//		m_textureMgrModule = static_cast<CaffSys::TextureManager*>(data.left.pData);
//	
//		evtMgr->removeController(&m_requestCallBack, CaffEvent::MODULE_TEX_MANAGER_ACCESS_MSG);
//		
//		return CaffEvent::SWALLOW_EVENT;
//	}
//	
//	if(data == CaffEvent::MODULE_CAM_MANAGER_ACCESS_MSG)
//	{
//		m_cameraMgrModule = static_cast<CaffSys::CameraManager*>(data.left.pData);
//	
//		evtMgr->removeController(&m_requestCallBack, CaffEvent::MODULE_CAM_MANAGER_ACCESS_MSG);
//		
//		return CaffEvent::SWALLOW_EVENT;
//	}
//	
//	if(data == CaffEvent::MODULE_RENDERER_ACCESS_MSG)
//	{
//		m_rendererModule = static_cast<CaffApp::Renderer*>(data.left.pData);
//		
//		evtMgr->removeController(&m_requestCallBack, CaffEvent::MODULE_RENDERER_ACCESS_MSG);
//		
//		return CaffEvent::SWALLOW_EVENT;
//	}
//	
//	if(data == CaffEvent::MODULE_TEXT_RENDERER_ACCESS_MSG)
//	{
//		m_textRendererModule = static_cast<CaffSys::TextRenderer*>(data.left.pData);
//		
//		evtMgr->removeController(&m_requestCallBack, CaffEvent::MODULE_TEXT_RENDERER_ACCESS_MSG);
//		
//		return CaffEvent::SWALLOW_EVENT;
//	}
//	
//	if(data == CaffEvent::MODULE_MODEL_MANAGER_ACCESS_MSG)
//	{
//		m_modelMgrModule = static_cast<CaffSys::ModelManager*>(data.left.pData);
//	
//		evtMgr->removeController(&m_requestCallBack, CaffEvent::MODULE_MODEL_MANAGER_ACCESS_MSG);
//		
//		return CaffEvent::SWALLOW_EVENT;
//	}
	
	return false;
}



CaffApp::Renderer & RenderableComponent::getRenderer() const
{
	assert(renderer); // Has the renderer been set?
	return *renderer;
}



CaffSys::TextRenderer & RenderableComponent::getTextRenderer() const
{
	assert(textRenderer); // Has the text renderer been set?
	return *textRenderer;
}



CaffSys::TextureManager & RenderableComponent::getTextureManager() const
{
	assert(textureManager); // Has the texture manager been set?
	return *textureManager;
}



CaffSys::ModelManager & RenderableComponent::getModelManager() const
{
	assert(modelManager); // Has the model manager been set?
	return *modelManager;
}



CaffSys::CameraManager & RenderableComponent::getCameraManager() const
{
	assert(cameraManager); // Has the camera manager been set?
	return *cameraManager;
}



} // namespace
} // namespace
