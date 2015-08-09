

#ifndef CAFFEINE_LIB_ARRAY_INCLUDED
#define CAFFEINE_LIB_ARRAY_INCLUDED


namespace Caffeine {
namespace Lib {

/*
	!!!EXPERIMENTAL - DO NOT USE!!!

	Array
	--
	Replacement array for std::vector and std::array.

	The main goals of this class are ...
	- A common interface between vector and array.
	- Debugging options to be able to see how much 'baggy' memory is being used.
	- Easy integration into memory pools.
	- Defining the properties of the array.
*/
template<typename T>
class Array
{
public:

	explicit 				Array();
							~Array();

	T& 						at(const std::size_t i);
	const T& 				at(const std::size_t i) const;

	T& 						operator[](const std::size_t i);
	const T& 				operator[](const std::size_t i) const;

	T* 						data();
	T* 						data() const;

	void 					insert();
	void 					push_back(T t);

	void 					erase();
	void 					pop_back();

	void 					reserve(const std::size_t i);
	void 					resize(const std::size_t i);
	
	void 					lock_size(const bool lock);
	bool 					can_expand() const;

	inline bool 		empty() const 	{ return size() == 0; }
	inline std::size_t 	length() const 	{ return size(); }
	inline std::size_t 	size() const 	{ return m_currentSize; }

	void 					shrink_to_fit();
	inline std::size_t		capacity() const { return m_currentCapacity; }

	// Iterators
	void 					cbegin();
	void 					cend();
	void					back();
	void 					front();
	void 					begin();
	void 					end();
	void 					rbegin();
	void 					rend();

private:

	T* 				m_data;
	std::size_t 	m_currentSize;
	std::size_t 	m_currentCapacity;

}; // class


} // namepsace
} // namespace 

#endif // include guard
