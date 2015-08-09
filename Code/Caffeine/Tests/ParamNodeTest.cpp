
#include <UnitTest.hpp>
#include <Caffeine/Utilities/ParamNode.hpp>

std::string paramData =
				"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
				"<!-- Settings.xml is not read by default, it is part of systems. -->"
				"<param_test>"
				"	<test_data_1>"
				"		<basic_types>"
				"			<type_1>10.3</type_1>"
				"			<type_2>14.2 12.3</type_2>"
				"			<type_3>76.1 93.13 34.9</type_3>"
				"			<type_4>73.5 82.1 13.1 32.5</type_4>"
				"			<type_5>32.544</type_5>"
				"			<type_6>true</type_6>"
				"			<type_7>TRUE</type_7>"
				"			<type_8>TrUe</type_8>"
				"			<type_9>false</type_9>"
				"			<type_10>FALSE</type_10>"
				"			<type_11>fAlSe</type_11>"
				"			<type_12>-327</type_12>"
				"			<type_13>134532</type_13>"
				"			<type_14>Hello World</type_14>"
				"			<type_15>foo bar boo baz burp!</type_15>"
				"		</basic_types>"
				"	</test_data_1>"
				"	<test_data_2>"
				"		<test_limits>"
				"			<type_1>65535</type_1>"
				"			<type_2>32767</type_2>"
				"			<type_3>-32767</type_3>"
				"		</test_limits>"
				"	</test_data_2>"
				"	<test_data_3>"
				"		<break_limits>"
				"			<type_1>65536</type_1>"
				"			<type_2>32768</type_2>"
				"			<type_3>-32768</type_3>"
				"		</break_limits>"
				"	</test_data_3>"
				"</param_test>";

TEST(basicTypes)
{
	// From file
	{
		Caffeine::Utilities::ParamNode params;
		params.loadFromFile("ParamFile.xml");

		// Floating Points
		float a = params["param_test"]["test_data_1"]["basic_types"]["type_1"].asFloat();
		ASSERT_IS_EQUAL(a, 10.3f)

		double b = params["param_test"]["test_data_1"]["basic_types"]["type_5"].asDouble();
		ASSERT_IS_EQUAL(b, 32.544)

		// Booleans
		bool c = params["param_test"]["test_data_1"]["basic_types"]["type_6"].asBool();
		bool d = params["param_test"]["test_data_1"]["basic_types"]["type_7"].asBool();
		bool e = params["param_test"]["test_data_1"]["basic_types"]["type_8"].asBool();
		bool f = params["param_test"]["test_data_1"]["basic_types"]["type_9"].asBool();
		bool g = params["param_test"]["test_data_1"]["basic_types"]["type_10"].asBool();
		bool h = params["param_test"]["test_data_1"]["basic_types"]["type_11"].asBool();
		ASSERT_IS_TRUE(c)
		ASSERT_IS_TRUE(d)
		ASSERT_IS_TRUE(e)
		ASSERT_IS_FALSE(f)
		ASSERT_IS_FALSE(g)
		ASSERT_IS_FALSE(h)

		// Ints
		int i = params["param_test"]["test_data_1"]["basic_types"]["type_12"].asInt();
		ASSERT_IS_EQUAL(i, -327)

		unsigned int j = params["param_test"]["test_data_1"]["basic_types"]["type_13"].asUInt();
		ASSERT_IS_EQUAL(j, 134532)

		// String
		std::string k = params["param_test"]["test_data_1"]["basic_types"]["type_14"].asStdString();
		ASSERT_IS_EQUAL(k, "Hello World")
	}

	// From data
	{
		Caffeine::Utilities::ParamNode params;
		params.loadFromString(paramData);

		// Floating Points
		float a = params["param_test"]["test_data_1"]["basic_types"]["type_1"].asFloat();
		ASSERT_IS_EQUAL(a, 10.3f)

		double b = params["param_test"]["test_data_1"]["basic_types"]["type_5"].asDouble();
		ASSERT_IS_EQUAL(b, 32.544)

		// Booleans
		bool c = params["param_test"]["test_data_1"]["basic_types"]["type_6"].asBool();
		bool d = params["param_test"]["test_data_1"]["basic_types"]["type_7"].asBool();
		bool e = params["param_test"]["test_data_1"]["basic_types"]["type_8"].asBool();
		bool f = params["param_test"]["test_data_1"]["basic_types"]["type_9"].asBool();
		bool g = params["param_test"]["test_data_1"]["basic_types"]["type_10"].asBool();
		bool h = params["param_test"]["test_data_1"]["basic_types"]["type_11"].asBool();
		ASSERT_IS_TRUE(c)
		ASSERT_IS_TRUE(d)
		ASSERT_IS_TRUE(e)
		ASSERT_IS_FALSE(f)
		ASSERT_IS_FALSE(g)
		ASSERT_IS_FALSE(h)

		// Ints
		int i = params["param_test"]["test_data_1"]["basic_types"]["type_12"].asInt();
		ASSERT_IS_EQUAL(i, -327)

		unsigned int j = params["param_test"]["test_data_1"]["basic_types"]["type_13"].asUInt();
		ASSERT_IS_EQUAL(j, 134532)

		// String
		std::string k = params["param_test"]["test_data_1"]["basic_types"]["type_14"].asStdString();
		ASSERT_IS_EQUAL(k, "Hello World")		
	}
}


