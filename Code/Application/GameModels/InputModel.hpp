
#ifndef INPUT_MODEL_INCLUDED
#define INPUT_MODEL_INCLUDED


#include <Caffeine/Entity/Component.hpp>


class InputModel : public CaffEnt::Component
{
public:

	enum InputFlags { NAV = 1 << 0, GAME = 1 << 1, };


	// ** Component Interaction ** //
	
	void			setInputType();
	
	
private:


	void			onParams(const CaffUtil::Param &params) override;
	void			onEarlyThink(const float dt) override;


private:

	uint32_t m_inputFlags = 0;
	

}; // class


#endif // include guard
