#include <strmd/serializer.h>
#include <strmd/deserializer.h>

#include "helpers.h"

#include <memory>
#include <ut/assert.h>
#include <ut/test.h>

using namespace std;

namespace
{
	template <typename T>
	struct Recursive
	{
		Recursive()
			: nested(make_shared< vector<Recursive> >())
		{	}

		shared_ptr< vector<Recursive> > nested;
	};
}

namespace strmd
{
	template <typename T> struct version< Recursive<T> > {	enum {	value = 1	};	};

	template <typename ArchiveT, typename T>
	inline void serialize(ArchiveT &a, Recursive<T> &v, unsigned /*ver*/)
	{	a(*v.nested);	}

	template <typename ArchiveT, typename T>
	inline void serialize(ArchiveT &a, Recursive<T> &v, const string &ctx, unsigned /*ver*/)
	{	a(*v.nested, ctx);	}


	namespace tests
	{
		begin_test_suite( RecursiveTypesSupport )
			vector<unsigned char> buffer;
			vector_writer w;

			RecursiveTypesSupport()
				: w(buffer)
			{	}


			test( DeserializationOfRecursiveTypesIsCompilable )
			{
				// INIT
				vector_reader r(buffer);
				deserializer<vector_reader, varint> dser(r);
				Recursive<int> x;

				varint::pack(w, 1u); // 1. version
				varint::pack(w, 1u); // 1. byte size
				varint::pack(w, 0u); // 1. elements count

				// ACT / ASSERT
				dser(x);
			}


			test( DeserializationOfRecursiveTypesIsCompilableWithContext )
			{
				// INIT
				vector_reader r(buffer);
				deserializer<vector_reader, varint> dser(r);
				Recursive<int> x;
				string dummy_ctx;

				varint::pack(w, 1u); // 1. version
				varint::pack(w, 1u); // 1. byte size
				varint::pack(w, 0u); // 1. elements count

				// ACT / ASSERT
				dser(x, dummy_ctx);
			}


			test( RecursiveSerializationIsSupportedWithContext )
			{
				// INIT
				vector_reader r(buffer);
				serializer<vector_writer> s(w);
				deserializer<vector_reader> d(r);
				Recursive<string> x, read;
				string ctx;

				x.nested->resize(4);
					(*x.nested)[1].nested->resize(3);
						(*(*x.nested)[1].nested)[0].nested->resize(2);
						(*(*x.nested)[1].nested)[1].nested->resize(4);
					(*x.nested)[2].nested->resize(7);

				// ACT
				s(x);
				d(read, ctx);

				// ASSERT
				assert_equal(4u, read.nested->size());
				assert_equal(0u, (*read.nested)[0].nested->size());
				assert_equal(3u, (*read.nested)[1].nested->size());
				assert_equal(2u, (*(*read.nested)[1].nested)[0].nested->size());
				assert_equal(4u, (*(*read.nested)[1].nested)[1].nested->size());
				assert_equal(0u, (*(*read.nested)[1].nested)[2].nested->size());
				assert_equal(7u, (*read.nested)[2].nested->size());
				assert_equal(0u, (*read.nested)[3].nested->size());
			}

		end_test_suite
	}
}
