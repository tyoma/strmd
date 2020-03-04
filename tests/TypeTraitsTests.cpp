#include <strmd/type_traits.h>

#include "helpers.h"
#include "types.h"

#include <deque>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <ut/assert.h>
#include <ut/test.h>
#include <vector>

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
				assert_is_false((is_container<int>::value));
				assert_is_false((is_container<char>::value));
				assert_is_false((is_container<A>::value));
				assert_is_false((is_container< pair<A, int> >::value));
			}


			test( StandardSequentialContainersAreDetectedAsContainers )
			{
				// INIT / ACT / ASSERT
				assert_is_true(is_container< vector<int> >::value);
				assert_is_true(is_container< vector<A> >::value);
				assert_is_true(is_container< list<int> >::value);
				assert_is_true(is_container< list<A> >::value);
				assert_is_true(is_container< deque<int> >::value);
				assert_is_true(is_container< deque<A> >::value);
				assert_is_true(is_container< basic_string<int> >::value);
				assert_is_true(is_container< basic_string<A> >::value);

				assert_is_true((is_container< basic_string< int, char_traits<int> > >::value));
				assert_is_true((is_container< basic_string< A, char_traits<A> > >::value));

				assert_is_true((is_container< vector< int, custom_allocator<int> > >::value));
				assert_is_true((is_container< vector< A, custom_allocator<A> > >::value));
				assert_is_true((is_container< list< int, custom_allocator<int> > >::value));
				assert_is_true((is_container< list< A, custom_allocator<A> > >::value));
				assert_is_true((is_container< deque< int, custom_allocator<int> > >::value));
				assert_is_true((is_container< deque< A, custom_allocator<A> > >::value));
				assert_is_true((is_container< basic_string< int, char_traits<int>, custom_allocator<int> > >::value));
				assert_is_true((is_container< basic_string< A, char_traits<A>, custom_allocator<A> > >::value));


				assert_is_false(is_associative< vector<int> >::value);
				assert_is_false(is_associative< vector<A> >::value);
				assert_is_false(is_associative< list<int> >::value);
				assert_is_false(is_associative< list<A> >::value);
				assert_is_false(is_associative< deque<int> >::value);
				assert_is_false(is_associative< deque<A> >::value);
				assert_is_false(is_associative< basic_string<int> >::value);
				assert_is_false(is_associative< basic_string<A> >::value);

				assert_is_false((is_associative< basic_string< int, char_traits<int> > >::value));
				assert_is_false((is_associative< basic_string< A, char_traits<A> > >::value));

				assert_is_false((is_associative< vector< int, custom_allocator<int> > >::value));
				assert_is_false((is_associative< vector< A, custom_allocator<A> > >::value));
				assert_is_false((is_associative< list< int, custom_allocator<int> > >::value));
				assert_is_false((is_associative< list< A, custom_allocator<A> > >::value));
				assert_is_false((is_associative< deque< int, custom_allocator<int> > >::value));
				assert_is_false((is_associative< deque< A, custom_allocator<A> > >::value));
				assert_is_false((is_associative< basic_string< int, char_traits<int>, custom_allocator<int> > >::value));
				assert_is_false((is_associative< basic_string< A, char_traits<A>, custom_allocator<A> > >::value));
		}


			test( StandardAssociativeSetContainersAreDetectedAsContainers )
			{
				// INIT / ACT / ASSERT
				assert_is_true(is_container< set<int> >::value);
				assert_is_true(is_container< set<A> >::value);
				assert_is_true(is_container< multiset<int> >::value);
				assert_is_true(is_container< multiset<A> >::value);
				assert_is_true(is_container< unordered_set<int> >::value);
				assert_is_true(is_container< unordered_set<A> >::value);
				assert_is_true(is_container< unordered_multiset<int> >::value);
				assert_is_true(is_container< unordered_multiset<A> >::value);

				assert_is_true((is_container< set<int, custom_compare> >::value));
				assert_is_true((is_container< set<A, custom_compare> >::value));
				assert_is_true((is_container< multiset<int, custom_compare> >::value));
				assert_is_true((is_container< multiset<A, custom_compare> >::value));
				assert_is_true((is_container< unordered_set<int, custom_hash> >::value));
				assert_is_true((is_container< unordered_set<A, custom_hash> >::value));
				assert_is_true((is_container< unordered_multiset<int, custom_hash> >::value));
				assert_is_true((is_container< unordered_multiset<A, custom_hash> >::value));

				assert_is_true((is_container< set<int, custom_compare, custom_allocator<int> > >::value));
				assert_is_true((is_container< set<A, custom_compare, custom_allocator<A> > >::value));
				assert_is_true((is_container< multiset<int, custom_compare, custom_allocator<int> > >::value));
				assert_is_true((is_container< multiset<A, custom_compare, custom_allocator<A> > >::value));

				assert_is_true((is_container< unordered_set<int, custom_hash, custom_compare> >::value));
				assert_is_true((is_container< unordered_set<A, custom_hash, custom_compare> >::value));
				assert_is_true((is_container< unordered_multiset<int, custom_hash, custom_compare> >::value));
				assert_is_true((is_container< unordered_multiset<A, custom_hash, custom_compare> >::value));

				assert_is_true((is_container< unordered_set<int, custom_hash, custom_compare, custom_allocator<int> > >::value));
				assert_is_true((is_container< unordered_set<A, custom_hash, custom_compare, custom_allocator<A> > >::value));
				assert_is_true((is_container< unordered_multiset<int, custom_hash, custom_compare, custom_allocator<int> > >::value));
				assert_is_true((is_container< unordered_multiset<A, custom_hash, custom_compare, custom_allocator<A> > >::value));


				assert_is_true(is_associative< set<int> >::value);
				assert_is_true(is_associative< set<A> >::value);
				assert_is_true(is_associative< multiset<int> >::value);
				assert_is_true(is_associative< multiset<A> >::value);
				assert_is_true(is_associative< unordered_set<int> >::value);
				assert_is_true(is_associative< unordered_set<A> >::value);
				assert_is_true(is_associative< unordered_multiset<int> >::value);
				assert_is_true(is_associative< unordered_multiset<A> >::value);

				assert_is_true((is_associative< set<int, custom_compare> >::value));
				assert_is_true((is_associative< set<A, custom_compare> >::value));
				assert_is_true((is_associative< multiset<int, custom_compare> >::value));
				assert_is_true((is_associative< multiset<A, custom_compare> >::value));
				assert_is_true((is_associative< unordered_set<int, custom_hash> >::value));
				assert_is_true((is_associative< unordered_set<A, custom_hash> >::value));
				assert_is_true((is_associative< unordered_multiset<int, custom_hash> >::value));
				assert_is_true((is_associative< unordered_multiset<A, custom_hash> >::value));

				assert_is_true((is_associative< set<int, custom_compare, custom_allocator<int> > >::value));
				assert_is_true((is_associative< set<A, custom_compare, custom_allocator<A> > >::value));
				assert_is_true((is_associative< multiset<int, custom_compare, custom_allocator<int> > >::value));
				assert_is_true((is_associative< multiset<A, custom_compare, custom_allocator<A> > >::value));

				assert_is_true((is_associative< unordered_set<int, custom_hash, custom_compare> >::value));
				assert_is_true((is_associative< unordered_set<A, custom_hash, custom_compare> >::value));
				assert_is_true((is_associative< unordered_multiset<int, custom_hash, custom_compare> >::value));
				assert_is_true((is_associative< unordered_multiset<A, custom_hash, custom_compare> >::value));

				assert_is_true((is_associative< unordered_set<int, custom_hash, custom_compare, custom_allocator<int> > >::value));
				assert_is_true((is_associative< unordered_set<A, custom_hash, custom_compare, custom_allocator<A> > >::value));
				assert_is_true((is_associative< unordered_multiset<int, custom_hash, custom_compare, custom_allocator<int> > >::value));
				assert_is_true((is_associative< unordered_multiset<A, custom_hash, custom_compare, custom_allocator<A> > >::value));
			}


			test( StandardAssociativeMapContainersAreDetectedAsContainers )
			{
				// INIT / ACT / ASSERT
				assert_is_true((is_container< map<int, int> >::value));
				assert_is_true((is_container< map<A, int> >::value));
				assert_is_true((is_container< multimap<int, int> >::value));
				assert_is_true((is_container< multimap<A, int> >::value));
				assert_is_true((is_container< unordered_map<int, int> >::value));
				assert_is_true((is_container< unordered_map<A, int> >::value));
				assert_is_true((is_container< unordered_multimap<int, int> >::value));
				assert_is_true((is_container< unordered_multimap<A, int> >::value));

				assert_is_true((is_container< map<int, int, custom_compare> >::value));
				assert_is_true((is_container< map<A, int, custom_compare> >::value));
				assert_is_true((is_container< multimap<int, int, custom_compare> >::value));
				assert_is_true((is_container< multimap<A, int, custom_compare> >::value));
				assert_is_true((is_container< unordered_map<int, int, custom_hash> >::value));
				assert_is_true((is_container< unordered_map<A, int, custom_hash> >::value));
				assert_is_true((is_container< unordered_multimap<int, int, custom_hash> >::value));
				assert_is_true((is_container< unordered_multimap<A, int, custom_hash> >::value));

				assert_is_true((is_container< map<int, int, custom_compare, custom_allocator<int> > >::value));
				assert_is_true((is_container< map<A, int, custom_compare, custom_allocator<A> > >::value));
				assert_is_true((is_container< multimap<int, int, custom_compare, custom_allocator<int> > >::value));
				assert_is_true((is_container< multimap<A, int, custom_compare, custom_allocator<A> > >::value));

				assert_is_true((is_container< unordered_map<int, int, custom_hash, custom_compare> >::value));
				assert_is_true((is_container< unordered_map<A, int, custom_hash, custom_compare> >::value));
				assert_is_true((is_container< unordered_multimap<int, int, custom_hash, custom_compare> >::value));
				assert_is_true((is_container< unordered_multimap<A, int, custom_hash, custom_compare> >::value));

				assert_is_true((is_container< unordered_map<int, int, custom_hash, custom_compare, custom_allocator<int> > >::value));
				assert_is_true((is_container< unordered_map<A, int, custom_hash, custom_compare, custom_allocator<A> > >::value));
				assert_is_true((is_container< unordered_multimap<int, int, custom_hash, custom_compare, custom_allocator<int> > >::value));
				assert_is_true((is_container< unordered_multimap<A, int, custom_hash, custom_compare, custom_allocator<A> > >::value));


				assert_is_true((is_associative< map<int, int> >::value));
				assert_is_true((is_associative< map<A, int> >::value));
				assert_is_true((is_associative< multimap<int, int> >::value));
				assert_is_true((is_associative< multimap<A, int> >::value));
				assert_is_true((is_associative< unordered_map<int, int> >::value));
				assert_is_true((is_associative< unordered_map<A, int> >::value));
				assert_is_true((is_associative< unordered_multimap<int, int> >::value));
				assert_is_true((is_associative< unordered_multimap<A, int> >::value));

				assert_is_true((is_associative< map<int, int, custom_compare> >::value));
				assert_is_true((is_associative< map<A, int, custom_compare> >::value));
				assert_is_true((is_associative< multimap<int, int, custom_compare> >::value));
				assert_is_true((is_associative< multimap<A, int, custom_compare> >::value));
				assert_is_true((is_associative< unordered_map<int, int, custom_hash> >::value));
				assert_is_true((is_associative< unordered_map<A, int, custom_hash> >::value));
				assert_is_true((is_associative< unordered_multimap<int, int, custom_hash> >::value));
				assert_is_true((is_associative< unordered_multimap<A, int, custom_hash> >::value));

				assert_is_true((is_associative< map<int, int, custom_compare, custom_allocator<int> > >::value));
				assert_is_true((is_associative< map<A, int, custom_compare, custom_allocator<A> > >::value));
				assert_is_true((is_associative< multimap<int, int, custom_compare, custom_allocator<int> > >::value));
				assert_is_true((is_associative< multimap<A, int, custom_compare, custom_allocator<A> > >::value));

				assert_is_true((is_associative< unordered_map<int, int, custom_hash, custom_compare> >::value));
				assert_is_true((is_associative< unordered_map<A, int, custom_hash, custom_compare> >::value));
				assert_is_true((is_associative< unordered_multimap<int, int, custom_hash, custom_compare> >::value));
				assert_is_true((is_associative< unordered_multimap<A, int, custom_hash, custom_compare> >::value));

				assert_is_true((is_associative< unordered_map<int, int, custom_hash, custom_compare, custom_allocator<int> > >::value));
				assert_is_true((is_associative< unordered_map<A, int, custom_hash, custom_compare, custom_allocator<A> > >::value));
				assert_is_true((is_associative< unordered_multimap<int, int, custom_hash, custom_compare, custom_allocator<int> > >::value));
				assert_is_true((is_associative< unordered_multimap<A, int, custom_hash, custom_compare, custom_allocator<A> > >::value));
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
