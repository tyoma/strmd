#include <strmd/type_traits.h>

#include "helpers.h"
#include "types.h"

#include <ut/assert.h>
#include <ut/test.h>

using namespace std;

namespace strmd
{
	namespace tests
	{
		struct custom_hash
		{
			size_t operator ()(int);
		};

		begin_test_suite( TypeTraitsTests )
			test( AllowedContainersAreDetected )
			{
				// INIT / ACT / ASSERT
				assert_is_true((is_container< vector<int> >::value));
				assert_is_true((is_container< vector<A> >::value));
				assert_is_true((is_container<string>::value));
				assert_is_true((is_container< basic_string<B> >::value));
				assert_is_true((is_container< unordered_map<int, B> >::value));
				assert_is_true((is_container< unordered_map<char, A> >::value));
				assert_is_true((is_container< unordered_map<int, B, custom_hash> >::value));
			}


			test( NonContainerTypesAreDetected )
			{
				// INIT / ACT / ASSERT
				assert_is_false((is_container<int>::value));
				assert_is_false((is_container<char>::value));
				assert_is_false((is_container<A>::value));
			}
		end_test_suite
	}
}