TEST(vectorTypesAndTokens)
{
	// From file
	{
		Caffeine::Utilities::ParamNode params;
		params.loadFromFile("ParamFile.xml");

		// Vector Type
		{
			std::array<float, 2> vec2 = params["param_test"]["test_data_1"]["basic_types"]["type_2"].asFloat2();
			ASSERT_IS_EQUAL(vec2[0], 14.2f)
			ASSERT_IS_EQUAL(vec2[1], 12.3f)

			std::array<float, 3> vec3 = params["param_test"]["test_data_1"]["basic_types"]["type_3"].asFloat3();
			ASSERT_IS_EQUAL(vec3[0], 76.1f)
			ASSERT_IS_EQUAL(vec3[1], 93.13f)
			ASSERT_IS_EQUAL(vec3[2], 34.9f)

			std::array<float, 4> vec4 = params["param_test"]["test_data_1"]["basic_types"]["type_4"].asFloat4();
			ASSERT_IS_EQUAL(vec4[0], 73.5f)
			ASSERT_IS_EQUAL(vec4[1], 82.1f)
			ASSERT_IS_EQUAL(vec4[2], 13.1f)
			ASSERT_IS_EQUAL(vec4[3], 32.5f)
		}

		// Vector Types over padding.
		{
			std::array<float, 4> vec4 = params["param_test"]["test_data_1"]["basic_types"]["type_2"].asFloat4();
			ASSERT_IS_EQUAL(vec4[0], 14.2f)
			ASSERT_IS_EQUAL(vec4[1], 12.3f)
			ASSERT_IS_EQUAL(vec4[2], 0.f)
			ASSERT_IS_EQUAL(vec4[3], 0.f)

			vec4 = params["param_test"]["test_data_1"]["basic_types"]["type_3"].asFloat4();
			ASSERT_IS_EQUAL(vec4[0], 76.1f)
			ASSERT_IS_EQUAL(vec4[1], 93.13f)
			ASSERT_IS_EQUAL(vec4[2], 34.9f)
			ASSERT_IS_EQUAL(vec4[3], 0.f)
		}


		// Vector Types under padding.
		{
			std::array<float, 2> vec2 = params["param_test"]["test_data_1"]["basic_types"]["type_3"].asFloat2();
			ASSERT_IS_EQUAL(vec2[0], 76.1f)
			ASSERT_IS_EQUAL(vec2[1], 93.13f)

			vec2 = params["param_test"]["test_data_1"]["basic_types"]["type_4"].asFloat2();
			ASSERT_IS_EQUAL(vec2[0], 73.5f)
			ASSERT_IS_EQUAL(vec2[1], 82.1f)		
		}


		// Vector Types defaults
		{
			std::array<float, 2> vec2 = params["none"].asFloat2();
			ASSERT_IS_EQUAL(vec2[0], 0.f)
			ASSERT_IS_EQUAL(vec2[1], 0.f)

			std::array<float, 3> vec3 = params["none"].asFloat3();
			ASSERT_IS_EQUAL(vec3[0], 0.f)
			ASSERT_IS_EQUAL(vec3[1], 0.f)
			ASSERT_IS_EQUAL(vec3[2], 0.f)

			std::array<float, 4> vec4 = params["none"].asFloat4();
			ASSERT_IS_EQUAL(vec4[0], 0.f)
			ASSERT_IS_EQUAL(vec4[1], 0.f)
			ASSERT_IS_EQUAL(vec4[2], 0.f)
			ASSERT_IS_EQUAL(vec4[3], 0.f)
		}

		// Tokens
		{
			std::vector<std::string> tokens = params["param_test"]["test_data_1"]["basic_types"]["type_15"].asTokens();
			ASSERT_IS_EQUAL(tokens[0], "foo")
			ASSERT_IS_EQUAL(tokens[1], "bar")
			ASSERT_IS_EQUAL(tokens[2], "boo")
			ASSERT_IS_EQUAL(tokens[3], "baz")
			ASSERT_IS_EQUAL(tokens[4], "burp!")
		}

		// Token defaults
		{
			std::vector<std::string> tokens = params["none"].asTokens();
			ASSERT_IS_EQUAL(tokens.size(), 0)
		}
	}
}


