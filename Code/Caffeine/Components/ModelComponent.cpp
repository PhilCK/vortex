

#include <Caffeine/Components/ModelComponent.hpp>
#include <Caffeine/Components/MaterialComponent.hpp>


namespace Caffeine {
namespace Components {


ModelComponent::ModelComponent()
: CaffEnt::Component()
, m_modelName()
, m_modelData(nullptr)
{
}


void ModelComponent::onParams(const CaffUtil::Param &params)
{
	m_modelName = params["model_name"].asStdString();
}


void ModelComponent::onStart()
{
	if(!m_modelName.empty())
	{
		setModel(m_modelName);
	}
}


void ModelComponent::setModel(const std::string &modelName)
{
	MaterialComponent *matComp = getOwner().getComponent<CaffComp::MaterialComponent>();
	
	if(matComp != nullptr)
	{
		matComp->setModelData(m_modelName);
        m_modelData = getOwner().getModelManager().getModelData(m_modelName).model.get();
	}
	else
	{
		CaffUtil::LogWarning("ModelComponent - Can't find a material.");
	}
}

    
CaffApp::Model* ModelComponent::getModelData()
{
    setModel(m_modelName);
    return m_modelData;
}


} // namespace
} // namespace