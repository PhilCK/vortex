

class EditorView : GameComponent
{
	// ** Component Interface ** //

	void setRenderBuffer(const string &in buffer)
	{
		m_bufferToRender = buffer;
	}



	// ** Engine Hooks ** //

	private void onParams(const Param &in params) override
	{
	}

	

	private void onLateThink(const float dt) override
	{
		const Math::Vec2 pos = {-1.9f, 1.f};
		getOwner().getTextRenderer().render2DScreenText(m_bufferToRender, pos);
	}



	// ** Member Vars ** //
	
	private string m_bufferToRender = "";

};
