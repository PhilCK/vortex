
#ifndef CAFFEINE_SYSTEMS_PROPERTY_MANAGER_INCLUDED
#define CAFFEINE_SYSTEMS_PROPERTY_MANAGER_INCLUDED


#include <Caffeine/Utilities/Param.hpp>


namespace Caffeine {
namespace Systems {


namespace Properties {


const CaffUtil::Param &	GetApplicationParams();
std::string				GetApplicationName();
std::string				GetApplicationDir();


} // namespace


} // namespace
} // namespace


#endif // include guard
