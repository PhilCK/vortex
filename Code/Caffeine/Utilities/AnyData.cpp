
#include <Caffeine/Utilities/AnyData.hpp>
#include <Caffeine/Utilities/StringHelpers.hpp>
#include <iostream>


namespace Caffeine {
namespace Utilities {


AnyData::AnyData(const std::string &data)
: m_data(data)
//, m_rawData(new char[8])
{
	//memcpy(m_rawData, &data[0], sizeof(char) * data.length());
}



AnyData::AnyData(const float data)
: AnyData(ToString(data))
{
}



AnyData::AnyData(const double data)
: AnyData(ToString(data))
{
}



AnyData::AnyData(const int32_t data)
: AnyData(ToString(data))
{
}



AnyData::AnyData(const uint32_t data)
: AnyData(ToString(data))
{
}



AnyData::AnyData()
: AnyData("")
{
}



int8_t AnyData::asInt8() const
{
	return (int8_t)std::stoi(m_data);
}



uint8_t AnyData::asUInt8() const
{
	return (uint8_t)std::stoi(m_data);
}



int16_t AnyData::asInt16() const
{
	return (int16_t)std::stoi(m_data);
}



uint16_t AnyData::asUInt16() const
{
	return (uint16_t)std::stoi(m_data);
}



int32_t AnyData::asInt32() const
{
	return (int32_t)std::stol(m_data);
}



uint32_t AnyData::asUInt32() const
{
	return (uint32_t)std::stol(m_data);
}



float AnyData::asFloat() const
{
	return (float)std::stof(m_data);
}



double AnyData::asDouble() const
{
	return (double)std::stod(m_data);
}



std::string AnyData::asStdString() const
{
	return m_data;
}



std::vector<int> AnyData::asIntTokens() const
{
	return ConvertStringToTokens<int>(m_data);
}



std::vector<float> AnyData::asFloatTokens() const
{
	return ConvertStringToTokens<float>(m_data);
}



std::vector<std::string> AnyData::asStringTokens() const
{
	return ConvertStringToTokens<std::string>(m_data);
}



void* AnyData::asVoidPtr() const
{
	//return asUInt16();
	return 0;
}



AnyData& AnyData::operator=(const int8_t data)
{
	m_data = ToString(data);
	
	return *this;
}



AnyData& AnyData::operator=(const uint8_t data)
{
	m_data = ToString(data);
	
	return *this;
}



AnyData& AnyData::operator=(const int16_t data)
{
	m_data = ToString(data);
	
	return *this;
}



AnyData& AnyData::operator=(const uint16_t data)
{
	m_data = ToString(data);
	
	return *this;
}



AnyData& AnyData::operator=(const int32_t data)
{
	m_data = ToString(data);
	
	return *this;
}



AnyData& AnyData::operator=(const uint32_t data)
{
	m_data = ToString(data);
	
	return *this;
}



AnyData& AnyData::operator=(const float data)
{
	m_data = ToString(data);
	
	return *this;
}



AnyData& AnyData::operator=(const double data)
{
	m_data = ToString(data);
	
	return *this;
}



AnyData& AnyData::operator=(const std::string &data)
{
	m_data = data;
	
	return *this;
}



AnyData& AnyData::operator=(const char *data)
{
	m_data = data;
	
	return *this;
}


AnyData& AnyData::operator=(const void* data)
{
	return *this;
}


} // namespace
} // namespace
