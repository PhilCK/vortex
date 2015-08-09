
class ShipView : GameComponent
{


	// ** Engine Hooks ** //

	private void onParams(const Param &in params) override
	{
		m_shaderParam 	= params["shader_name"].asString(m_shaderParam);
		m_textureParam 	= params["texture_name"].asString(m_textureParam);
		m_modelParam    = params["model_name"].asString(m_modelParam);
	}



	private void onStart() override
	{
	}



	// ** Member Vars ** //

	private uint 		m_shaderID 			= 0;
	private uint 		m_vertexFmtID 		= 0;
	private uint 		m_vertexBufferID 	= 0;
	private uint 		m_textureID 		= 0;
	private ModelData   @m_modelData;

	private string 		m_shaderParam 		= "FullBright.shd";
	private string 		m_textureParam 		= "dev_grid_orange.png";
	private string 		m_modelParam 		= "ship01.obj";
};