TEST(defaultsAndNonExistant)
{
	// From file
	{
		Caffeine::Utilities::ParamNode params;
		params.loadFromFile("ParamFile.xml");

		// Defaults
		ASSERT_IS_EQUAL(params["none"].asFloat(10.2f), 10.2f)
		ASSERT_IS_EQUAL(params["none"].asDouble(11.4), 11.4)
		ASSERT_IS_EQUAL(params["none"].asBool(true), true)
		ASSERT_IS_EQUAL(params["none"].asStdString("123"), "123")
		ASSERT_IS_EQUAL(params["none"].asInt(-1234), -1234)
		ASSERT_IS_EQUAL(params["none"].asUInt(12345), 12345)

		// None existants
		ASSERT_IS_EQUAL(params["none"].asFloat(), 0.f);
		ASSERT_IS_EQUAL(params["none"].asDouble(), 0.0);
		ASSERT_IS_EQUAL(params["none"].asBool(), false);
		ASSERT_IS_EQUAL(params["none"].asStdString(), "");
		ASSERT_IS_EQUAL(params["none"].asInt(), 0);
		ASSERT_IS_EQUAL(params["none"].asUInt(), 0);
	}
}


TEST(constCorrectness)
{
	const Caffeine::Utilities::ParamNode params;
	const_cast<Caffeine::Utilities::ParamNode*>(&params)->loadFromFile("ParamFile.xml");

	// Floating Points
	float a = params["param_test"]["test_data_1"]["basic_types"]["type_1"].asFloat();
	ASSERT_IS_EQUAL(a, 10.3f)

	double b = params["param_test"]["test_data_1"]["basic_types"]["type_5"].asDouble();
	ASSERT_IS_EQUAL(b, 32.544)

	// Booleans
	bool c = params["param_test"]["test_data_1"]["basic_types"]["type_6"].asBool();
	bool d = params["param_test"]["test_data_1"]["basic_types"]["type_7"].asBool();
	bool e = params["param_test"]["test_data_1"]["basic_types"]["type_8"].asBool();
	bool f = params["param_test"]["test_data_1"]["basic_types"]["type_9"].asBool();
	bool g = params["param_test"]["test_data_1"]["basic_types"]["type_10"].asBool();
	bool h = params["param_test"]["test_data_1"]["basic_types"]["type_11"].asBool();
	ASSERT_IS_TRUE(c)
	ASSERT_IS_TRUE(d)
	ASSERT_IS_TRUE(e)
	ASSERT_IS_FALSE(f)
	ASSERT_IS_FALSE(g)
	ASSERT_IS_FALSE(h)

	// Ints
	int i = params["param_test"]["test_data_1"]["basic_types"]["type_12"].asInt();
	ASSERT_IS_EQUAL(i, -327)

	unsigned int j = params["param_test"]["test_data_1"]["basic_types"]["type_13"].asUInt();
	ASSERT_IS_EQUAL(j, 134532)

	// String
	std::string k = params["param_test"]["test_data_1"]["basic_types"]["type_14"].asStdString();
	ASSERT_IS_EQUAL(k, "Hello World")

	// Zero
	ASSERT_IS_FALSE(params["none"].asBool())
}


int main()
{
	Test::RunTests();

	return 0;
}