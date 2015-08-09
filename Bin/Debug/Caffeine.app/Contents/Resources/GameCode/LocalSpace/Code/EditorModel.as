
class EditorModel : GameComponent
{

	// ** Component Interface ** //

	void moveCursorUp()
	{

	}



	void moveCursorDown()
	{

	}



	void moveCursorLeft()
	{
	}



	void moveCursorRight()
	{
	}



	void insertText(const string &in str)
	{
		if(m_code.length() > 0)
		{
			if(str == "\n")
			{
				m_code.insertLast("");
				m_cursorLine = m_code.length() - 1;
				m_cursorChar = 0;
			}
			else
			{
				m_code[m_code.length() - 1] += str;
				m_cursorChar += str.length();
			}
		}
		else
		{
			const string newCode = str;
			m_code.insertLast(newCode);
		}
	}



	void backspace()
	{
		const string oldLine = m_code[m_cursorLine];

		Util::LogInfo(Util::ToString(m_cursorChar));

		if(m_cursorChar == oldLine.length() && oldLine.length != 0)
		{
			const string newLine = oldLine.substr(0, oldLine.length() - 1);
			m_code[m_cursorLine] = newLine;

			if(m_cursorChar != 0)
			{
				m_cursorChar = m_cursorChar - 1;
			}
		}

		if(oldLine.length == 0 && m_cursorLine > 0)
		{
			m_code.removeAt(m_cursorLine);
			m_cursorLine -= 1;
			m_cursorChar = m_code[m_cursorLine].length();
		}
	}



	void deleteChar()
	{
	}



	string getCodeOutput()
	{
		string outputStr = "";

		for(uint i = 0; i < m_code.length(); i++)
		{
			outputStr += m_code[i];
			outputStr += "\n";
		}

		return outputStr;
	}



	string getCursorOutput()
	{
		return "";
	}



	// ** Engine Hooks ** //

	private void onParams(const Param &in params) override
	{
	}



	private void onStart() override
	{
		if(m_code.isEmpty())
		{
			m_code.insertLast("");
		}
	}

	

	private void onThink(const float dt) override
	{
	}



	// ** Member Vars ** //

	private string[]	m_code;
	private uint 		m_cursorLine = 0;
	private uint 		m_cursorChar = 0;
	
};
