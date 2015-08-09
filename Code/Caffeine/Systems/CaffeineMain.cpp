
#include <Caffeine/Systems/CaffeineMain.hpp>
#include <Caffeine/Systems/PropertyManager.hpp>
#include <Caffeine/Systems/CameraManager.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>
#include <Caffeine/Systems/Event.hpp>
#include <Caffeine/Systems/EventManager.hpp>
#include <Caffeine/Systems/ModelManager.hpp>
#include <Caffeine/Systems/PhysicsManager.hpp>
#include <Caffeine/Systems/State.hpp>
#include <Caffeine/Systems/StateSystem.hpp>
#include <Caffeine/Systems/TextureManager.hpp>
#include <Caffeine/Systems/TextRenderer.hpp>
#include <Caffeine/Systems/DebugRenderer.hpp>
#include <Caffeine/Application/Application.hpp>
#include <Caffeine/Application/Window.hpp>
#include <Caffeine/Components/CameraComponent.hpp>
#include <Caffeine/Utilities/Directories.hpp>
#include <Caffeine/Utilities/Logging.hpp>
#include <Caffeine/Utilities/StringHelpers.hpp>



namespace
{
    CaffApp::VertexID skyBuffer         = CaffApp::NO_VERTEX_BUFFER;
    CaffApp::VertexFormatID skyVertFmt  = CaffApp::NO_VERTEX_FORMAT;
    CaffApp::ShaderID skyShd            = CaffApp::NO_SHADER;
    CaffApp::TextureID skyTex           = CaffApp::NO_TEXTURE;
}



