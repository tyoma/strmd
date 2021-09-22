#include <strmd/serializer.h>

#include "helpers.h"

#include <ut/assert.h>
#include <ut/test.h>

using namespace std;

namespace
{
	struct A1791
	{
		unsigned a;
		unsigned b;

		mutable vector<int> contexts;
	};

	struct A190111
	{
		A1791 nested;
		unsigned x;

		mutable vector<int> contexts;
	};
}

namespace strmd
{
	template <> struct version<A1791> {	enum {	value = 1791	};	};
	template <> struct version<A190111> {	enum {	value = 190111	};	};

	template <typename ArchiveT>
	void serialize(ArchiveT &a, A1791 &v, unsigned ver)
	{
		assert_equal(1791u, ver);
		a(v.a), a(v.b);
	}

	template <typename ArchiveT>
	void serialize(ArchiveT &a, A190111 &v, unsigned ver)
	{
		assert_equal(190111u, ver);
		a(v.nested), a(v.x);
	}

	template <typename ArchiveT>
	void serialize(ArchiveT &a, A1791 &v, int ctx, unsigned ver)
	{
		assert_equal(1791u, ver);
		a(v.a), a(v.b);
		v.contexts.push_back(ctx);
	}

	template <typename ArchiveT>
	void serialize(ArchiveT &a, A190111 &v, int ctx, unsigned ver)
	{
		assert_equal(190111u, ver);
		a(v.nested), a(v.x);
		v.contexts.push_back(ctx);
	}

	namespace tests
	{
		begin_test_suite( VersionedSerializationTests )
			vector<unsigned char> buffer;
			vector_writer w;

			VersionedSerializationTests()
				: w(buffer)
			{	}


			test( TypeVersionIsWritten )
			{
				// INIT
				unsigned v = 0;
				serializer<vector_writer, direct> ser(w);

				A1791 a1791 = {};
				A190111 a190111 = {};

				// ACT
				ser(a1791);

				// ASSERT
				vector_reader r1(buffer);
				
				direct::unpack(r1, v);
				assert_equal(1791u, v);

				// INIT
				buffer.clear();

				// ACT
				ser(a190111);

				// ASSERT
				vector_reader r2(buffer);
				
				direct::unpack(r2, v);
				assert_equal(190111u, v);
			}


			test( TypeVersionIsWrittenWithContext )
			{
				// INIT
				unsigned v = 0, ctx;
				serializer<vector_writer, direct> ser(w);

				A1791 a1791;
				A190111 a190111;

				// ACT
				ctx = 171913;
				ser(a1791, ctx);

				// ASSERT
				vector_reader r1(buffer);
				
				direct::unpack(r1, v);
				assert_equal(1791u, v);
				assert_equal(2u, a1791.contexts.size());
				assert_equal(171913, a1791.contexts[0]);
				assert_equal(171913, a1791.contexts[1]);

				// INIT
				buffer.clear();

				// ACT
				ctx = 11;
				ser(a190111, ctx);

				// ASSERT
				vector_reader r2(buffer);
				
				direct::unpack(r2, v);
				assert_equal(190111u, v);
				assert_equal(2u, a190111.contexts.size());
				assert_equal(11, a190111.contexts[0]);
				assert_equal(11, a190111.contexts[1]);
			}


			test( ByteSizeOfASerializedValueAndItselfAreWritten )
			{
				// INIT
				unsigned v = 0;
				serializer<vector_writer, varint> ser(w);

				A1791 a = {	};
				A190111 b = { };

				// ACT
				ser(a);

				// ASSERT
				vector_reader r1(buffer);
				
				varint::unpack(r1, v);
				varint::unpack(r1, v);
				assert_equal(2u, v);

				// INIT
				buffer.clear();

				// ACT
				ser(b);

				// ASSERT
				vector_reader r2(buffer);
				
				varint::unpack(r2, v);
				varint::unpack(r2, v);
				assert_equal(6u, v);

				// INIT
				a.a = 0x10, a.b = 0x1000000;
				buffer.clear();

				// ACT
				ser(a);

				// ASSERT
				vector_reader r3(buffer);
				
				varint::unpack(r3, v);
				varint::unpack(r3, v);
				assert_equal(5u, v);
				varint::unpack(r3, v);
				assert_equal(0x10u, v);
				varint::unpack(r3, v);
				assert_equal(0x1000000u, v);

				// INIT
				b.nested.a = 0x80000000, b.nested.b = 0x80000001, b.x = 0x80001000;
				buffer.clear();

				// ACT
				ser(b);

				// ASSERT
				vector_reader r4(buffer);
				
				varint::unpack(r4, v);
				varint::unpack(r4, v);
				assert_equal(18u, v);

				varint::unpack(r4, v);
				varint::unpack(r4, v);
				assert_equal(10u, v);
				varint::unpack(r4, v);
				assert_equal(0x80000000u, v);
				varint::unpack(r4, v);
				assert_equal(0x80000001u, v);

				varint::unpack(r4, v);
				assert_equal(0x80001000u, v);
			}


			test( ByteSizeOfASerializedValueAndItselfAreWrittenWithContext )
			{
				// INIT
				unsigned v = 0, ctx = 123;
				serializer<vector_writer, varint> ser(w);

				A1791 a = {	};
				A190111 b = { };

				// ACT
				ser(a, ctx);

				// ASSERT
				vector_reader r1(buffer);
				
				varint::unpack(r1, v);
				varint::unpack(r1, v);
				assert_equal(2u, v);

				// INIT
				buffer.clear();

				// ACT
				ser(b, ctx);

				// ASSERT
				vector_reader r2(buffer);
				
				varint::unpack(r2, v);
				varint::unpack(r2, v);
				assert_equal(6u, v);

				// INIT
				a.a = 0x10, a.b = 0x1000000;
				buffer.clear();

				// ACT
				ser(a, ctx);

				// ASSERT
				vector_reader r3(buffer);
				
				varint::unpack(r3, v);
				varint::unpack(r3, v);
				assert_equal(5u, v);
				varint::unpack(r3, v);
				assert_equal(0x10u, v);
				varint::unpack(r3, v);
				assert_equal(0x1000000u, v);

				// INIT
				b.nested.a = 0x80000000, b.nested.b = 0x80000001, b.x = 0x80001000;
				buffer.clear();

				// ACT
				ser(b, ctx);

				// ASSERT
				vector_reader r4(buffer);
				
				varint::unpack(r4, v);
				varint::unpack(r4, v);
				assert_equal(18u, v);

				varint::unpack(r4, v);
				varint::unpack(r4, v);
				assert_equal(10u, v);
				varint::unpack(r4, v);
				assert_equal(0x80000000u, v);
				varint::unpack(r4, v);
				assert_equal(0x80000001u, v);

				varint::unpack(r4, v);
				assert_equal(0x80001000u, v);
			}
		end_test_suite
	}
}
