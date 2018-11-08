#include "core/Reflection.h"
#include "catch.hpp"
#include "core/Log.h"

ReflectMember(X);
ReflectMember(Y);
ReflectMember(Z);
ReflectFuncion(Dot);
ReflectFuncion(Foo);

using namespace std;
using namespace CR::Core;

class Vector2 {
  public:
	float X;
	float Y;
	float Foo(float arg) { return arg; }
};

class Vector3 {
  public:
	float X;
	float Y;
	float Z;
	float Dot(const Vector3& arg) const { return X * arg.X + Y * arg.Y + Z * arg.Z; }
	float Foo() { return 42.0f; }
};

template<typename T>
bool PrintDotIfAvailable([[maybe_unused]] const T& arg1, [[maybe_unused]] const T& arg2) {
	if constexpr(HasFuncionDot_v<T, T>) {
		Log::Info("Dot: {}\n", arg1.Dot(arg2));
		return true;
	} else {
		Log::Info("No dot function available\n");
		return false;
	}
}

TEST_CASE("Reflection", "") {
	REQUIRE(HasMemberX_v<Vector2>);
	REQUIRE(HasMemberY_v<Vector2>);
	REQUIRE(!HasMemberZ_v<Vector2>);
	REQUIRE(HasMemberX_v<Vector3>);
	REQUIRE(HasMemberY_v<Vector3>);
	REQUIRE(HasMemberZ_v<Vector3>);

	REQUIRE(!HasFuncionDot_v<Vector2, Vector3>);
	REQUIRE(HasFuncionDot_v<Vector3, Vector3>);
	REQUIRE(!HasFuncionFoo_v<Vector2>);          // dont have a foo taking any arguments
	REQUIRE(HasFuncionFoo_v<Vector2, float>);    // do have one taking one float argument
	REQUIRE(HasFuncionFoo_v<Vector3>);
	REQUIRE(!HasFuncionFoo_v<Vector3, float>);

	REQUIRE(PrintDotIfAvailable(Vector3{1.0f, 2.0f, 3.0f}, Vector3{3.0f, 2.0f, 1.0f}));
	REQUIRE(!PrintDotIfAvailable(Vector2{1.0f, 2.0f}, Vector2{3.0f, 2.0f}));
}
