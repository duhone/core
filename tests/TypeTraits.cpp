#include <core/TypeTraits.h>

static_assert(CR::Core::is_one_of_v<int, float, int, double> == true, "is_one_of unexpected result");
static_assert(CR::Core::is_one_of_v<int, float, double> == false, "is_one_of unexpected result");
static_assert(CR::Core::is_one_of_v<int, int> == true, "is_one_of unexpected result");
static_assert(CR::Core::is_one_of_v<int, float> == false, "is_one_of unexpected result");
static_assert(CR::Core::is_one_of_v<int> == false, "is_one_of unexpected result");

static_assert(CR::Core::is_unique_v<int, float, int, double> == false, "is_unique_v unexpected result");
static_assert(CR::Core::is_unique_v<int, float, double> == true, "is_unique_v unexpected result");
static_assert(CR::Core::is_unique_v<int, float, float> == false, "is_unique_v unexpected result");
static_assert(CR::Core::is_unique_v<int, double, float, double> == false, "is_unique_v unexpected result");
static_assert(CR::Core::is_unique_v<int, int> == false, "is_unique_v unexpected result");
static_assert(CR::Core::is_unique_v<int, float> == true, "is_unique_v unexpected result");
static_assert(CR::Core::is_unique_v<int> == true, "is_unique_v unexpected result");
