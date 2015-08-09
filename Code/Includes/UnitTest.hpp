/*!
 *	History
 *  September 2013, removed dependencies, added support for different streams.
 *  April 2012, added/created file
 */


/*!
 * 	Issues
 *	- Issue with ASSERT_IS_NEAR() with floats. ie (1.0, 1.2, 0.2) doesn't
 *	  pass as true, when it should.
 *	- Add Greater and Less then checks.
 */


#ifndef TEST_UNIT_TEST_INCLUDED
#define TEST_UNIT_TEST_INCLUDED


#include <list>
#include <string>
#include <math.h>
#include <iostream>


namespace Test {


/*!
 *	Interface class for the tests.
 */
struct ITest
{
	virtual std::string getName() const = 0;
	virtual void run(std::ostream & stream) const = 0;
}; // end of struct




/*!
 *	Unit tests, this is a singleton class, access through instance();
 */
class UnitTest
{
private:

	typedef std::list<ITest*> 	TestList;
	typedef TestList::iterator	TestListIt; // C++11 auto instead?

	//! Some stats on how things have done.
	unsigned int m_passed, m_failed;

	//! Holds all the tests.
	TestList 	  m_tests;

	// Singleton so hiding access
	UnitTest(const UnitTest &);
	UnitTest& operator=(const UnitTest&);

	//! Private ctor.
	explicit UnitTest()
		: m_passed(0)
		, m_failed(0)
		, m_tests()
	{}


public:


	//! Singlton access.
	static UnitTest& instance()
	{
		static UnitTest test;
		return test;
	}


	//! This adds a test to the list.
	void addTest(ITest * newTest) {
		m_tests.push_back(newTest);	
	}


	//! Runs all the tests.
	void runTests(std::ostream & stream)
	{
		// Title screen.
		stream << "\nRunning Unit Tests\n";
		stream << "==================\n";


		// Run the individual tests.
		TestListIt testIt = m_tests.begin();

		for(; testIt != m_tests.end(); ++testIt)
		{
			unsigned int fails(m_failed);

			stream << "\n" << (*testIt)->getName();
			(*testIt)->run(stream);

			if(fails == m_failed) {
				stream << "\nAll test(s) passed!";
			} else {
				stream << "\n" << (m_failed - fails) << " Test(s) failed!";
			}

			stream << "\n--";

		} // for


		// End screen.
		stream << "\n\nResults\n-------\n";
		stream << "Tests Passed: " << m_passed << "\n";
		stream << "Tests Failed: " << m_failed << "\n";

	} // void runTests(...)



	//! Increase the passed tests.
	void passedCurrentTest() { m_passed++; }
	//! Increase the failed tests.
	void failedCurrentTest() { m_failed++; }



}; // end of class




// Run the tests
void RunTests(std::ostream & stream = std::cout)
{
	UnitTest::instance().runTests(stream);
}


} // end of namespace



// *** TEST METHOD *** //


//! Creates a test case.
#define TEST(ClassName)											\
struct Test##ClassName : public Test::ITest						\
{																\
	std::string name; 											\
	Test##ClassName()											\
	: name(#ClassName)											\
	{															\
		Test::UnitTest::instance().addTest(this);				\
	}															\
																\
	std::string getName() const { return name; }				\
																\
	void run(std::ostream & unitTestStream) const;				\
} instanceTest##ClassName;										\
																\
void Test##ClassName::run(std::ostream & unitTestStream) const	\




// *** TEST MACROS *** //


//! Check to see if two values are equal.
#define ASSERT_IS_EQUAL(a, b)																		\
if(a == b) {																						\
	Test::UnitTest::instance().passedCurrentTest();													\
	unitTestStream << "\nPassed! ASSERT_IS_EQUAL";													\
} else {																							\
	Test::UnitTest::instance().failedCurrentTest();													\
	unitTestStream << "\nFailed! ASSERT_IS_EQUAL, with values: " << a << " and " << b;				\
}																									\


//! Check to see if two vales are not equal.
#define ASSERT_IS_NOT_EQUAL(a, b)																		\
if(a != b) {																							\
	Test::UnitTest::instance().passedCurrentTest();														\
	unitTestStream << "\nPassed! ASSERT_IS_NOT_EQUAL";														\
} else {																								\
	Test::UnitTest::instance().failedCurrentTest();														\
	unitTestStream << "\nFailed! ASSERT_IS_NOT_EQUAL, with values: " << a << " and " << b;				\
}																										\


//! Check if two values are inside the error margin.
#define ASSERT_IS_NEAR(a, b, error)																						\
if((fabs(a - b)) <= error) {																							\
	Test::UnitTest::instance().passedCurrentTest();																		\
	unitTestStream << "\nPassed! ASSERT_IS_NEAR";																		\
} else {																												\
	Test::UnitTest::instance().failedCurrentTest();																		\
	unitTestStream << "\nFailed! ASSERT_IS_NEAR, with values: " << a << ", " << b << " and error " << error; 			\
}																														\


//! Check to see if two values are outside the error margin.
#define ASSERT_IS_NOT_NEAR(a, b, error)																									\
if((fabs(a - b)) > error) {																												\
	Test::UnitTest::instance().passedCurrentTest();																						\
	unitTestStream << "\nPassed! ASSERT_IS_NOT_NEAR";																						\
} else {																																\
	Test::UnitTest::instance().failedCurrentTest();																						\
	unitTestStream << "\nFailed! ASSERT_IS_NOT_NEAR, with values: " << a << ", " << b << " and error " << error; 						\
}																																		\


//! Check to see if its true.
#define ASSERT_IS_TRUE(a)														\
if(a) {																			\
	Test::UnitTest::instance().passedCurrentTest();								\
	unitTestStream << "\nPassed! ASSERT_IS_TRUE";								\
} else {																		\
	Test::UnitTest::instance().failedCurrentTest();								\
	unitTestStream << "\nFailed! ASSERT_IS_TRUE, with values: " << a;			\
}																				\


//! Check to see its false.
#define ASSERT_IS_FALSE(a)														\
if(!a) {																		\
	Test::UnitTest::instance().passedCurrentTest();								\
	unitTestStream << "\nPassed! ASSERT_IS_FALSE";								\
} else {																		\
	Test::UnitTest::instance().failedCurrentTest();								\
	unitTestStream << "\nFailed! ASSERT_IS_FALSE, with values: " << a;			\
}																				\



#define ASSERT_IS_GREATER(a, b)																\
if(a > b) {																					\
	Test::UnitTest::instance().passedCurrentTest();											\
	unitTestStream << "\nPassed! ASSERT_IS_GREATER";											\
} else {																					\
	Test::UnitTest::instance().failedCurrentTest();											\
	unitTestStream << "\nFailed! ASSERT_IS_GREATER, with values: " << a << ", " << b;		\
}																							\


#define ASSERT_IS_LESS(a, b)																\
if(a < b) {																					\
	Test::UnitTest::instance().passedCurrentTest();											\
	unitTestStream << "\nPassed! ASSERT_IS_LESS";											\
} else {																					\
	Test::UnitTest::instance().failedCurrentTest();											\
	unitTestStream << "\nFailed! ASSERT_IS_LESS, with values: " << a << ", " << b;			\
}																							\


#endif // include guard
