#include <strmd/deserializer.h>

#include "helpers.h"

#include <ut/assert.h>
#include <ut/test.h>

using namespace std;

namespace
{
	struct context_t
	{
		int v;
	};

	struct A1
	{
		unsigned a, b, c;

		vector<unsigned> versions;
		vector<int> contexts;
	};

	struct V13
	{
		string a;
		int b;
	};

	struct V15
	{
		int a;
		vector<int> b;
	};
}

namespace strmd
{
	template <> struct version<A1> {	enum {	value = 7777777	};	};
	template <> struct version<V13> {	enum {	value = 7777777	};	};
	template <> struct version<V15> {	enum {	value = 7777777	};	};

	template <typename ArchiveT>
	void serialize(ArchiveT &a, A1 &v, unsigned ver)
	{
		a(v.a), a(v.b), a(v.c);
		v.versions.push_back(ver);
	}

	template <typename ArchiveT>
	void serialize(ArchiveT &a, A1 &v, context_t &ctx, unsigned ver)
	{
		a(v.a), a(v.b), a(v.c);
		v.versions.push_back(ver);
		v.contexts.push_back(ctx.v);
	}

	template <typename ArchiveT>
	void serialize(ArchiveT &a, V13 &v, unsigned ver)
	{
		a(v.a), a(v.b);
		assert_equal(13u, ver);
	}

	template <typename ArchiveT>
	void serialize(ArchiveT &a, V15 &v, unsigned ver)
	{
		a(v.a), a(v.b);
		assert_equal(15u, ver);
	}

	template <typename ArchiveT>
	void serialize(ArchiveT &a, V13 &v, const string &ctx, unsigned ver)
	{
		a(v.a), a(v.b);
		assert_equal("BAR", ctx);
		assert_equal(13u, ver);
	}

	template <typename ArchiveT>
	void serialize(ArchiveT &a, V15 &v, int ctx, unsigned ver)
	{
		a(v.a), a(v.b);
		assert_equal(181, ctx);
		assert_equal(15u, ver);
	}

	namespace tests
	{
		begin_test_suite( VersionedDeSerializationTests )
			vector<unsigned char> buffer;
			vector_writer w;

			VersionedDeSerializationTests()
				: w(buffer)
			{	}


			test( VersionIsProvidedToSerializerFunction )
			{
				// INIT
				vector_reader r(buffer);
				deserializer<vector_reader, varint> dser(r);
				A1 a;
				varint::pack(w, 7u);
				varint::pack(w, 5u);
				varint::pack(w, 111111u);
				varint::pack(w, 11u);
				varint::pack(w, 117u);

				// ACT
				dser(a);

				// ASSERT
				assert_equal(1u, a.versions.size());
				assert_equal(7u, a.versions[0]);
				assert_equal(111111u, a.a);
				assert_equal(11u, a.b);
				assert_equal(117u, a.c);

				// INIT
				varint::pack(w, 7001u);
				varint::pack(w, 6u);
				varint::pack(w, 1111u);
				varint::pack(w, 111u);
				varint::pack(w, 111700u);

				// ACT
				dser(a);

				// ASSERT
				assert_equal(2u, a.versions.size());
				assert_equal(7001u, a.versions[1]);
				assert_equal(1111u, a.a);
				assert_equal(111u, a.b);
				assert_equal(111700u, a.c);
			}


			test( VersionIsProvidedToSerializerFunctionWithContext )
			{
				// INIT
				vector_reader r(buffer);
				deserializer<vector_reader, varint> dser(r);
				context_t ctx;
				A1 a;
				varint::pack(w, 7u);
				varint::pack(w, 5u);
				varint::pack(w, 111111u);
				varint::pack(w, 11u);
				varint::pack(w, 117u);

				// ACT
				dser(a, ctx);

				// ASSERT
				assert_equal(1u, a.versions.size());
				assert_equal(7u, a.versions[0]);
				assert_equal(111111u, a.a);
				assert_equal(11u, a.b);
				assert_equal(117u, a.c);

				// INIT
				varint::pack(w, 7001u);
				varint::pack(w, 6u);
				varint::pack(w, 1111u);
				varint::pack(w, 111u);
				varint::pack(w, 111700u);

				// ACT
				dser(a, ctx);

				// ASSERT
				assert_equal(2u, a.versions.size());
				assert_equal(7001u, a.versions[1]);
				assert_equal(1111u, a.a);
				assert_equal(111u, a.b);
				assert_equal(111700u, a.c);
			}


