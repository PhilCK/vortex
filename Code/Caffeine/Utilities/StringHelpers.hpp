
#ifndef CAFFEINE_UTILITIES_STRING_HELPERS_INCLUDED
#define CAFFEINE_UTILITIES_STRING_HELPERS_INCLUDED

#include <Caffeine/Platform.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <algorithm>

namespace Caffeine {
namespace Utilities {


// Will split a string based on spaces. ie "foo bar baz" would become ["foo", "bar", "baz"]
inline std::vector<std::string> SplitStringBySpaces(const std::string &tokenString)
{
	std::istringstream ss(tokenString);
	std::istream_iterator<std::string> begin(ss), end;
	std::vector<std::string>tokens(begin, end);
	
	return tokens;
}



// Checks to see if a token is contained in a vector.
// TODO: Move this somewhere more general as it works with other stuff other than std::string.
// Also this vector can be T instead and use iterators.
template<typename T>
bool ContainsToken(const T &token, const std::vector<T> &tokens)
{
	for(auto & t : tokens)
	{
		if(t == token)
		{
			return true;
		}
	}
	
	return false;
}


template<typename T>
std::size_t ContainsTokens(const std::vector<T> &leftTokens, const std::vector<T> &rightTokens)
{
	std::size_t tokensContained = 0;
	
	for(auto & t : leftTokens)
	{
		if(ContainsToken(t, rightTokens))
		{
			++tokensContained;
		}
	}
	
	return tokensContained;
}


// Converts a string to a token format.
template<typename T>
std::vector<T> ConvertStringToTokens(const std::string &tokenString)
{
	std::istringstream ss(tokenString);
	std::istream_iterator<T> begin(ss), end;
	std::vector<T>tokens(begin, end);
	
	return tokens;
}


// Converts string items to a vec3.
inline glm::vec3 MakeVec3(const std::string &x, const std::string &y, const std::string &z)
{
	glm::vec3 result(::atof(x.c_str()), ::atof(y.c_str()), ::atof(z.c_str()));
	
	return result;
}



// To string helpers
template<typename T>
std::string ToString(const T &t)
{
	return std::to_string(t);
}



inline std::string PtrToString(const void *toStr)
{
	std::stringstream ss;
	ss << toStr;
	return ss.str();
}



inline std::string ToLower(const std::string &str)
{
	std::string result = str;
	std::transform(result.begin(), result.end(), result.begin(), ::tolower);
	
	return result;
}


inline std::string ToUpper(const std::string &str)
{
	std::string result = str;
	std::transform(result.begin(), result.end(), result.begin(), ::toupper);
	
	return result;
}


int StringToInt(const std::string &str);


} // namespace
} // namespace

#endif // include guard
