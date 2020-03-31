#include "core/BinaryStream.h"

#include <3rdParty/doctest.h>

using namespace std;
using namespace CR;
using namespace CR::Core;

TEST_CASE("BinaryStream") {
	struct Foo {
		uint32_t a;
		float b;
	};
	vector<byte> stream;

	Write(stream, 4);
	Write(stream, 3.14f);
	Write(stream, 2.02);
	Foo foo;
	foo.a = 537;
	foo.b = 49.024f;
	Write(stream, foo);

	vector<uint32_t> list{8, 54, 38, 52, 87, 3};
	Write(stream, list);

	BinaryReader reader{stream};
	int one = 0;
	Read(reader, one);
	REQUIRE(one == 4);

	float two;
	Read(reader, two);
	REQUIRE(two == doctest::Approx{3.14f});

	double three;
	Read(reader, three);
	REQUIRE(three == doctest::Approx{2.02});

	Foo four;
	Read(reader, four);
	REQUIRE(four.a == 537);
	REQUIRE(four.b == doctest::Approx{49.024f});

	vector<uint32_t> five;
	Read(reader, five);
	REQUIRE(five.size() == 6);
	REQUIRE(five[0] == 8);
	REQUIRE(five[1] == 54);
	REQUIRE(five[2] == 38);
	REQUIRE(five[3] == 52);
	REQUIRE(five[4] == 87);
	REQUIRE(five[5] == 3);
}
