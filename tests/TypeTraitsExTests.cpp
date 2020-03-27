#include <strmd/container_ex.h>

#include "helpers.h"
#include "types.h"

#include <memory>
#include <ut/assert.h>
#include <ut/test.h>

using namespace std;

namespace strmd
{
	namespace tests
	{
		namespace
		{
			struct custom_compare
			{
				template <typename T>
				bool operator ()(T lhs, T rhs) const;
			};

			struct custom_hash
			{
				template <typename T>
				size_t operator ()(T);
			};

			template <typename T>
			struct custom_allocator : allocator<T>
			{	};
		}

		begin_test_suite( TypeTraitsExTests )
			test( StandardAssociativeSetContainersAreDetectedAsContainers )
			{
				// INIT / ACT / ASSERT
				assert_equal(is_container, category(type_traits< unordered_set<int> >::category()));
				assert_equal(is_container, category(type_traits< unordered_set<A> >::category()));
				assert_equal(is_container, category(type_traits< unordered_multiset<int> >::category()));
				assert_equal(is_container, category(type_traits< unordered_multiset<A> >::category()));

				assert_equal(is_container, category(type_traits< unordered_set<int, custom_hash> >::category()));
				assert_equal(is_container, category(type_traits< unordered_set<A, custom_hash> >::category()));
				assert_equal(is_container, category(type_traits< unordered_multiset<int, custom_hash> >::category()));
				assert_equal(is_container, category(type_traits< unordered_multiset<A, custom_hash> >::category()));

				assert_equal(is_container, category(type_traits< unordered_set<int, custom_hash, custom_compare> >::category()));
				assert_equal(is_container, category(type_traits< unordered_set<A, custom_hash, custom_compare> >::category()));
				assert_equal(is_container, category(type_traits< unordered_multiset<int, custom_hash, custom_compare> >::category()));
				assert_equal(is_container, category(type_traits< unordered_multiset<A, custom_hash, custom_compare> >::category()));

				assert_equal(is_container, category(type_traits< unordered_set<int, custom_hash, custom_compare, custom_allocator<int> > >::category()));
				assert_equal(is_container, category(type_traits< unordered_set<A, custom_hash, custom_compare, custom_allocator<A> > >::category()));
				assert_equal(is_container, category(type_traits< unordered_multiset<int, custom_hash, custom_compare, custom_allocator<int> > >::category()));
				assert_equal(is_container, category(type_traits< unordered_multiset<A, custom_hash, custom_compare, custom_allocator<A> > >::category()));
			}


			test( StandardAssociativeMapContainersAreDetectedAsContainers )
			{
				// INIT / ACT / ASSERT
				assert_equal(is_container, category(type_traits< unordered_map<int, int> >::category()));
				assert_equal(is_container, category(type_traits< unordered_map<A, int> >::category()));
				assert_equal(is_container, category(type_traits< unordered_multimap<int, int> >::category()));
				assert_equal(is_container, category(type_traits< unordered_multimap<A, int> >::category()));

				assert_equal(is_container, category(type_traits< unordered_map<int, int, custom_hash> >::category()));
				assert_equal(is_container, category(type_traits< unordered_map<A, int, custom_hash> >::category()));
				assert_equal(is_container, category(type_traits< unordered_multimap<int, int, custom_hash> >::category()));
				assert_equal(is_container, category(type_traits< unordered_multimap<A, int, custom_hash> >::category()));

				assert_equal(is_container, category(type_traits< unordered_map<int, int, custom_hash, custom_compare> >::category()));
				assert_equal(is_container, category(type_traits< unordered_map<A, int, custom_hash, custom_compare> >::category()));
				assert_equal(is_container, category(type_traits< unordered_multimap<int, int, custom_hash, custom_compare> >::category()));
				assert_equal(is_container, category(type_traits< unordered_multimap<A, int, custom_hash, custom_compare> >::category()));

				assert_equal(is_container, category(type_traits< unordered_map<int, int, custom_hash, custom_compare, custom_allocator<int> > >::category()));
				assert_equal(is_container, category(type_traits< unordered_map<A, int, custom_hash, custom_compare, custom_allocator<A> > >::category()));
				assert_equal(is_container, category(type_traits< unordered_multimap<int, int, custom_hash, custom_compare, custom_allocator<int> > >::category()));
				assert_equal(is_container, category(type_traits< unordered_multimap<A, int, custom_hash, custom_compare, custom_allocator<A> > >::category()));
			}

		end_test_suite
	}
}
