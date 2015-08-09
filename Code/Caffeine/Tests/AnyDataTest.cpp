
#include <UnitTest.hpp>
#include <Caffeine/Utilities/AnyData.hpp>


TEST(SimpleUse)
{
	// Ints and String
	{
		const Caffeine::Utilities::AnyData data(123);

		ASSERT_IS_EQUAL(123, data.asInt8())
		ASSERT_IS_EQUAL(123, data.asUInt8())
		ASSERT_IS_EQUAL(123, data.asInt16())
		ASSERT_IS_EQUAL(123, data.asUInt16())
		ASSERT_IS_EQUAL(123, data.asInt32())
		ASSERT_IS_EQUAL(123, data.asUInt32())
		ASSERT_IS_EQUAL("123", data.asStdString())

		ASSERT_IS_TRUE(false) // you need to test extreams of ints etc.
	}

	// Floating Point
	{
		const Caffeine::Utilities::AnyData data1(123.32f);

		ASSERT_IS_EQUAL(123.32f, data1.asFloat())
		ASSERT_IS_EQUAL(123.32, data1.asDouble())

		const Caffeine::Utilities::AnyData data2(-123.32);

		ASSERT_IS_EQUAL(-123.32f, data2.asFloat())
		ASSERT_IS_EQUAL(-123.32, data2.asDouble())		
	}

	// Signed Ints
	{
		const Caffeine::Utilities::AnyData data("-32");

		ASSERT_IS_EQUAL(-32, data.asInt8())
		ASSERT_IS_EQUAL(-32, data.asInt16())
		ASSERT_IS_EQUAL(-32, data.asInt32())
	}
}



TEST(Tokens)
{
	const Caffeine::Utilities::AnyData data("9 8 7 6 5");

	// Int Tokens
	{
		const std::vector<int> tokens = data.asIntTokens();
		ASSERT_IS_EQUAL(5, tokens.size())

		ASSERT_IS_EQUAL(tokens.at(0), 9)
		ASSERT_IS_EQUAL(tokens.at(1), 8)
		ASSERT_IS_EQUAL(tokens.at(2), 7)
		ASSERT_IS_EQUAL(tokens.at(3), 6)
		ASSERT_IS_EQUAL(tokens.at(4), 5)
	}

	// Float Tokens
	{
		const Caffeine::Utilities::AnyData fdata("9.1 8.2 7.3 6.4 5.5");
		const std::vector<float> tokens = fdata.asFloatTokens();
		ASSERT_IS_EQUAL(tokens.at(0), 9.1f)
		ASSERT_IS_EQUAL(tokens.at(1), 8.2f)
		ASSERT_IS_EQUAL(tokens.at(2), 7.3f)
		ASSERT_IS_EQUAL(tokens.at(3), 6.4f)
		ASSERT_IS_EQUAL(tokens.at(4), 5.5f)
	}	

	// String Tokens
	{
		const std::vector<std::string> tokens = data.asStringTokens();
		ASSERT_IS_EQUAL(tokens.at(0), "9")
		ASSERT_IS_EQUAL(tokens.at(1), "8")
		ASSERT_IS_EQUAL(tokens.at(2), "7")
		ASSERT_IS_EQUAL(tokens.at(3), "6")
		ASSERT_IS_EQUAL(tokens.at(4), "5")
	}
}



TEST(Asignments)
{
	Caffeine::Utilities::AnyData data;
	data = 3;
	ASSERT_IS_EQUAL(3, data.asUInt8())

	data = -4;
	ASSERT_IS_EQUAL(-4, data.asInt16())

	data = 65535;
	ASSERT_IS_EQUAL(65535, data.asUInt16())

	data = 3.2f;
	ASSERT_IS_EQUAL(3.2f, data.asFloat())
	ASSERT_IS_EQUAL(3.2, data.asDouble())

	data = std::string("foo");
	ASSERT_IS_EQUAL("foo", data.asStdString())

	data = "bar";
	ASSERT_IS_EQUAL("bar", data.asStdString())	
}



TEST(Pointer)
{
	//float a = 1.23f;
	//float b = 3.23f;

	//Caffeine::Utilities::AnyData data(&a);
	//ASSERT_IS_EQUAL("0", data.asStdString())

	//ASSERT_IS_EQUAL(8, sizeof(long*))
}



int main()
{
	Test::RunTests();

	return 0;
}