#include "core/storage_buffer.h"

#include <3rdParty/doctest.h>

#include <memory_resource>

using namespace std;
using namespace CR::Core;

TEST_CASE("storage_buffer regular") {
	storage_buffer<std::byte> buffer;

	REQUIRE(buffer.empty());

	buffer.prepare(4);
	REQUIRE(buffer.empty());
	REQUIRE(buffer.size() == 0);

	buffer[0] = (byte)2;
	buffer[1] = (byte)7;
	buffer[2] = (byte)23;
	buffer[3] = (byte)250;

	buffer.commit(4);

	REQUIRE(!buffer.empty());
	REQUIRE(buffer.size() == 4);

	REQUIRE(buffer[0] == (byte)2);
	REQUIRE(buffer[3] == (byte)250);
}

TEST_CASE("storage_buffer pmr") {
	byte localBuffer[10];
	pmr::monotonic_buffer_resource monotonic(localBuffer, sizeof(localBuffer));
	pmr::polymorphic_allocator<std::byte> allocator(&monotonic);

	storage_buffer<std::byte> buffer(allocator);

	REQUIRE(buffer.empty());

	buffer.prepare(4);
	REQUIRE(buffer.empty());
	REQUIRE(buffer.size() == 0);

	buffer[0] = (byte)2;
	buffer[1] = (byte)7;
	buffer[2] = (byte)23;
	buffer[3] = (byte)250;

	buffer.commit(4);

	REQUIRE(!buffer.empty());
	REQUIRE(buffer.size() == 4);

	REQUIRE(buffer[0] == (byte)2);
	REQUIRE(buffer[3] == (byte)250);
}
