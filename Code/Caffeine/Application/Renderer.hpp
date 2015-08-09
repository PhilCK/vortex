
#ifndef CAFFEINE_APPLICATION_RENDERER_INCLUDED
#define CAFFEINE_APPLICATION_RENDERER_INCLUDED

#include <Caffeine/Platform.hpp>
#include <Caffeine/Utilities/NonCopyable.hpp>
#include <cstddef>
#include <vector>
#include <string>
#include <array>


namespace Caffeine {
namespace Application {

/*
	Renderer
	--------
	This contains all rendering code related to the graphics API.
	Not graphic API code will be located anywhere else.

 */

typedef std::size_t ShaderID;
typedef std::size_t VertexFormatID;
typedef std::size_t VertexID;
typedef std::size_t IndexID;
typedef std::size_t TextureID;


static const ShaderID		NO_SHADER		 = 0;
static const VertexFormatID NO_VERTEX_FORMAT = 0;
static const VertexID		NO_VERTEX_BUFFER = 0;
static const IndexID		NO_INDEX_BUFFER  = 0;
static const TextureID		NO_TEXTURE		 = 0;


static const unsigned int MAX_TEXTURES_UNIT  = 16;
static const unsigned int MAX_SAMPLERS_STATE = 16;


struct Sampler
{
	char *		 name;
	unsigned int index;
};

	
enum class ConstantType
{
	FLOAT,
	VEC2,
	VEC3,
	VEC4,
	INT,
	IVEC2,
	IVEC3,
	IVEC4,
	BOOL,
	BVEC2,
	BVEC3,
	BVEC4,
	MAT2,
	MAT3,
	MAT4,
	
	size,
};




struct Constant
{
	char * name;
	unsigned char * data;
	unsigned int index;
	ConstantType type;
	int nElements;
	bool dirty;
};


struct Shader
{
	GLuint program;
	GLuint vertexShader;
	GLuint geometryShader;
	GLuint fragmentShader;
	
	unsigned int nSamplers;
	unsigned int nConstants;
	
	Constant *constants;
	Sampler  *samplers;
};


struct VertexBuffer
{
	GLuint  vbo;
	GLsizei size;
};


struct IndexBuffer
{
	GLuint		vbo;
	std::size_t indicies;
	std::size_t size;
};


enum class BufferAccess {
	STATIC,
	DEFAULT,
	DYNAMIC,
};


enum class Primitive {
	TRIANGLES,
	TRIANGLE_FAN,
	TRIANGLE_STRIP,
	QUADS,
	LINES,
	LINE_STRIP,
	LINE_LOOP,
	POINTS,
};

// -- DIE -- //
// enum class AttrType {
// 	GENERIC,
// 	VERTEX,
// 	NORMAL,
// 	TANGENT,
// 	BINORMAL,
// 	TEXCOORD,
// };
// -- DIE -- //


enum class AttrFormat {
	FLOAT,
	NIBBLE,
	UBYTE,
};


/*
struct FormatDesc {
	AttrType		type;
	AttrFormat		format;
	unsigned int	size;
};
*/


struct FormatDesc {
	std::string		attrName;
	AttrFormat		type;
	unsigned int	size;
};


/*
struct Attribute {
	AttrFormat		format;
	unsigned int	size;
	unsigned int	offset;
};
*/

struct Attribute {
	unsigned int	index;
	std::string		name;
	unsigned int	size;
	AttrFormat		type;
	std::size_t		pointer;
};


/*
struct VertexFormat {
	Attribute generic[8];
	Attribute texCoord[8];
	Attribute vertex;
	Attribute normal;
	
	unsigned int vertexSize;
	unsigned int maxGeneric;
	unsigned int maxTexCoord;
};
*/

struct VertexFormat {
	Attribute	 attrs[16];
	std::size_t vertexSize;
	std::size_t numberOfAttrs;
	bool		 dirty;
};


struct Texture
{
	unsigned int	width;
	unsigned int	height;
	GLuint			tex;
	GLuint			target;
	unsigned int	flags;
};



#pragma mark - Main Renderer Class

class Renderer : private CaffUtil::NonCopyable
{
	// SDL
	SDL_GLContext							 m_glContext;
	
	GLuint									 m_fboID;
	GLuint									 m_vaoID;

