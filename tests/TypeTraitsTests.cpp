#include <strmd/container.h>

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

		begin_test_suite( TypeTraitsTests )
			test( NonContainerTypesAreDetectedAsSuch )
			{
				// INIT / ACT / ASSERT
				assert_is_false((container_traits<int>::is_container));
				assert_is_false((container_traits<char>::is_container));
				assert_is_false((container_traits<A>::is_container));
				assert_is_false((container_traits< pair<A, int> >::is_container));
			}


			test( StandardSequentialContainersAreDetectedAsContainers )
			{
				// INIT / ACT / ASSERT
				assert_is_true(container_traits< vector<int> >::is_container);
				assert_is_true(container_traits< vector<A> >::is_container);
				assert_is_true(container_traits< list<int> >::is_container);
				assert_is_true(container_traits< list<A> >::is_container);
				assert_is_true(container_traits< deque<int> >::is_container);
				assert_is_true(container_traits< deque<A> >::is_container);
				assert_is_true(container_traits< basic_string<int> >::is_container);
				assert_is_true(container_traits< basic_string<A> >::is_container);

				assert_is_true((container_traits< basic_string< int, char_traits<int> > >::is_container));
				assert_is_true((container_traits< basic_string< A, char_traits<A> > >::is_container));

				assert_is_true((container_traits< vector< int, custom_allocator<int> > >::is_container));
				assert_is_true((container_traits< vector< A, custom_allocator<A> > >::is_container));
				assert_is_true((container_traits< list< int, custom_allocator<int> > >::is_container));
				assert_is_true((container_traits< list< A, custom_allocator<A> > >::is_container));
				assert_is_true((container_traits< deque< int, custom_allocator<int> > >::is_container));
				assert_is_true((container_traits< deque< A, custom_allocator<A> > >::is_container));
				assert_is_true((container_traits< basic_string< int, char_traits<int>, custom_allocator<int> > >::is_container));
				assert_is_true((container_traits< basic_string< A, char_traits<A>, custom_allocator<A> > >::is_container));


				//assert_is_false(is_associative< vector<int> >::is_container);
				//assert_is_false(is_associative< vector<A> >::is_container);
				//assert_is_false(is_associative< list<int> >::is_container);
				//assert_is_false(is_associative< list<A> >::is_container);
				//assert_is_false(is_associative< deque<int> >::is_container);
				//assert_is_false(is_associative< deque<A> >::is_container);
				//assert_is_false(is_associative< basic_string<int> >::is_container);
				//assert_is_false(is_associative< basic_string<A> >::is_container);

				//assert_is_false((is_associative< basic_string< int, char_traits<int> > >::is_container));
				//assert_is_false((is_associative< basic_string< A, char_traits<A> > >::is_container));

				//assert_is_false((is_associative< vector< int, custom_allocator<int> > >::is_container));
				//assert_is_false((is_associative< vector< A, custom_allocator<A> > >::is_container));
				//assert_is_false((is_associative< list< int, custom_allocator<int> > >::is_container));
				//assert_is_false((is_associative< list< A, custom_allocator<A> > >::is_container));
				//assert_is_false((is_associative< deque< int, custom_allocator<int> > >::is_container));
				//assert_is_false((is_associative< deque< A, custom_allocator<A> > >::is_container));
				//assert_is_false((is_associative< basic_string< int, char_traits<int>, custom_allocator<int> > >::is_container));
				//assert_is_false((is_associative< basic_string< A, char_traits<A>, custom_allocator<A> > >::is_container));
		}


			test( StandardAssociativeSetContainersAreDetectedAsContainers )
			{
				// INIT / ACT / ASSERT
				assert_is_true(container_traits< set<int> >::is_container);
				assert_is_true(container_traits< set<A> >::is_container);
				assert_is_true(container_traits< multiset<int> >::is_container);
				assert_is_true(container_traits< multiset<A> >::is_container);

				assert_is_true((container_traits< set<int, custom_compare> >::is_container));
				assert_is_true((container_traits< set<A, custom_compare> >::is_container));
				assert_is_true((container_traits< multiset<int, custom_compare> >::is_container));
				assert_is_true((container_traits< multiset<A, custom_compare> >::is_container));

				assert_is_true((container_traits< set<int, custom_compare, custom_allocator<int> > >::is_container));
				assert_is_true((container_traits< set<A, custom_compare, custom_allocator<A> > >::is_container));
				assert_is_true((container_traits< multiset<int, custom_compare, custom_allocator<int> > >::is_container));
				assert_is_true((container_traits< multiset<A, custom_compare, custom_allocator<A> > >::is_container));

				//assert_is_true(is_associative< set<int> >::is_container);
				//assert_is_true(is_associative< set<A> >::is_container);
				//assert_is_true(is_associative< multiset<int> >::is_container);
				//assert_is_true(is_associative< multiset<A> >::is_container);

				//assert_is_true((is_associative< set<int, custom_compare> >::is_container));
				//assert_is_true((is_associative< set<A, custom_compare> >::is_container));
				//assert_is_true((is_associative< multiset<int, custom_compare> >::is_container));
				//assert_is_true((is_associative< multiset<A, custom_compare> >::is_container));

				//assert_is_true((is_associative< set<int, custom_compare, custom_allocator<int> > >::is_container));
				//assert_is_true((is_associative< set<A, custom_compare, custom_allocator<A> > >::is_container));
				//assert_is_true((is_associative< multiset<int, custom_compare, custom_allocator<int> > >::is_container));
				//assert_is_true((is_associative< multiset<A, custom_compare, custom_allocator<A> > >::is_container));
			}


			test( StandardAssociativeMapContainersAreDetectedAsContainers )
			{
				// INIT / ACT / ASSERT
				assert_is_true((container_traits< map<int, int> >::is_container));
				assert_is_true((container_traits< map<A, int> >::is_container));
				assert_is_true((container_traits< multimap<int, int> >::is_container));
				assert_is_true((container_traits< multimap<A, int> >::is_container));

				assert_is_true((container_traits< map<int, int, custom_compare> >::is_container));
				assert_is_true((container_traits< map<A, int, custom_compare> >::is_container));
				assert_is_true((container_traits< multimap<int, int, custom_compare> >::is_container));
				assert_is_true((container_traits< multimap<A, int, custom_compare> >::is_container));

				assert_is_true((container_traits< map<int, int, custom_compare, custom_allocator<int> > >::is_container));
				assert_is_true((container_traits< map<A, int, custom_compare, custom_allocator<A> > >::is_container));
				assert_is_true((container_traits< multimap<int, int, custom_compare, custom_allocator<int> > >::is_container));
				assert_is_true((container_traits< multimap<A, int, custom_compare, custom_allocator<A> > >::is_container));

				//assert_is_true((is_associative< map<int, int> >::is_container));
				//assert_is_true((is_associative< map<A, int> >::is_container));
				//assert_is_true((is_associative< multimap<int, int> >::is_container));
				//assert_is_true((is_associative< multimap<A, int> >::is_container));

				//assert_is_true((is_associative< map<int, int, custom_compare> >::is_container));
				//assert_is_true((is_associative< map<A, int, custom_compare> >::is_container));
				//assert_is_true((is_associative< multimap<int, int, custom_compare> >::is_container));
				//assert_is_true((is_associative< multimap<A, int, custom_compare> >::is_container));

				//assert_is_true((is_associative< map<int, int, custom_compare, custom_allocator<int> > >::is_container));
				//assert_is_true((is_associative< map<A, int, custom_compare, custom_allocator<A> > >::is_container));
				//assert_is_true((is_associative< multimap<int, int, custom_compare, custom_allocator<int> > >::is_container));
				//assert_is_true((is_associative< multimap<A, int, custom_compare, custom_allocator<A> > >::is_container));
			}


			test( ConstRemovalHandlesPair )
			{
				// INIT / ACT / ASSERT (must compile)
				strmd::remove_const<int>::type v1;
				strmd::remove_const<const int>::type v2;
				strmd::remove_const<char>::type v3;
				strmd::remove_const<const char>::type v4;
				strmd::remove_const< pair<int, char> >::type v5;
				strmd::remove_const< pair<int, const char> >::type v6;
				strmd::remove_const< pair<const int, char> >::type v7;
				strmd::remove_const< pair<const int, const string> >::type v8;
				strmd::remove_const< const pair<const string, const int> >::type v9;

				v1 = 1233;
				v2 = 321123;
				v3 = 'e';
				v4 = 'Z';
				v5 = make_pair(12215, 'h');
				v6 = make_pair(1215, 'j');
				v7 = make_pair(1225, 'i');
				v8 = make_pair(115, "kree");
				v9 = make_pair("asdasd", 314159);
			}

		end_test_suite
	}
}
