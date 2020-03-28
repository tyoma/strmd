#include <strmd/deserializer.h>

#include <strmd/serializer.h>

#include "helpers.h"

#include <ut/assert.h>
#include <ut/test.h>

using namespace std;

namespace strmd
{
	namespace tests
	{
		namespace mocks
		{
			struct container
			{
				container()
					: prepared_size(0u), prepared_at(static_cast<size_t>(-1)), read_size(0u), complete_at(0u)
				{	}

				size_t prepared_size;
				size_t prepared_at;
				size_t read_size;
				size_t complete_at;
			};

			struct reader
			{
				void prepare(container &c, size_t count)
				{
					c.prepared_size = count;
					c.prepared_at = c.read_size;
				}

				template <typename ArchiveT>
				void read_item(ArchiveT &a, container &c)
				{
					int dummy;
					a(dummy);
					c.read_size++;
				}

				template <typename ArchiveT, typename ContextT>
				void read_item(ArchiveT &a, container &c, ContextT &/*unused*/)
				{
					int dummy;
					a(dummy);
					c.read_size++;
				}

				void complete(container &c)
				{	c.complete_at = c.read_size;	}
			};
		}
	}

	template <> struct type_traits<tests::mocks::container> {
		typedef container_type_tag category;
		typedef tests::mocks::reader item_reader_type;
	};

	namespace tests
	{
		begin_test_suite( ContainerDeserializationTests )
			vector<unsigned char> buffer;

			test( PrepareIsCalledPriorItemsDeserialized )
			{
				// INIT
				vector_writer writer(buffer);
				serializer<vector_writer> s(writer);
				vector_reader reader(buffer);
				deserializer<vector_reader> d(reader);
				mocks::container c;

				s(2u /*count*/), s(1), s(1);
				s(5u /*count*/), s(1), s(1), s(1), s(1), s(1);

				// ACT
				d(c);

				// ASSERT
				assert_equal(0u, c.prepared_at);
				assert_equal(2u, c.prepared_size);

				// INTI
				c.read_size = 0;

				// ACT
				d(c);

				// ASSERT
				assert_equal(0u, c.prepared_at);
				assert_equal(5u, c.prepared_size);
			}


			test( PrepareIsCalledPriorItemsDeserializedContextual )
			{
				// INIT
				vector_writer writer(buffer);
				serializer<vector_writer> s(writer);
				vector_reader reader(buffer);
				deserializer<vector_reader> d(reader);
				mocks::container c;
				int dummy_context;

				s(3u /*count*/), s(1), s(1), s(1);
				s(7u /*count*/), s(1), s(1), s(1), s(1), s(1), s(1), s(1);

				// ACT
				d(c, dummy_context);

				// ASSERT
				assert_equal(0u, c.prepared_at);
				assert_equal(3u, c.prepared_size);

				// INTI
				c.read_size = 0;

				// ACT
				d(c, dummy_context);

				// ASSERT
				assert_equal(0u, c.prepared_at);
				assert_equal(7u, c.prepared_size);
			}


			test( CompleteIsCalledWhenAllItemsAreRead )
			{
				// INIT
				vector_writer writer(buffer);
				serializer<vector_writer> s(writer);
				vector_reader reader(buffer);
				deserializer<vector_reader> d(reader);
				mocks::container c;

				s(3u /*count*/), s(1), s(1), s(1);
				s(1u /*count*/), s(1);

				// ACT
				d(c);

				// ASSERT
				assert_equal(3u, c.complete_at);

				// INIT
				c.read_size = 0;

				// ACT
				d(c);

				// ASSERT
				assert_equal(1u, c.complete_at);
			}


			test( CompleteIsCalledWhenAllItemsAreReadContextual )
			{
				// INIT
				vector_writer writer(buffer);
				serializer<vector_writer> s(writer);
				vector_reader reader(buffer);
				deserializer<vector_reader> d(reader);
				mocks::container c;
				int dummy_context;

				s(3u /*count*/), s(1), s(1), s(1);
				s(2u /*count*/), s(1), s(1);

				// ACT
				d(c, dummy_context);

				// ASSERT
				assert_equal(3u, c.complete_at);

				// INIT
				c.read_size = 0;

				// ACT
				d(c, dummy_context);

				// ASSERT
				assert_equal(2u, c.complete_at);
			}
		end_test_suite
	}
}
