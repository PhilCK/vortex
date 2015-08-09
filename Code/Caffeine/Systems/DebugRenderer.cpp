
#include <Caffeine/Systems/DebugRenderer.hpp>
#include <Caffeine/Systems/CameraManager.hpp>
#include <Caffeine/Components/CameraComponent.hpp>
#include <Caffeine/Application/Renderer.hpp>
#include <Caffeine/Utilities/Directories.hpp>
#include <array>


#ifndef NDEBUG
namespace {

	CaffApp::VertexFormatID	phyDebugVertexFmtID(CaffApp::NO_VERTEX_FORMAT);
	CaffApp::VertexID		phyDebugVertexBufferID(CaffApp::NO_VERTEX_BUFFER);
	CaffApp::ShaderID		phyDebugShaderID(CaffApp::NO_SHADER);
	
	const std::array<CaffApp::FormatDesc, 1> vertFmtDesc = {{
		{"inPosition", CaffApp::AttrFormat::FLOAT, 3},
	}};
	
	std::array<float, 6> lineMesh = {{
		0,0,0,1,1,1,
	}};
	
	CaffApp::Renderer		*renderer(nullptr);
	CaffSys::CameraManager	*camMgr(nullptr);
	
	void InitRenderInfo()
	{
		// Generate the debug info if it hasn't already been.
		{
			if(!phyDebugShaderID) {
				phyDebugShaderID = renderer->addShader(CaffUtil::GetPathDir() + "AppAssets/Shaders/Debug.shd");
			}
			
			if(!phyDebugVertexFmtID) {
				phyDebugVertexFmtID = renderer->addVertexFormat(&vertFmtDesc[0], vertFmtDesc.size());
			}
			
			if(!phyDebugVertexBufferID) {
				phyDebugVertexBufferID = renderer->addVertexBuffer(&lineMesh[0], sizeof(lineMesh), CaffApp::BufferAccess::DYNAMIC);
			}
		}
	}
}
#endif


namespace Caffeine {
namespace Systems {
namespace DebugRenderer {



void Init(CaffApp::Renderer *setRenderer, CaffSys::CameraManager *setCamMgr)
{
#ifndef NDEBUG
	assert(setRenderer);
	renderer = setRenderer;
	
	assert(setCamMgr);
	camMgr = setCamMgr;
	
	InitRenderInfo();
#endif
}



void DrawLine(const glm::vec3 & start, const glm::vec3 & end, const glm::vec3 & color)
{
#ifndef NDEBUG
	CaffComp::CameraComponent &camera = camMgr->getCurrentCamera();

	InitRenderInfo();

	// Update the line mesh with the new start and end points.
	{
		memcpy(&lineMesh[0], &start[0], sizeof(float) * 3);
		memcpy(&lineMesh[3], &end[0],   sizeof(float) * 3);
		
		renderer->updateDynamicVertexBuffer(phyDebugVertexBufferID, &lineMesh, sizeof(lineMesh));
	}

	// Render the line
	{
		const glm::mat4 w;
		const glm::mat4 v = camera.getViewMatrix();
		const glm::mat4 p = camera.getProjMatrix();
		const glm::mat4 wvp = p * v * w;
		
		renderer->reset();
		renderer->setShader(phyDebugShaderID);
		renderer->setVertexFormat(phyDebugVertexFmtID);
		renderer->setVertexBuffer(phyDebugVertexBufferID);
		renderer->setShaderMatrix44f("wvpMat", &wvp[0][0]);
		renderer->setShader4f("color", &color[0]);
		renderer->apply();
		
		glDrawArrays(GL_LINES, 0, 2);
	}
#endif
}



void DrawCube(const glm::vec3 & center, const glm::vec3 & size, const glm::vec3 & color)
{
#ifndef NDEBUG

	const float maxX = center.x + (size.x / 2.0f);
	const float minX = center.x - (size.x / 2.0f);
	const float maxY = center.y + (size.y / 2.0f);
	const float minY = center.y - (size.y / 2.0f);
	const float maxZ = center.z + (size.z / 2.0f);
	const float minZ = center.z - (size.z / 2.0f);

	// All x facing lines
	{
		DrawLine(glm::vec3(minX, minY, minZ), glm::vec3(maxX, minY, minZ), color);
		DrawLine(glm::vec3(minX, maxY, minZ), glm::vec3(maxX, maxY, minZ), color);
		DrawLine(glm::vec3(minX, minY, maxZ), glm::vec3(maxX, minY, maxZ), color);
		DrawLine(glm::vec3(minX, maxY, maxZ), glm::vec3(maxX, maxY, maxZ), color);
	}
	
	// All y faceing lines
	{
		DrawLine(glm::vec3(minX, minY, minZ), glm::vec3(minX, maxY, minZ), color);
		DrawLine(glm::vec3(maxX, minY, minZ), glm::vec3(maxX, maxY, minZ), color);
		DrawLine(glm::vec3(minX, minY, maxZ), glm::vec3(minX, maxY, maxZ), color);
		DrawLine(glm::vec3(maxX, minY, maxZ), glm::vec3(maxX, maxY, maxZ), color);
	}
	
	// All z facing lines
	{
		DrawLine(glm::vec3(minX, minY, minZ), glm::vec3(minX, minY, maxZ), color);
		DrawLine(glm::vec3(maxX, minY, minZ), glm::vec3(maxX, minY, maxZ), color);
		DrawLine(glm::vec3(minX, maxY, minZ), glm::vec3(minX, maxY, maxZ), color);
		DrawLine(glm::vec3(maxX, maxY, minZ), glm::vec3(maxX, maxY, maxZ), color);
	}
#endif
}



} // namespace
} // namespace
} // namespace
