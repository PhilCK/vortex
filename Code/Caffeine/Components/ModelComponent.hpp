

#ifndef CAFFEINE_COMPONENTS_MODEL_INCLUDED
#define CAFFEINE_COMPONENTS_MODEL_INCLUDED


#include <Caffeine/Platform.hpp>
#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Application/Model.hpp>
#include <string>


namespace Caffeine {
namespace Components {


class ModelComponent : public CaffEnt::Component
{
public:
    
    COMPONENT_RTTI(ModelComponent)

	explicit			ModelComponent();
	
	void				onParams(const CaffUtil::Param &param) override;
	void				onStart() override;
	
	void				setModel(const std::string &modelName);
    CaffApp::Model*     getModelData();
	
private:

	std::string			m_modelName;
    CaffApp::Model      *m_modelData;

}; // class


} // namespace
} // namespace



#endif // include guard


