
#ifndef CAFFEINE_UTILITIES_VARIANT_INCLUDED
#define CAFFEINE_UTILITIES_VARIANT_INCLUDED


#include <Caffeine/Platform.hpp>
#include <type_traits>
#include <memory>
#include <iostream>



namespace Caffeine {
namespace Utilities {


// This is not for the client to use.
namespace VariantDetails
{


template<typename... Ts>
struct TypeInfo;	



template<typename T, typename... Ts>
struct TypeInfo<T, Ts...>
{
	static const bool 		 hasRef = false; /* std::is_reference<T>() || TypeInfo<Ts...>::hasRef; */
	static const std::size_t size 	= TypeInfo<Ts...>::size > sizeof(T) ? TypeInfo<Ts...>::size : sizeof(T);
};



template<>
struct TypeInfo<>
{
	static const bool hasRef = false;
	static const std::size_t size = 0;
};


} // namespace


template<typename... Ts>
class Variant
{

public:
    
    typedef unsigned char Byte;

private:
    
	Byte 		m_data[VariantDetails::TypeInfo<Ts...>::size];
	uint32_t 	m_index;
    
    
public:

	template<typename T>
	Variant(const T& t)
	: m_data()
	, m_index(0)
	{
		static_assert(!VariantDetails::TypeInfo<Ts...>::hasRef, "CaffUtil::Variant: can't contain references.");
		//static_assert(std::is_copy_constructible<T>(), "CaffUtil::Variant: Type is not copy-constructable.");
		
		//std::cout << "ctor" << std::endl;

		//memset(&m_data[0], 0, VariantDetails::TypeInfo<Ts...>::size);
		new(m_data) T(t);
	}


//	Variant(const Variant<Ts...> &other)
//	{
//		assert(&other);
//		//std::cout << "copy" << std::endl;
//
//		memcpy(&m_data[0], &other.m_data[0], VariantDetails::TypeInfo<Ts...>::size);
//	}


	~Variant()
	{
		//std::cout << "dtor'd" << std::endl;
	}


	template<typename T>
	const T& get() const
	{
		return *reinterpret_cast<const T*>(m_data);
	}



	template<typename T>
	T& get()
	{
		return const_cast<T&>(
			static_cast<const Variant<Ts...>& >(*this).get<T>()
		);
	}
    
    
    template<typename T>
    void operator=(const T& other)
    {
        get() = other.get();
    }
    
    
    Byte* data() { return &m_data[0]; }
    Byte const * data() const { return &m_data[0]; }



private:

}; // class


} // namespace
} // namespace


#endif // include guard
