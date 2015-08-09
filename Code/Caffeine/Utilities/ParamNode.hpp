
#ifndef CAFFEINE_UTILTITIES_PARAM_NODE_INCLUDED
#define CAFFEINE_UTILTITIES_PARAM_NODE_INCLUDED


#include <Caffeine/Platform.hpp>
#include <tinyxml2/tinyxml2.h>
#include <string>
#include <array>
#include <memory>
#include <vector>
#include <map>


namespace Caffeine {
namespace Utilities {

typedef std::array<float, 2> 		Float2Array;
typedef std::array<float, 3> 		Float3Array;
typedef std::array<float, 4> 		Float4Array;
typedef std::vector<std::string> 	StringTokens;
typedef std::vector<uint16_t>		UIntTokens;
typedef std::vector<int16_t>		IntTokens;


/*
	ParamNode
	--
	An abstracted way of dealing with structures.
	Currently we use XML.


	Issues
	--
	No Arrays of elements, no asignment of elements.
*/
class ParamNode
{
public:

	explicit 				ParamNode();

protected:

	// Internal constructor for creating child nodes.
	explicit 				ParamNode(tinyxml2::XMLElement *parent, const std::string &nodeName);

public:

	// Load the param data from file or from a string, both of which must be valid xml.
	// Will only load from root node.
	bool 					loadFromFile(const std::string &data);
	bool 					loadFromString(const std::string &data);

	// Access an element of the tree.
	// settings.getNode("someNode").getNode("otherNode").
	// or using operator[]
	// settings["someNode"]["otherNode"].
	// If there is no node in the xml it will just create an empty node.
	const ParamNode&		getNode(const std::string &nodeName) const;
	const ParamNode&		operator[](const std::string &nodeName) const;

	const ParamNode& 		getIndex(const unsigned int index) const;
	const ParamNode& 		operator[](const unsigned int index) const;

	void 					operator=(const unsigned int asInt);
	void 					operator=(const std::string &asStr);

	// Allows you to check to see if a node exists.
	bool 					nodeExists() const;

	// Cast the data in the xml document as a data type.
	// You can also pass in a default value if no node is found.
	// settings["someNode"].asFloat() // Will return 0.f if no node exists.
	// settings["someNode"].asFloat(640.f) // Will return 640.f if no node exists.
	// The default paramater for all casts is anything that could be considered false. 0, false, "" etc.
	float					asFloat(const float defaultData = 0.f) const;
	Float2Array				asFloat2(const Float2Array &defaultData = Float2Array{{0,0}}) const;
	Float3Array				asFloat3(const Float3Array &defaultData = Float3Array{{0,0,0}}) const;
	Float4Array				asFloat4(const Float4Array &defaultData = Float4Array{{0,0,0,0}}) const;
	double					asDouble(const double defaultData = 0.0) const;
	bool					asBool(const bool defaultData = false) const;
	int						asInt(const int defaultData = 0) const;
	unsigned int 			asUInt(const unsigned int defaultData = 0) const;
	std::string				asStdString(const std::string &defaultData = "") const;
	StringTokens			asStringTokens(const StringTokens &defaultData = StringTokens()) const;
	UIntTokens				asUintTokens(const UIntTokens &defaultData = UIntTokens()) const;
	IntTokens 				asIntTokens(const IntTokens &defaultData = IntTokens()) const;
	void* 					asVoidPtr(const void *ptr = nullptr) const;

private:

	// We use mutable here as we lazy load data, however we want to maintain the const
	// so as the client can't load up new data.
	// We are using mutable to maintain a logical const rather than a bitwise one.

	mutable std::map<std::string, std::unique_ptr<ParamNode> >	m_childNodes;
			bool 					m_isRoot;

			tinyxml2::XMLDocument	m_xmlDocument;
			tinyxml2::XMLElement 	*m_parentNode;
	mutable tinyxml2::XMLElement 	*m_elementNode;

};

} // namespace
} // namespace

#endif // include guard

