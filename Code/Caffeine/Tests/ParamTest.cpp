
#include <UnitTest.hpp>
#include <Caffeine/Utilities/Param.hpp>
#include <climits>

using namespace Caffeine::Utilities;


TEST(SetupCopyingEtc)
{
	Param param1;
	param1["foo"]["bar"] = 2;
	ASSERT_IS_TRUE(param1.doesMemberExist("foo"))
	ASSERT_IS_EQUAL(param1["foo"]["bar"].asInt32(), 2)

	// Copy
	Param param2(param1);
	ASSERT_IS_EQUAL(param1["foo"]["bar"].asInt32(), 2)

	ASSERT_IS_TRUE(param2.doesMemberExist("foo"))
	ASSERT_IS_EQUAL(param2["foo"]["bar"].asInt32(), 2)

	// Splicing
	Param foo("foo");
	foo["fee"];

	Param bar("bar");
	bar["boo"];
	bar["bee"];

	//std::cout << bar.getDataAsTree() << std::endl;

	foo.addNode(bar);

	ASSERT_IS_TRUE(foo.doesMemberExist("bar"))
	ASSERT_IS_TRUE(foo["bar"].doesMemberExist("boo"))

	foo.getDataAsTree();

	//param1.getDataAsTree();
	//param2.getDataAsTree();
}



TEST(Traversing)
{
	Param params;

	ASSERT_IS_FALSE(params.doesMemberExist("foo"))
	params.getNode("foo");
	ASSERT_IS_TRUE(params.doesMemberExist("foo"))

	ASSERT_IS_FALSE(params["foo"].doesMemberExist("bar"))
	params["foo"]["bar"];
	ASSERT_IS_TRUE(params["foo"].doesMemberExist("bar"))

	params["foo"]["boo"][3];
	ASSERT_IS_EQUAL(params["foo"].numberOfMembers(), 5)

	//params.getDataAsTree();
}


TEST(PushNode)
{
	Param params;
	params["root"];
	ASSERT_IS_EQUAL(params["root"].numberOfMembersOfName("foo"), 0);

	params["root"].push("foo") = "abc";
	ASSERT_IS_EQUAL(params["root"].numberOfMembersOfName("foo"), 1);

	params["root"].push("foo") = "xyz";
	ASSERT_IS_EQUAL(params["root"].numberOfMembersOfName("foo"), 2);

	params["root"].push("foo") = "123";
	params["root"].push("foo") = "234";
	params["root"].push("foo") = "345";

	ASSERT_IS_EQUAL(params["root"].numberOfMembersOfName("foo"), 5);

	//params.getDataAsTree();
}


TEST(AssignmentsAndGets)
{
	Param params;
	params["foo"] = 123;
	ASSERT_IS_EQUAL(params["foo"].asInt32(), 123)
	ASSERT_IS_EQUAL(params["foo"].asUInt32(), 123)
	ASSERT_IS_EQUAL(params["foo"].asInt64(), 123)
	ASSERT_IS_EQUAL(params["foo"].asUInt64(), 123)

	params["foo"] = 123.1f;
	ASSERT_IS_NEAR(params["foo"].asFloat(), 123.1f, 0.01f)
	ASSERT_IS_NEAR(params["foo"].asDouble(), 123.1, 0.01)

	params["bar"] = "woof";
	ASSERT_IS_EQUAL(params["bar"].asStdString(), "woof")

	// VECS

	std::array<float, 2> arr1 = {{1.1f, 2.2f}};
	params["vec2"] = arr1;

	std::array<float, 2> result1 = params["vec2"].asFloat2();
	ASSERT_IS_NEAR(arr1[0], result1[0], 0.01f)
	ASSERT_IS_NEAR(arr1[1], result1[1], 0.01f)


	std::array<float, 3> arr2 = {{1.1f, 2.2f, 3.3f}};
	params["vec3"] = arr2;

	std::array<float, 3> result2 = params["vec3"].asFloat3();
	ASSERT_IS_NEAR(arr2[0], result2[0], 0.01f)
	ASSERT_IS_NEAR(arr2[1], result2[1], 0.01f)
	ASSERT_IS_NEAR(arr2[2], result2[2], 0.01f)


	std::array<float, 4> arr3 = {{1.1f, 2.2f, 3.3f, 4.4f}};
	params["vec4"] = arr3;

	std::array<float, 4> result3 = params["vec4"].asFloat4();
	ASSERT_IS_NEAR(arr3[0], result3[0], 0.01f)
	ASSERT_IS_NEAR(arr3[1], result3[1], 0.01f)
	ASSERT_IS_NEAR(arr3[2], result3[2], 0.01f)
	ASSERT_IS_NEAR(arr3[3], result3[3], 0.01f)


	std::array<float, 2> arr4 = {{1.1f, 2.2f }};
	params["vec4"] = arr4;

	std::array<float, 4> result4 = params["vec4"].asFloat4();
	ASSERT_IS_NEAR(arr4[0], result4[0], 0.01f)
	ASSERT_IS_NEAR(arr4[1], result4[1], 0.01f)
	ASSERT_IS_NEAR(0.0f, result4[2], 0.01f)
	ASSERT_IS_NEAR(0.0f, result4[3], 0.01f)

	std::array<float, 3> result5 = params["vec4"].asFloat3();
	ASSERT_IS_NEAR(arr4[0], result5[0], 0.01f)
	ASSERT_IS_NEAR(arr4[1], result5[1], 0.01f)
	ASSERT_IS_NEAR(0.0f, result5[2], 0.01f)
}