namespace Caffeine {
namespace Systems {


CaffeineMain::CaffeineMain()
// Modules
: m_caffApp(nullptr)
, m_cameraMgr(nullptr)
, m_eventMgr(nullptr)
, m_modelMgr(nullptr)
//, m_physicsMgr(nullptr)
, m_stateMgr(nullptr)
, m_textureMgr(nullptr)
, m_entityMgr(nullptr)
, m_textRenderer(nullptr)
{
	// Application params
	const CaffUtil::Param &appParams = CaffSys::Properties::GetApplicationParams();

	// Initialize Logging
	{
		// Log Levels
		const std::string logLevels              = appParams["settings"]["logging"]["level"].asStdString();
		const std::vector<std::string> logTokens = CaffUtil::SplitStringBySpaces(logLevels);

		uint32_t logLevelFlags = 0;

		for(auto &str : logTokens)
		{
			if(CaffUtil::ToLower(str) == "all")
			{
				logLevelFlags = CaffUtil::LogLevel::INFO_LOG | CaffUtil::LogLevel::WARNING_LOG | CaffUtil::LogLevel::ERROR_LOG;
				break;
			}
			else if(CaffUtil::ToLower(str) == "info")
			{
				logLevelFlags |= CaffUtil::LogLevel::INFO_LOG;
			}
			else if(CaffUtil::ToLower(str) == "warning")
			{
				logLevelFlags |= CaffUtil::LogLevel::WARNING_LOG;
			}
			else if(CaffUtil::ToLower(str) == "error")
			{
				logLevelFlags |= CaffUtil::LogLevel::ERROR_LOG;
			}
		}

		CaffUtil::SetLogLevel(logLevelFlags);

		// Log Targets
		const std::string logTargets					= appParams["settings"]["logging"]["outputs"].asStdString();
		const std::vector<std::string> logTargetTokens	= CaffUtil::SplitStringBySpaces(logTargets);

		uint32_t logTargetFlags = 0;

		for(auto &str : logTargetTokens)
		{
            if(CaffUtil::ToLower(str) == "all")
            {
                logTargetFlags = CaffUtil::LogOutput::CONSOLE | CaffUtil::LogOutput::FILE;
                break;
            }
            else if(CaffUtil::ToLower(str) == "console")
            {
                logTargetFlags |= CaffUtil::LogOutput::CONSOLE;
            }
            else if(CaffUtil::ToLower(str) == "file")
            {
                logTargetFlags |= CaffUtil::LogOutput::FILE;
            }
            else if(CaffUtil::ToLower(str) == "buffer")
            {
                logTargetFlags |= CaffUtil::LogOutput::BUFFER;
            }
		}
		

		CaffUtil::SetOutputLogTargets(logTargetFlags);
	}
    
	// Create application.
	m_caffApp.reset(new CaffApp::Application(appParams["settings"]["application"]));
	
	// Load systems
	{
		const CaffUtil::Param &sysParams = appParams["settings"]["systems"];
	
		m_cameraMgr.reset(new CaffSys::CameraManager());
		m_eventMgr.reset(new CaffSys::EventManager());
		
		m_modelMgr.reset(new CaffSys::ModelManager(sysParams["models"], m_caffApp->getRenderer()));
		m_textureMgr.reset(new CaffSys::TextureManager(sysParams["textures"], m_caffApp->getRenderer()));
		m_physicsMgr.reset(new CaffSys::PhysicsManager());
		m_textRenderer.reset(new CaffSys::TextRenderer(m_caffApp->getRenderer(), *m_textureMgr, *m_modelMgr, *m_cameraMgr));
		
		m_entityMgr.reset(new CaffSys::EntityFactory(
										&m_caffApp->getWindow(),
										&m_caffApp->getInput(),
										&m_caffApp->getRenderer(),
										nullptr, // The state will set the event manager.
										m_stateMgr.get(),
										m_cameraMgr.get(),
										m_modelMgr.get(),
										m_textureMgr.get(),
										m_textRenderer.get(),
										nullptr,
										m_physicsMgr.get(),
										appParams["settings"]["systems"]["entities"]));
								
		m_stateMgr.reset(new CaffSys::StateManager(sysParams["states"], m_entityMgr.get()));
		//m_entityMgr->setPhysicsManager(m_physicsMgr.get());
		
		CaffComp::CameraUtils::SetCameraManager(m_cameraMgr.get());
		CaffComp::CameraUtils::SetWindow(&m_caffApp->getWindow());
	}
	
	// Register Input callbacks
	{
		CaffApp::Input &input = m_caffApp->getInput();
	
		input.onButtonUpEvent   = std::bind(&CaffeineMain::onButtonUpCallback,   this, std::placeholders::_1);
		input.onButtonDownEvent = std::bind(&CaffeineMain::onButtonDownCallback, this, std::placeholders::_1);
		input.onTextStreamEvent = std::bind(&CaffeineMain::onTextStreamCallback, this, std::placeholders::_1);
	}

	// Everything else.
	{
		DebugRenderer::Init(&m_caffApp->getRenderer(), m_cameraMgr.get());
	}
	
    // Skybox test
    {
//        const std::array<CaffApp::FormatDesc, 1> fmtDesc = {{ {"inPosition", CaffApp::AttrFormat::FLOAT, 2 } }};
//        skyVertFmt = m_caffApp->getRenderer().addVertexFormat(&fmtDesc[0], 1);
//        
//        GLfloat vertexData[] =
//        {
//           -1,-1,
//            1,-1,
//            1,1,
//            //-1,1,
//        };
//        
//        skyBuffer = m_caffApp->getRenderer().addVertexBuffer(&vertexData[0], sizeof(vertexData), CaffApp::BufferAccess::STATIC);
//        
//        const GLubyte * foo = glGetString(GL_EXTENSIONS);
//        std::cout << foo << std::endl;
//    
//		const std::string filepath = CaffUtil::GetPathDir() + "AppAssets/Shaders/";
//		const std::string filename = "Skybox.shd";
//		
//		skyShd = m_caffApp->getRenderer().addShader(filepath + filename);
//		
//		const std::string cubeFile = CaffSys::Properties::GetApplicationDir() + "Textures/sky.png";
//		
//		skyTex = m_caffApp->getRenderer().addCubeMapTexture(cubeFile, cubeFile, cubeFile, cubeFile, cubeFile, cubeFile);
    }
}



CaffeineMain::~CaffeineMain()
{
	// Unbind callbacks
	{
		m_caffApp->getInput().onButtonDownEvent			= nullptr;
		m_caffApp->getInput().onButtonUpEvent			= nullptr;
		m_caffApp->getInput().onMouseDeltaEvent			= nullptr;
		m_caffApp->getInput().onMouseMovePositionEvent	= nullptr;
	}
}



void CaffeineMain::start()
{
    m_stateMgr->pushState("MainState");
	m_stateMgr->processPending();
	
	while(!m_caffApp->shouldQuit())
	{
		// Start Frame
		m_caffApp->startFrame();
		
		// Delta Time
		const float dt = m_caffApp->getDeltaTime();
	
		// Check hot loaders if re-focused.
		if(m_caffApp->hasGainedFocus())
		{
			//m_entityMgr->checkForUpdates();
			//m_textureMgr->checkForUpdates();
			//m_modelMgr->checkForUpdates();
			
			// Send Msg Gained Focus
			//m_eventMgr->sendQueuedEvent(CaffEvent::APP_GAINED_FOCUS_MSG);
		}
		
		// If app has lost focus we send message.
		if(m_caffApp->hasLostFocus())
		{
			//m_eventMgr->sendQueuedEvent(CaffEvent::APP_LOST_FOCUS_MSG);
		}
		
		// Inserted events.
		{
			//m_insertedEvents->update(dt);
		}
		
		// Send out queued events.
		{
			m_eventMgr->deliverQueuedEvents();
		}
		
		// Update physics.
		{
			m_physicsMgr->updateWorld(dt);
		}
		
		// Update scene.
		{
			CaffApp::Renderer &renderer = m_caffApp->getRenderer();
			
			renderer.reset();
			renderer.setViewPort(m_caffApp->getWindow().getWidth(), m_caffApp->getWindow().getHeight()); // TODO: Sort
			//renderer.setClearColor(0.1f, 0.15f, 0.15f);
			renderer.clearDefault(true, true);
			
			m_stateMgr->topState().updateEntities(dt); // Also renders currently.
		}
		
		// Check state now, we might need to change.
		{
			// If a state change fire a message.
			if(m_stateMgr->processPending())
			{
				//m_eventMgr->sendInstantEvent(CaffEvent::STATE_CHANGE_MSG);
			}
		}
		
		// Render.
		{
			m_textRenderer->renderPendingText();
			//m_renderMgr->renderScene();
			//m_physicsMgr->renderDebug();
		}
		
        
        // Sky Test
//        {
//            glm::mat4 projMat = m_cameraMgr->getCurrentCamera().getProjMatrix();
//            glm::mat4 toCamera = glm::inverse(m_cameraMgr->getCurrentCamera().getViewProjMatrix() * m_cameraMgr->getCurrentCamera().getWorldMatrix());
//            
//            m_caffApp->getRenderer().reset();
//            
//            m_caffApp->getRenderer().setShader(skyShd);
//            m_caffApp->getRenderer().setTexture("skyboxTex", skyTex);
//            m_caffApp->getRenderer().setVertexBuffer(skyBuffer);
//            m_caffApp->getRenderer().setVertexFormat(skyVertFmt);
//            m_caffApp->getRenderer().setShaderMatrix44f("projMat", &projMat[0][0]);
//            m_caffApp->getRenderer().setShaderMatrix44f("worldToCameraMat", &toCamera[0][0]);
//            
//            m_caffApp->getRenderer().apply();
//            
//            glDrawArrays(GL_TRIANGLES, 0, 3);
//        }
        
		
		//m_textRenderer->renderText("[flash]moop[/flash]", &pos[0], *m_cameraMgr);
		
		// End Frame
		m_caffApp->endFrame();
	}

	//m_eventMgr->sendInstantEvent(CaffEvent::APP_SHUTDOWN_MSG);


}



void CaffeineMain::registerComponent(const std::string & factoryName, ComponentFactory *factory)
{
	if(m_entityMgr)
	{
		m_entityMgr->registerComponent(factoryName, factory);
	}
}



void CaffeineMain::onButtonUpCallback(const CaffApp::KeyID key)
{
	assert(m_stateMgr);
	m_stateMgr->topState().onKeyboardChange(CaffApp::KeyState::UP, key);
}



void CaffeineMain::onButtonDownCallback(const CaffApp::KeyID key)
{
	assert(m_stateMgr);
	m_stateMgr->topState().onKeyboardChange(CaffApp::KeyState::DOWN, key);
}



void CaffeineMain::onTextStreamCallback(const std::string &text)
{
	assert(m_stateMgr);
	m_stateMgr->topState().onTextStream(text);
}


} // namespace
} // namespace
