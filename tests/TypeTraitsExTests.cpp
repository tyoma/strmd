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
		}

		template <typename T>
		struct custom_allocator : allocator<T>
		{	};

		begin_test_suite( TypeTraitsExTests )
			test( StandardAssociativeSetContainersAreDetectedAsContainers )
			{
				// INIT / ACT / ASSERT
				assert_is_true(container_traits< unordered_set<int> >::is_container);
				assert_is_true(container_traits< unordered_set<A> >::is_container);
				assert_is_true(container_traits< unordered_multiset<int> >::is_container);
				assert_is_true(container_traits< unordered_multiset<A> >::is_container);

				assert_is_true((container_traits< unordered_set<int, custom_hash> >::is_container));
				assert_is_true((container_traits< unordered_set<A, custom_hash> >::is_container));
				assert_is_true((container_traits< unordered_multiset<int, custom_hash> >::is_container));
				assert_is_true((container_traits< unordered_multiset<A, custom_hash> >::is_container));

				assert_is_true((container_traits< unordered_set<int, custom_hash, custom_compare> >::is_container));
				assert_is_true((container_traits< unordered_set<A, custom_hash, custom_compare> >::is_container));
				assert_is_true((container_traits< unordered_multiset<int, custom_hash, custom_compare> >::is_container));
				assert_is_true((container_traits< unordered_multiset<A, custom_hash, custom_compare> >::is_container));

				assert_is_true((container_traits< unordered_set<int, custom_hash, custom_compare, custom_allocator<int> > >::is_container));
				assert_is_true((container_traits< unordered_set<A, custom_hash, custom_compare, custom_allocator<A> > >::is_container));
				assert_is_true((container_traits< unordered_multiset<int, custom_hash, custom_compare, custom_allocator<int> > >::is_container));
				assert_is_true((container_traits< unordered_multiset<A, custom_hash, custom_compare, custom_allocator<A> > >::is_container));


				//assert_is_true(is_associative< unordered_set<int> >::value);
				//assert_is_true(is_associative< unordered_set<A> >::value);
				//assert_is_true(is_associative< unordered_multiset<int> >::value);
				//assert_is_true(is_associative< unordered_multiset<A> >::value);

				//assert_is_true((is_associative< unordered_set<int, custom_hash> >::value));
				//assert_is_true((is_associative< unordered_set<A, custom_hash> >::value));
				//assert_is_true((is_associative< unordered_multiset<int, custom_hash> >::value));
				//assert_is_true((is_associative< unordered_multiset<A, custom_hash> >::value));

				//assert_is_true((is_associative< unordered_set<int, custom_hash, custom_compare> >::value));
				//assert_is_true((is_associative< unordered_set<A, custom_hash, custom_compare> >::value));
				//assert_is_true((is_associative< unordered_multiset<int, custom_hash, custom_compare> >::value));
				//assert_is_true((is_associative< unordered_multiset<A, custom_hash, custom_compare> >::value));

				//assert_is_true((is_associative< unordered_set<int, custom_hash, custom_compare, custom_allocator<int> > >::value));
				//assert_is_true((is_associative< unordered_set<A, custom_hash, custom_compare, custom_allocator<A> > >::value));
				//assert_is_true((is_associative< unordered_multiset<int, custom_hash, custom_compare, custom_allocator<int> > >::value));
				//assert_is_true((is_associative< unordered_multiset<A, custom_hash, custom_compare, custom_allocator<A> > >::value));
			}


			test( StandardAssociativeMapContainersAreDetectedAsContainers )
			{
				// INIT / ACT / ASSERT
				assert_is_true((container_traits< unordered_map<int, int> >::is_container));
				assert_is_true((container_traits< unordered_map<A, int> >::is_container));
				assert_is_true((container_traits< unordered_multimap<int, int> >::is_container));
				assert_is_true((container_traits< unordered_multimap<A, int> >::is_container));

				assert_is_true((container_traits< unordered_map<int, int, custom_hash> >::is_container));
				assert_is_true((container_traits< unordered_map<A, int, custom_hash> >::is_container));
				assert_is_true((container_traits< unordered_multimap<int, int, custom_hash> >::is_container));
				assert_is_true((container_traits< unordered_multimap<A, int, custom_hash> >::is_container));

				assert_is_true((container_traits< unordered_map<int, int, custom_hash, custom_compare> >::is_container));
				assert_is_true((container_traits< unordered_map<A, int, custom_hash, custom_compare> >::is_container));
				assert_is_true((container_traits< unordered_multimap<int, int, custom_hash, custom_compare> >::is_container));
				assert_is_true((container_traits< unordered_multimap<A, int, custom_hash, custom_compare> >::is_container));

				assert_is_true((container_traits< unordered_map<int, int, custom_hash, custom_compare, custom_allocator<int> > >::is_container));
				assert_is_true((container_traits< unordered_map<A, int, custom_hash, custom_compare, custom_allocator<A> > >::is_container));
				assert_is_true((container_traits< unordered_multimap<int, int, custom_hash, custom_compare, custom_allocator<int> > >::is_container));
				assert_is_true((container_traits< unordered_multimap<A, int, custom_hash, custom_compare, custom_allocator<A> > >::is_container));


				//assert_is_true((is_associative< unordered_map<int, int> >::value));
				//assert_is_true((is_associative< unordered_map<A, int> >::value));
				//assert_is_true((is_associative< unordered_multimap<int, int> >::value));
				//assert_is_true((is_associative< unordered_multimap<A, int> >::value));

				//assert_is_true((is_associative< unordered_map<int, int, custom_hash> >::value));
				//assert_is_true((is_associative< unordered_map<A, int, custom_hash> >::value));
				//assert_is_true((is_associative< unordered_multimap<int, int, custom_hash> >::value));
				//assert_is_true((is_associative< unordered_multimap<A, int, custom_hash> >::value));

				//assert_is_true((is_associative< unordered_map<int, int, custom_hash, custom_compare> >::value));
				//assert_is_true((is_associative< unordered_map<A, int, custom_hash, custom_compare> >::value));
				//assert_is_true((is_associative< unordered_multimap<int, int, custom_hash, custom_compare> >::value));
				//assert_is_true((is_associative< unordered_multimap<A, int, custom_hash, custom_compare> >::value));

				//assert_is_true((is_associative< unordered_map<int, int, custom_hash, custom_compare, custom_allocator<int> > >::value));
				//assert_is_true((is_associative< unordered_map<A, int, custom_hash, custom_compare, custom_allocator<A> > >::value));
				//assert_is_true((is_associative< unordered_multimap<int, int, custom_hash, custom_compare, custom_allocator<int> > >::value));
				//assert_is_true((is_associative< unordered_multimap<A, int, custom_hash, custom_compare, custom_allocator<A> > >::value));
			}

		end_test_suite
	}
}
