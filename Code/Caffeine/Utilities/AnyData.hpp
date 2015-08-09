
#ifndef CAFFEINE_UTILITIES_ANYDATA_INCLUDED
#define CAFFEINE_UTILITIES_ANYDATA_INCLUDED


#include <stdint.h>
#include <vector>
#include <string>
#include <array>


namespace Caffeine {
namespace Utilities {

/*
	AnyData
	--
	- Do I need all the different numbers asignments? Or can I get away with just u32?
	- Most of this can be inline I think.
	- Internal data type needs some optimisation. union might be better.
*/
class AnyData final
{
public:

	explicit					AnyData(const std::string &data);
	explicit					AnyData(const float data);
	explicit					AnyData(const double data);
	explicit					AnyData(const int32_t data);
	explicit					AnyData(const uint32_t data);
	//explicit					AnyData(const void* data);
	explicit					AnyData();

	// Ints
	int8_t						asInt8() const;
	uint8_t						asUInt8() const;
	int16_t						asInt16() const;
	uint16_t 					asUInt16() const;
	int32_t						asInt32() const;
	uint32_t					asUInt32() const;

	// Floating Points
	float 						asFloat() const;
	double 						asDouble() const;

	// String
	std::string 				asStdString() const;

	// Tokens
	std::vector<int> 			asIntTokens() const;
	std::vector<float>			asFloatTokens() const;
	std::vector<std::string>	asStringTokens() const;

	// Pointer
	void * 						asVoidPtr() const;

	// Assignment Ints.
	AnyData& 					operator=(const int8_t data);
	AnyData& 					operator=(const uint8_t data);
	AnyData& 					operator=(const int16_t data);
	AnyData& 					operator=(const uint16_t data);
	AnyData& 					operator=(const int32_t data);
	AnyData& 					operator=(const uint32_t data);

	// Assignment floating points.
	AnyData& 					operator=(const float data);
	AnyData& 					operator=(const double data);

	// Assignment String.
	AnyData&					operator=(const std::string &data);
	AnyData&					operator=(const char *data);

	// Assignment pointer.
	AnyData&					operator=(const void* data);

private:

	std::string 				m_data;

}; // class


} // namespace
} // namespace


#endif // include guard
