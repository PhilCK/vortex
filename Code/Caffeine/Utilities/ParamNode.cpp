
#include <Caffeine/Utilities/ParamNode.hpp>
#include <sstream>
#include <iostream>

namespace {

// Converts a vector of strings to an array of floats.
// I indicates how big the array should be.
template<unsigned int I>
void ToFloatArray(std::array<float, I> &result, std::vector<std::string> &tokens)
{
	for(unsigned int i = 0; i < I; ++i)
	{
		if(tokens.size() > i)
		{
			result.at(i) = std::stof(tokens.at(i));
		}
	}
};

} // namespace


namespace Caffeine {
namespace Utilities {


ParamNode::ParamNode()
: m_childNodes()
, m_isRoot(true)
, m_xmlDocument()
, m_parentNode(nullptr)
, m_elementNode(nullptr)
{
}



ParamNode::ParamNode(tinyxml2::XMLElement *parent, const std::string &nodeName)
: m_childNodes()
, m_isRoot(false)
, m_xmlDocument()
, m_parentNode(parent)
, m_elementNode(nullptr)
{
    if(parent)
    {
        
    }
}



bool ParamNode::loadFromFile(const std::string &data)
{
	if(m_isRoot)
	{
		m_xmlDocument.LoadFile(data.c_str());

		return !m_xmlDocument.Error();
	}

	return false;
}



bool ParamNode::loadFromString(const std::string &data)
{
	if(m_isRoot)
	{
		m_xmlDocument.Parse(data.c_str());

		return !m_xmlDocument.Error();
	}

	return false;
}



const ParamNode& ParamNode::getNode(const std::string &nodeName) const
{
	// Check children to see if we already have it.
	if(m_childNodes.count(nodeName))
	{
		return *m_childNodes.at(nodeName).get();
	}

	// Create a new node.
	else
	{
		// If we have xml info find it.
		if(m_isRoot)
		{
			// Yuck! this is to maintain the const for this function, header for more details.
			m_elementNode = const_cast<tinyxml2::XMLDocument*>(&m_xmlDocument)->FirstChildElement(nodeName.c_str());
		}
		else if(m_parentNode)
		{
			m_elementNode = m_parentNode->FirstChildElement(nodeName.c_str());
		}
        
        // If none make a new one.
        if(!m_elementNode)
        {
            // TODO: We want to create new nodes so we can create these on the flies.
            //m_elementNode = m_parentNode->GetDocument()->NewElement(nodeName.c_str()); 
        }

		// Create the new node and add it to children.
		std::unique_ptr<ParamNode> newNode(new ParamNode(m_elementNode, nodeName));
		m_childNodes[nodeName] = std::move(newNode);

		return *m_childNodes.at(nodeName).get();
	}
}



const ParamNode& ParamNode::operator[](const std::string &nodeName) const
{
	return getNode(nodeName);
}



// const ParamNode& ParamNode::getIndex(const unsigned int index) const
// {
// }



// const ParamNode& ParamNode::operator[](const unsigned int index) const
// {
// 	return getIndex(index);
// }

    

void ParamNode::operator=(const unsigned int asInt)
{
    
}
    


bool ParamNode::nodeExists() const
{
	return (m_parentNode ? true : false);
}



float ParamNode::asFloat(const float defaultData) const
{
	if(m_parentNode)
	{
		std::string result = m_parentNode->GetText();
		return std::stof(result);
	}

	return defaultData;
}



Float2Array ParamNode::asFloat2(const Float2Array &defaultData) const
{
	Float2Array result = defaultData;

	if(m_parentNode)
	{
		StringTokens tokens = asStringTokens();

		ToFloatArray<2>(result, tokens);
	}

	return result;
}



Float3Array	ParamNode::asFloat3(const Float3Array &defaultData) const
{
	Float3Array result = defaultData;

	if(m_parentNode)
	{
		StringTokens tokens = asStringTokens();

		ToFloatArray<3>(result, tokens);
	}

	return result;
}



Float4Array	ParamNode::asFloat4(const Float4Array &defaultData) const
{
	Float4Array result = defaultData;

	if(m_parentNode)
	{
		StringTokens tokens = asStringTokens();

		ToFloatArray<4>(result, tokens);
	}

	return result;
}



double ParamNode::asDouble(const double defaultData) const
{
	if(m_parentNode)
	{
		std::string result = m_parentNode->GetText();
		return std::stod(result);
	}

	return defaultData;
}



bool ParamNode::asBool(const bool defaultData) const
{
	if(m_parentNode)
	{
		std::string result = m_parentNode->GetText();
		std::transform(result.begin(), result.end(), result.begin(), ::tolower);

		if(result == "true")
		{
			return true;
		}

		if(result == "false")
		{
			return false;
		}
	}

	return defaultData;
}



int ParamNode::asInt(const int defaultData) const
{
	if(m_parentNode)
	{
		std::string result = m_parentNode->GetText();
		return std::stoi(result);
	}

	return defaultData;
}



unsigned int ParamNode::asUInt(const unsigned int defaultData) const
{
	if(m_parentNode)
	{
		std::string result = m_parentNode->GetText();
		return (unsigned int)std::stol(result);
	}

	return defaultData;
}



std::string ParamNode::asStdString(const std::string &defaultData) const
{
	if(m_parentNode)
	{
		std::string result = m_parentNode->GetText();
		return result;
	}

	return defaultData;
}



StringTokens ParamNode::asStringTokens(const StringTokens &defaultData) const
{
	if(m_parentNode)
	{
		std::string paramData = m_parentNode->GetText();

		std::istringstream ss(paramData);
		std::istream_iterator<std::string> begin(ss), end;

		StringTokens result(begin, end);
		
		return result;
	}

	return defaultData;	
}



UIntTokens ParamNode::asUintTokens(const UIntTokens &defaultData) const
{
	if(m_parentNode)
	{
		std::string paramData = m_parentNode->GetText();

		std::istringstream ss(paramData);
		std::istream_iterator<uint16_t> begin(ss), end;

		UIntTokens result(begin, end);
		
		return result;
	}

	return defaultData;	
}


// As IntTokens

// As Void*


} // namespace
} // namespace
