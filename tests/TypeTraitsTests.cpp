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

			template <typename T>
			struct custom_allocator : allocator<T>
			{	};
			
			template <typename T>
			void unused(T /*v*/)
			{	}
		}

		begin_test_suite( TypeTraitsTests )
			test( NonContainerTypesAreDetectedAsSuch )
			{
				// INIT / ACT / ASSERT
				assert_equal(is_arithmetic, category(type_traits<int>::category()));
				assert_equal(is_arithmetic, category(type_traits<char>::category()));
				assert_equal(is_user_type, category(type_traits<A>::category()));
				assert_equal(is_user_type, category(type_traits< pair<A, int> >::category()));
			}


			test( StandardSequentialContainersAreDetectedAsContainers )
			{
				// INIT / ACT / ASSERT
				assert_equal(is_container, category(type_traits< vector<int> >::category()));
				assert_equal(is_container, category(type_traits< vector<A> >::category()));
				assert_equal(is_container, category(type_traits< list<int> >::category()));
				assert_equal(is_container, category(type_traits< list<A> >::category()));
				assert_equal(is_container, category(type_traits< deque<int> >::category()));
				assert_equal(is_container, category(type_traits< deque<A> >::category()));
				assert_equal(is_container, category(type_traits< basic_string<int> >::category()));
				assert_equal(is_container, category(type_traits< basic_string<A> >::category()));

				assert_equal(is_container, category(type_traits< basic_string< int, char_traits<int> > >::category()));
				assert_equal(is_container, category(type_traits< basic_string< A, char_traits<A> > >::category()));

				assert_equal(is_container, category(type_traits< vector< int, custom_allocator<int> > >::category()));
				assert_equal(is_container, category(type_traits< vector< A, custom_allocator<A> > >::category()));
				assert_equal(is_container, category(type_traits< list< int, custom_allocator<int> > >::category()));
				assert_equal(is_container, category(type_traits< list< A, custom_allocator<A> > >::category()));
				assert_equal(is_container, category(type_traits< deque< int, custom_allocator<int> > >::category()));
				assert_equal(is_container, category(type_traits< deque< A, custom_allocator<A> > >::category()));
				assert_equal(is_container, category(type_traits< basic_string< int, char_traits<int>, custom_allocator<int> > >::category()));
				assert_equal(is_container, category(type_traits< basic_string< A, char_traits<A>, custom_allocator<A> > >::category()));
		}


			test( StandardAssociativeSetContainersAreDetectedAsContainers )
			{
				// INIT / ACT / ASSERT
				assert_equal(is_container, category(type_traits< set<int> >::category()));
				assert_equal(is_container, category(type_traits< set<A> >::category()));
				assert_equal(is_container, category(type_traits< multiset<int> >::category()));
				assert_equal(is_container, category(type_traits< multiset<A> >::category()));

				assert_equal(is_container, category(type_traits< set<int, custom_compare> >::category()));
				assert_equal(is_container, category(type_traits< set<A, custom_compare> >::category()));
				assert_equal(is_container, category(type_traits< multiset<int, custom_compare> >::category()));
				assert_equal(is_container, category(type_traits< multiset<A, custom_compare> >::category()));

				assert_equal(is_container, category(type_traits< set<int, custom_compare, custom_allocator<int> > >::category()));
				assert_equal(is_container, category(type_traits< set<A, custom_compare, custom_allocator<A> > >::category()));
				assert_equal(is_container, category(type_traits< multiset<int, custom_compare, custom_allocator<int> > >::category()));
				assert_equal(is_container, category(type_traits< multiset<A, custom_compare, custom_allocator<A> > >::category()));
			}


			test( StandardAssociativeMapContainersAreDetectedAsContainers )
			{
				// INIT / ACT / ASSERT
				assert_equal(is_container, category(type_traits< map<int, int> >::category()));
				assert_equal(is_container, category(type_traits< map<A, int> >::category()));
				assert_equal(is_container, category(type_traits< multimap<int, int> >::category()));
				assert_equal(is_container, category(type_traits< multimap<A, int> >::category()));

				assert_equal(is_container, category(type_traits< map<int, int, custom_compare> >::category()));
				assert_equal(is_container, category(type_traits< map<A, int, custom_compare> >::category()));
				assert_equal(is_container, category(type_traits< multimap<int, int, custom_compare> >::category()));
				assert_equal(is_container, category(type_traits< multimap<A, int, custom_compare> >::category()));

				assert_equal(is_container, category(type_traits< map<int, int, custom_compare, custom_allocator<int> > >::category()));
				assert_equal(is_container, category(type_traits< map<A, int, custom_compare, custom_allocator<A> > >::category()));
				assert_equal(is_container, category(type_traits< multimap<int, int, custom_compare, custom_allocator<int> > >::category()));
				assert_equal(is_container, category(type_traits< multimap<A, int, custom_compare, custom_allocator<A> > >::category()));
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
				
				unused(v1);
				unused(v2);
				unused(v3);
				unused(v4);
				unused(v5);
				unused(v6);
				unused(v7);
				unused(v8);
				unused(v9);
			}

		end_test_suite
	}
}
