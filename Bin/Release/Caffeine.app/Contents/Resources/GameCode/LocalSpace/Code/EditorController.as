

class EditorController : GameComponent
{
	// ** Component Interface ** //

	string getCode() const
	{
		EditorModel@ editorModel = getOwner().getComponent("EditorModel");

		if(editorModel !is null)
		{
			return editorModel.getCodeOutput();
		}

		return "ERROR";
	}



	// ** Engine Hooks ** //

	private void onParams(const Param &in params) override
	{
	}



	private void onStart() override
	{
		getOwner().getInput().setTextStream(m_editorOpen);
		registerForEvent(getOwner().getEventManager().getEventID("CubeCreated"));

	}

	

	private void onThink(const float dt) override
	{
		EditorView@  editorView  = getOwner().getComponent("EditorView");
		EditorModel@ editorModel = getOwner().getComponent("EditorModel");

		if(editorView !is null && m_editorOpen)
		{
			editorView.setRenderBuffer(editorModel.getCodeOutput());
		}
	}



	private void onKeyboardChange(const KeyState state, const Key key) override
	{
		// Listen for special keys that don't come down the stream.
		if(state == KeyState::DOWN && m_editorOpen)
		{
			EditorModel@ editorModel = getOwner().getComponent("EditorModel");

			// New line
			if(key == Key::KB_RETURN || key == Key::KB_ENTER)
			{
				if(editorModel !is null)
				{
					editorModel.insertText("\n");
				}
			}

			// Backspace and delete
			{
				if(key == Key::KB_BACKSPACE)
				{
					editorModel.backspace();
				}

				if(key == Key::KB_DELETE)
				{
					editorModel.deleteChar();
				}
			}
		}

		// Open close editor
		if(state == KeyState::DOWN && key == Key::KB_TAB)
		{
			m_editorOpen = !m_editorOpen;
			getOwner().getInput().setTextStream(m_editorOpen);
		}
	}



	private void onTextStream(const string &in str) override
	{
		EditorModel@ editorModel = getOwner().getComponent("EditorModel");

		if(editorModel !is null && m_editorOpen)
		{
			editorModel.insertText(str);
		}
	}



	private bool onEvent(const Event &in data) override
	{
		const uint cubeCreateEvent = getOwner().getEventManager().getEventID("CubeCreated");

		if(data.getID() == cubeCreateEvent)
		{
			// Set program in cube
		}

		return false;
	}



	// ** Member Vars ** //

	private bool m_editorOpen = false;
	
};