TEST(MaximumIntValues)
{
	Param params;

	params["uint32"] = UINT32_MAX;
	ASSERT_IS_EQUAL(params["uint32"].asUInt32(), UINT32_MAX)

	params["int32"] = INT32_MAX;
	ASSERT_IS_EQUAL(params["int32"].asInt32(), INT32_MAX)

	params["uint64"] = UINT64_MAX;
	ASSERT_IS_EQUAL(params["uint64"].asUInt64(), UINT64_MAX)

	params["int64"] = INT64_MAX;
	ASSERT_IS_EQUAL(params["int64"].asInt64(), INT64_MAX)
}


TEST(XMLLoader)
{
	Param param("DebugGround.xml", Param::FileType::XML);

	//param.getDataAsTree();

	ASSERT_IS_EQUAL(param["entity"]["name"].asStdString(), "DebugGround")
	ASSERT_IS_EQUAL(param["entity"]["tags"].asStdString(), "debug ground")
	ASSERT_IS_EQUAL(param["entity"]["components"]["component"][0]["name"].asStdString(), "Transform")
	ASSERT_IS_EQUAL(param["entity"]["components"]["component"][1]["name"].asStdString(), "Rigidbody")
	ASSERT_IS_EQUAL(param["entity"]["components"]["component"][1]["params"]["mass"].asInt32(), 10)
	ASSERT_IS_EQUAL(param["entity"]["components"]["component"][2]["name"].asStdString(), "GroundModel")
	ASSERT_IS_EQUAL(param["entity"]["components"]["component"][2]["params"]["ground_size"].asInt32(), 10)
	ASSERT_IS_EQUAL(param["entity"]["components"]["component"][3]["name"].asStdString(), "GroundView")
	ASSERT_IS_EQUAL(param["entity"]["components"]["component"][3]["params"]["texture_name"].asStdString(), "dev_orange_grid")
	ASSERT_IS_EQUAL(param["entity"]["components"]["component"][3]["params"]["shader_name"].asStdString(), "fullbright")

	//param.getDataAsTree();
}


TEST(ImplicitCasting)
{
	Param params;

	Param::Float2 float2 = {{1.2f, 3.4f}};
	Param::Float3 float3 = {{1.2f, 3.4f, 5.6f}};
	Param::Float4 float4 = {{1.2f, 3.4f, 5.6f, 7.8f}};

	params["float2"] 		= float2;
	params["float3"]		= float3;
	params["float4"] 		= float4;
	params["bool"]			= true;
	params["double"] 		= 2.23;
	params["float"] 		= 1.3f;
	params["uint32_t"] 		= 234;
	params["int32_t"] 		= -3424;
	params["uint64_t"]		= 12313;
	params["int64_t"] 		= -342;
	params["std_string"] 	= "I am a std::string";

	const Param::Float2 asFloat2 	= params["float2"];
	const Param::Float3 asFloat3 	= params["float3"];
	const Param::Float4 asFloat4 	= params["float4"];
	const bool asBool 				= params["bool"];
	const double asDouble 			= params["double"];
	const float asFloat 			= params["float"];
	const uint32_t asU32 			= params["uint32_t"];
	const int32_t asI32 			= params["int32_t"];
	const uint64_t asU64 			= params["uint64_t"];
	const int64_t asI64 			= params["int64_t"];
	const std::string asStdString 	= params["std_string"];

	enum {X = 0, Y, Z, W};

	ASSERT_IS_EQUAL(asFloat2.at(X), 1.2f);
	ASSERT_IS_EQUAL(asFloat2.at(Y), 3.4f);
	ASSERT_IS_EQUAL(asFloat3.at(Z), 5.6f);
	ASSERT_IS_EQUAL(asFloat4.at(W), 7.8f);

	ASSERT_IS_TRUE(asBool);
	ASSERT_IS_EQUAL(asDouble, 2.23);
	ASSERT_IS_EQUAL(asFloat, 1.3f);
	ASSERT_IS_EQUAL(asU32, 234);
	ASSERT_IS_EQUAL(asI32, -3424);
	ASSERT_IS_EQUAL(asU64, 12313);
	ASSERT_IS_EQUAL(asI64, -342);
	ASSERT_IS_EQUAL(asStdString, "I am a std::string");

	// Stream test, can't really assert this.
	params["stream_test"] = "foo foo stream!";
	std::cout << std::endl << params["stream_test"] << std::endl;
}


int main()
{
	Test::RunTests();

	return 0;
}