	// Containers
	std::vector<Shader*>					 m_shaders;
	std::vector<VertexFormat*>				 m_vertexFormats;
	std::vector<VertexBuffer*>				 m_vertexBuffers;
	std::vector<IndexBuffer*>				 m_indexBuffers;
	std::vector<Texture*>					 m_textures;

	// Selected items
	ShaderID								 m_selectedShader;
	VertexFormatID							 m_selectedVertexFormat;
	VertexID								 m_selectedVertexBuffer;
	IndexID									 m_selectedIndexBuffer;
	std::array<TextureID, MAX_TEXTURES_UNIT> m_selectedTextures;

	// Settings
	float									 clearColor[4];
	
public:
	
    explicit			Renderer(SDL_Window * sdlWindow);
						~Renderer();
				
				
				
	// *** APPLY INTERNAL STATES *** //
	#pragma mark - Internal States
	
	void				reset();
	void				apply();
	void				setViewPort(const unsigned int width, const unsigned int height);
	void				setClearColor(const float r, const float g, const float b, const float a = 1.0f);
	void				clearDefault(const bool backBuffer = true, const bool depthBuffer = true) const;
	
	
	
	// *** SHADERS *** //
	#pragma mark - Shaders
	
	ShaderID			addShader(const std::string & filename);
	ShaderID			addShader(const std::string & vertexShader, const std::string & geometryShader, const std::string & pixelShader, const std::string & shaderName = "Unknown");
	inline void			setShader(const ShaderID id) { m_selectedShader = id; }
	GLuint				getShader(); // Temp
	
	inline void			setShader1f(const std::string & constant, float const * data)	{ setShaderRaw(constant, data, sizeof(float) * 1); }
	inline void			setShader2f(const std::string & constant, float const * data)	{ setShaderRaw(constant, data, sizeof(float) * 2); }
	inline void			setShader3f(const std::string & constant, float const * data)	{ setShaderRaw(constant, data, sizeof(float) * 3); }
	inline void			setShader4f(const std::string & constant, float const * data)	{ setShaderRaw(constant, data, sizeof(float) * 4); }
	
	void				setShaderMatrix33f(const std::string & constant, const float * data);
	inline void			setShaderMatrix44f(const std::string & constant, const float * data) { setShaderRaw(constant, data, sizeof(float) * 16); }
	
	void				setShaderRaw(const std::string & constant, float const * data, std::size_t size);
	
	
	// *** VERTEX FORMAT *** //
	#pragma mark - Vertex Format
	
	VertexFormatID		addVertexFormat(const FormatDesc* desc, std::size_t attrs);
	inline void			setVertexFormat(const VertexFormatID id) { m_selectedVertexFormat = id; }
	
	
	// *** VERTEX BUFFERS *** //
	#pragma mark - Vertex Buffer
	
	VertexID			addVertexBuffer(const void * data, std::size_t const size, BufferAccess const access);
	inline void			setVertexBuffer(const VertexID id) { m_selectedVertexBuffer = id; }
	void				updateDynamicVertexBuffer(const VertexID id, void * updateData, const unsigned int updateSize);
	
	
	// *** INDEX BUFFERS *** //
	#pragma mark - Index Buffer
	
	IndexID				addIndexBuffer(const void * data, std::size_t const indices, std::size_t const size, BufferAccess const access);
	inline void			setIndexBuffer(IndexID const id) { m_selectedIndexBuffer = id; }
	
	
	// *** TEXTURES *** //
	#pragma mark - Textures
	
	TextureID			addTexture(const std::string & filename);
    TextureID           addCubeMapTexture(const std::string &xPos, const std::string &xNeg, const std::string &yPos, const std::string &yNeg, const std::string &zPos, const std::string &zNeg);
	void				setTexture(const std::string & constant, const TextureID id);
	void				removeTexture(const TextureID id); // TODO: do
	
	
	// *** FRAME BUFFERS *** //
	#pragma mark - Render Targets
	// TODO: Far from done :(
	TextureID			addRenderTarget(const uint16_t width, const uint16_t height, const uint16_t depth);
	void				setRenderTarget(const TextureID id);
	void				removeRenderTarget(const TextureID id); // TODO: do
	
	
	// *** DRAW *** //
	#pragma mark - Drawing
	
	void				drawArray(Primitive const prim, int const firstVert, std::size_t numberOfVerts);
	void				drawElements();

	
    
}; // class
    
} // namespace
} // namespace

#endif // include guard
