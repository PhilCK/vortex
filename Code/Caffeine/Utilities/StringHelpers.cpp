
#include <Caffeine/Utilities/StringHelpers.hpp>
#include <algorithm>
#include <glm/glm.hpp>


namespace Caffeine {
namespace Utilities {


//template<>
//std::string ToString<void *>(const void* &toStr)
//{
//	std::stringstream ss;
//	ss << toStr;
//	return ss.str();
//}

/*
template<>
std::string ToString<uint8_t>(const uint8_t &toStr)
{
	return std::to_string((int)toStr);
}



template<>
std::string ToString<glm::vec3>(const glm::vec3 &vec)
{
	return ToString(vec.x) + ", " + ToString(vec.y) + ", " + ToString(vec.z);
}
*/


int StringToInt(const std::string &str)
{
	return std::stoi(str);
}

} // namespace
} // namespace