			test( IncompleteReadsSkipsOverToNextRecord )
			{
				// INIT
				vector_reader r(buffer);
				deserializer<vector_reader, varint> dser(r);
				A1 a;
				varint::pack(w, 7u);
				varint::pack(w, 173u);
				varint::pack(w, 31u);
				varint::pack(w, 11u);
				varint::pack(w, 17u);
				buffer.resize(buffer.size() + 170, 190u);
				varint::pack(w, 7u);
				varint::pack(w, 135u);
				varint::pack(w, 128u); // 2 bytes
				varint::pack(w, 10u);
				varint::pack(w, 97u);
				buffer.resize(buffer.size() + 131, 255u);
				varint::pack(w, 7u);
				varint::pack(w, 100u);
				varint::pack(w, 1u);
				varint::pack(w, 11u);
				varint::pack(w, 23u);

				// ACT
				dser(a);

				// ASSERT
				assert_equal(31u, a.a);
				assert_equal(11u, a.b);
				assert_equal(17u, a.c);

				// ACT
				dser(a);

				// ASSERT
				assert_equal(128u, a.a);
				assert_equal(10u, a.b);
				assert_equal(97u, a.c);

				// ACT
				dser(a);

				// ASSERT
				assert_equal(1u, a.a);
				assert_equal(11u, a.b);
				assert_equal(23u, a.c);
			}


			test( IncompleteReadsSkipsOverToNextRecordWithContext )
			{
				// INIT
				vector_reader r(buffer);
				deserializer<vector_reader, varint> dser(r);
				context_t ctx;
				A1 a;
				varint::pack(w, 7u);
				varint::pack(w, 173u);
				varint::pack(w, 31u);
				varint::pack(w, 11u);
				varint::pack(w, 17u);
				buffer.resize(buffer.size() + 170, 190u);
				varint::pack(w, 7u);
				varint::pack(w, 135u);
				varint::pack(w, 128u); // 2 bytes
				varint::pack(w, 10u);
				varint::pack(w, 97u);
				buffer.resize(buffer.size() + 131, 255u);
				varint::pack(w, 7u);
				varint::pack(w, 100u);
				varint::pack(w, 1u);
				varint::pack(w, 11u);
				varint::pack(w, 23u);

				// ACT
				dser(a, ctx);

				// ASSERT
				assert_equal(31u, a.a);
				assert_equal(11u, a.b);
				assert_equal(17u, a.c);

				// ACT
				dser(a, ctx);

				// ASSERT
				assert_equal(128u, a.a);
				assert_equal(10u, a.b);
				assert_equal(97u, a.c);

				// ACT
				dser(a, ctx);

				// ASSERT
				assert_equal(1u, a.a);
				assert_equal(11u, a.b);
				assert_equal(23u, a.c);
			}


			test( StaticVersionDeserializationDoesNotAttemptToReadVersionAndSize )
			{
				// INIT
				vector_reader r(buffer);
				deserializer<vector_reader, varint, 13> dser1(r);
				V13 v13 = {};
				V15 v15 = {};
				varint::pack(w, 3u);
				varint::pack(w, 'f');
				varint::pack(w, 'o');
				varint::pack(w, 'o');
				varint::pack(w, 1191183);

				varint::pack(w, 19191723);
				varint::pack(w, 2u);
				varint::pack(w, 173);
				varint::pack(w, 171);

				// ACT
				dser1(v13);

				// ASSERT
				assert_equal("foo", v13.a);
				assert_equal(1191183, v13.b);

				// INIT
				deserializer<vector_reader, varint, 15> dser2(r);

				// ACT
				dser2(v15);

				// ASSERT
				unsigned reference[] = {	173u, 171u,	};

				assert_equal(19191723, v15.a);
				assert_equal(reference, v15.b);
			}


			test( StaticVersionDeserializationDoesNotAttemptToReadVersionAndSizeWithContext )
			{
				// INIT
				vector_reader r(buffer);
				deserializer<vector_reader, varint, 13> dser1(r);
				V13 v13 = {};
				V15 v15 = {};
				varint::pack(w, 3u);
				varint::pack(w, 'f');
				varint::pack(w, 'o');
				varint::pack(w, 'o');
				varint::pack(w, 1191183);

				varint::pack(w, 19191723);
				varint::pack(w, 2u);
				varint::pack(w, 173);
				varint::pack(w, 171);

				// ACT
				dser1(v13, "BAR");

				// ASSERT
				assert_equal("foo", v13.a);
				assert_equal(1191183, v13.b);

				// INIT
				deserializer<vector_reader, varint, 15> dser2(r);
				int ctx = 181;

				// ACT
				dser2(v15, ctx);

				// ASSERT
				unsigned reference[] = {	173u, 171u,	};

				assert_equal(19191723, v15.a);
				assert_equal(reference, v15.b);
			}
		end_test_suite
	}
}
