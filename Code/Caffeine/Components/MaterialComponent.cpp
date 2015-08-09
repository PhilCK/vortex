

#include <Caffeine/Components/MaterialComponent.hpp>
#include <Caffeine/Components/CameraComponent.hpp>
#include <Caffeine/Components/TransformComponent.hpp>
#include <Caffeine/Components/CameraComponent.hpp>
#include <Caffeine/Components/LightComponent.hpp>
#include <Caffeine/Systems/CameraManager.hpp>
#include <Caffeine/Systems/TextureManager.hpp>
#include <Caffeine/Entity/Entity.hpp>
#include <Caffeine/Application/Model.hpp>
#include <Caffeine/Utilities/Directories.hpp>
#include <Caffeine/Utilities/Logging.hpp>



namespace Caffeine {
namespace Components {



// Some Material Render info.
namespace
{
	// Fullbright
	CaffApp::ShaderID		fullBrightShaderID	= CaffApp::NO_SHADER;
	CaffApp::VertexFormatID fullBrightVertexID	= CaffApp::NO_VERTEX_FORMAT;

	// Blinn
	CaffApp::ShaderID 		blinnShaderId		= CaffApp::NO_SHADER;
	CaffApp::VertexFormatID blinnVertexId		= CaffApp::NO_VERTEX_FORMAT;
}


FullbrightMaterialComponent::FullbrightMaterialComponent()
: MaterialComponent()
, m_diffuseScale({1.f, 1.f})
, m_rgba({1.f, 1.f, 1.f, 0.f})
, m_textureName()
, m_diffuseMapID(CaffApp::NO_TEXTURE)
{
}


void FullbrightMaterialComponent::onParams(const CaffUtil::Param &params)
{
	// RGB
	{
		const std::array<float, 4> rgba = params["rgba"].asFloat4({m_rgba.r, m_rgba.g, m_rgba.b, m_rgba.a});
		
		enum { R = 0, G, B, A, };
		
		m_rgba.r = rgba.at(R);
		m_rgba.g = rgba.at(G);
		m_rgba.b = rgba.at(B);
		m_rgba.a = rgba.at(A);
	}
	
	// Lookup textureName.
	{
		m_textureName = params["diffuse_map"].asStdString();
	}
	
	// Texture scale
	{
		const std::array<float, 2> uvScale = params["diffuse_map_scale"].asFloat2({m_diffuseScale.x, m_diffuseScale.y});
		
		enum { U = 0, V };
		m_diffuseScale.u = uvScale.at(U);
		m_diffuseScale.v = uvScale.at(V);
	}
}


void FullbrightMaterialComponent::onStart()
{
	// Get texture ID.
	if(!m_textureName.empty())
	{
		CaffSys::TextureData *textureData = getOwner().getTextureManager().getTextureData(m_textureName);
		assert(textureData); // Can't find texture.
		
		m_diffuseMapID = textureData->textureID;
		assert(m_diffuseMapID != CaffApp::NO_TEXTURE);
	}
	
	// Create shader if it doesn't exist.
	if(fullBrightShaderID == CaffApp::NO_SHADER)
	{
		CaffApp::Renderer &renderer = getOwner().getRenderer();
	
		const std::string filepath = CaffUtil::GetPathDir() + "AppAssets/Shaders/";
		const std::string filename = "FullBright2.shd";
	
		fullBrightShaderID = renderer.addShader(filepath + filename);
		assert(fullBrightShaderID); // Failed to load fullbright shader.
	}
	
	// Create vertex format if it doens't exist.
	if(fullBrightVertexID == CaffApp::NO_VERTEX_FORMAT)
	{
		CaffApp::Renderer &renderer = getOwner().getRenderer();
		
		std::array<CaffApp::FormatDesc, 3> desc = {{
			{ "inPosition",	CaffApp::AttrFormat::FLOAT, 3 },
			{ "inTexC",		CaffApp::AttrFormat::FLOAT, 2 },
			{ "inNormal",	CaffApp::AttrFormat::FLOAT, 3 },
		}};
		
		fullBrightVertexID = renderer.addVertexFormat(&desc[0], desc.size());
		assert(fullBrightVertexID); // failed to add vertex format.
	}
}


void FullbrightMaterialComponent::onRender()
{
	if(!hasModelData()) {
		return;
	}
	
	const CameraComponent &cam				= getOwner().getCameraManager().getCurrentCamera();
	//const TransformComponent *camTransform  = cam.getOwner()->getComponent<TransformComponent>();
	TransformComponent *transform			= getOwner().getComponent<TransformComponent>();
	assert(transform);
	
	CaffMath::Matrix44 worldMat = transform->getWorldMatrix();
	glm::mat4 wMat;
	
	memcpy(&wMat[0][0], &worldMat._11, sizeof(float) * 16);
	
//	const glm::mat4 pMat  = cam.getProjMatrix();
//	
//	CaffMath::Matrix44 viewMat = camTransform->getWorldMatrix();
//	glm::mat4 vMat;
//	const glm::mat4 vMat2 = cam.getViewMatrix();
//	
//	memcpy(&vMat[0][0], &viewMat._11, sizeof(float) * 16);
//	
//	//vMat = vMat._inverse();
//	vMat = vMat2;

	const glm::mat4 wvpMat = cam.getViewProjMatrix() * wMat;
	
	CaffApp::Renderer &renderer = getOwner().getRenderer();
	
	renderer.reset();
	renderer.setShader(fullBrightShaderID);
	renderer.setVertexFormat(fullBrightVertexID);
	renderer.setTexture("diffuseTex", m_diffuseMapID);
	renderer.setShader2f("texScale", &m_diffuseScale.x);
	renderer.setShaderMatrix44f("wvpMat", &wvpMat[0][0]);
	renderer.setShader4f("color", &m_rgba.x);
	
	for(std::size_t i = 0; i < getModelData()->model->getNumberOfMeshes(); ++i)
	{
		renderer.setVertexBuffer(getModelData()->vertBufferIDs.at(i));
		
		if(getModelData()->indexBufferIDs.size() < i)
		{
			renderer.setIndexBuffer(getModelData()->indexBufferIDs.at(i));
		}
		
		renderer.apply();
		renderer.drawArray(CaffApp::Primitive::TRIANGLES, 0, getModelData()->model->getMesh(i).getGLFaces());
	}
}

BlinnPhongMaterialComponent::BlinnPhongMaterialComponent()
: MaterialComponent()
, m_diffuseScale({1.f, 1.f})
, m_rgba({1.f, 1.f, 1.f, 1.f})
, m_textureName()
, m_diffuseMapID(CaffApp::NO_TEXTURE)
{}



void BlinnPhongMaterialComponent::onParams(const CaffUtil::Param &params)
{
	// RGB
	{
		const std::array<float, 4> rgba = params["rgba"].asFloat4({m_rgba.r, m_rgba.g, m_rgba.b, m_rgba.a});
		
		enum { R = 0, G, B, A, };
		
		m_rgba.r = rgba.at(R);
		m_rgba.g = rgba.at(G);
		m_rgba.b = rgba.at(B);
		m_rgba.a = rgba.at(A);
	}
	
	// Lookup textureName.
	{
		m_textureName = params["diffuse_map"].asStdString();
	}
	
	// Texture scale
	{
		const std::array<float, 2> uvScale = params["diffuse_map_scale"].asFloat2({m_diffuseScale.x, m_diffuseScale.y});
		
		enum { U = 0, V };
		m_diffuseScale.u = uvScale.at(U);
		m_diffuseScale.v = uvScale.at(V);
	}
}



void BlinnPhongMaterialComponent::onStart()
{
	// Get texture ID.
	if(!m_textureName.empty())
	{
		CaffSys::TextureData *textureData = getOwner().getTextureManager().getTextureData(m_textureName);
		assert(textureData); // Can't find texture.
		
		m_diffuseMapID = textureData->textureID;
		assert(m_diffuseMapID != CaffApp::NO_TEXTURE);
	}

	// Create shader if it doesn't exist.
	if(blinnShaderId == CaffApp::NO_SHADER)
	{
		CaffApp::Renderer &renderer = getOwner().getRenderer();
	
		const std::string filepath = CaffUtil::GetPathDir() + "AppAssets/Shaders/";
		const std::string filename = "Blinn.shd";
	
		blinnShaderId = renderer.addShader(filepath + filename);
		assert(blinnShaderId); // Failed to load fullbright shader.
	}
	
	// Create vertex format if it doens't exist.
	if(blinnVertexId == CaffApp::NO_VERTEX_FORMAT)
	{
		CaffApp::Renderer &renderer = getOwner().getRenderer();
		
		std::array<CaffApp::FormatDesc, 3> desc = {{
			{ "inPosition",	CaffApp::AttrFormat::FLOAT, 3 },
			{ "inTexC",		CaffApp::AttrFormat::FLOAT, 2 },
			{ "inNormal",	CaffApp::AttrFormat::FLOAT, 3 },
		}};
		
		blinnVertexId = renderer.addVertexFormat(&desc[0], desc.size());
		assert(blinnVertexId); // failed to add vertex format.
	}
}



void BlinnPhongMaterialComponent::onRender()
{
	if(!hasModelData()) {
		return;
	}
	
	const CameraComponent &cam    = getOwner().getCameraManager().getCurrentCamera();
	TransformComponent *transform = getOwner().getComponent<TransformComponent>();
	assert(transform);
	
	CaffMath::Matrix44 worldMat = transform->getWorldMatrix();
	glm::mat4 wMat;
	
	memcpy(&wMat[0][0], &worldMat._11, sizeof(float) * 16);

	const glm::mat4 wvpMat = cam.getViewProjMatrix() * wMat;

	const TransformComponent *camTransform = cam.getOwner().getComponent<TransformComponent>();
	assert(camTransform);

	const glm::vec3 camPos = cam.getPosition();

	CaffApp::Renderer &renderer = getOwner().getRenderer();
	
	renderer.reset();
	renderer.setShader(blinnShaderId);
	renderer.setVertexFormat(blinnVertexId);
	renderer.setTexture("diffuseTex", m_diffuseMapID);
	renderer.setShader2f("texScale", &m_diffuseScale.x);
	renderer.setShaderMatrix44f("wMat", &wMat[0][0]);
	renderer.setShaderMatrix44f("wvpMat", &wvpMat[0][0]);
	renderer.setShader3f("camPos", &camPos[0]);
	renderer.setShader4f("color", &m_rgba.x);

	LightUtils::UploadShaderInfo(renderer);
	
	for(std::size_t i = 0; i < getModelData()->model->getNumberOfMeshes(); ++i)
	{
		renderer.setVertexBuffer(getModelData()->vertBufferIDs.at(i));
		
		if(getModelData()->indexBufferIDs.size() < i)
		{
			renderer.setIndexBuffer(getModelData()->indexBufferIDs.at(i));
		}
		
		renderer.apply();
		renderer.drawArray(CaffApp::Primitive::TRIANGLES, 0, getModelData()->model->getMesh(i).getGLFaces());
	}


}


} // namespace
} // namespace
