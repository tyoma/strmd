#include <strmd/deserializer.h>
#include <strmd/serializer.h>

#include "helpers.h"
#include "types.h"

#include <ut/assert.h>
#include <ut/test.h>

using namespace std;

namespace strmd
{
	template <typename ArchiveT, typename T1, typename T2>
	inline void serialize(ArchiveT &archive, std::pair<T1, T2> &data, unsigned /*version*/, string &context)
	{
		archive(data.first);
		archive(data.second);
		context = "archive";
	}

	template <typename ArchiveT, typename T1>
	inline void serialize(ArchiveT &archive, std::pair<T1, string> &data, unsigned /*version*/, wstring &context)
	{
		archive(data.first);
		archive(data.second);
		context.insert(context.end(), data.second.begin(), data.second.end());
	}

	template <typename ArchiveT, typename T1>
	inline void serialize(ArchiveT &archive, std::pair<T1, int> &data, unsigned /*version*/, int &context)
	{
		archive(data.first);
		archive(data.second);
		context = data.first;
	}

	namespace tests
	{
		namespace
		{
			template <typename T, size_t n>
			vector<T> mkvector(T (&arr)[n])
			{	return vector<T>(arr, arr + n);	}
		}

		begin_test_suite( CrossSerializationTests )
			vector<unsigned char> buffer;

			test( ContextualizedSerializationIsNormallyDeserialized )
			{
				// INIT
				vector_writer w(buffer);
				vector_reader r(buffer);
				serializer<vector_writer> s(w);
				deserializer<vector_reader> d(r);
				string scontext;
				int icontext;

				pair<int, string> reference1(121, "one two one");
				pair<int, int> reference2(3, 1415926), reference3(1415926, 3);

				// ACT
				s(reference1, scontext);

				// ASSERT
				pair<int, string> read1;

				d(read1);
				assert_equal(reference1, read1);
				assert_equal("archive", scontext);

				// ACT
				s(reference2, scontext);

				// ASSERT
				pair<int, int> read2;

				d(read2);
				assert_equal(reference2, read2);

				// ACT
				s(reference2, icontext);

				// ASSERT
				d(read2);
				assert_equal(reference2, read2);
				assert_equal(3, icontext);

				// ACT
				s(reference3, icontext);

				// ASSERT
				d(read2);
				assert_equal(reference3, read2);
				assert_equal(1415926, icontext);
			}


			test( NormallySerializedCanBeContextuallyDeserialized )
			{
				// INIT
				vector_writer w(buffer);
				vector_reader r(buffer);
				serializer<vector_writer> s(w);
				deserializer<vector_reader> d(r);
				string scontext;
				int icontext;
				pair<int, string> read1;
				pair<int, int> read2;

				pair<int, string> reference1(121, "one two one");
				pair<int, int> reference2(3, 1415926), reference3(1415926, 3);

				s(reference1);

				// ACT
				d(read1, scontext);

				// ASSERT
				assert_equal(reference1, read1);
				assert_equal("archive", scontext);

				// INIT
				scontext = "";
				s(reference2, scontext);

				// ACT
				d(read2);

				// ASSERT
				assert_equal(reference2, read2);
				assert_equal("archive", scontext);

				// INIT
				s(reference2);

				// ACT
				d(read2, icontext);

				// ASSERT
				assert_equal(reference2, read2);
				assert_equal(3, icontext);

				// INIT
				s(reference3);

				// ACT
				d(read2, icontext);

				// ASSERT
				assert_equal(reference3, read2);
				assert_equal(1415926, icontext);
			}


			test( ContextualizedContainerSerializationIsNormallyDeserialized )
			{
				// INIT
				vector_writer w(buffer);
				vector_reader r(buffer);
				serializer<vector_writer> s(w);
				deserializer<vector_reader> d(r);
				wstring scontext;
				int icontext;
				pair<int, string> reference1_[] = {
					make_pair(112, "one two"), make_pair(113, "lorem"), make_pair(7, "ipsum"),
				};
				pair<int, int> reference2_[] = {
					make_pair(2, 7), make_pair(3, 1), make_pair(9, 8),
				};
				int reference3_[] = { 17, 19, 1001, 4, 51, };


				vector< pair<int, string> >reference1(mkvector(reference1_));
				map<int, int> reference2(reference2_, reference2_ + 3);
				vector<int> reference3(mkvector(reference3_));

				// ACT
				s(reference1, scontext);

				// ASSERT
				vector< pair<int, string> > read1;

				d(read1);
				assert_equal(reference1_, read1);
				assert_equal(L"one twoloremipsum", scontext);

				// ACT
				s(reference2, icontext);

				// ASSERT
				vector< pair<int, int> > read2;

				d(read2);
				assert_equal(reference2_, read2);
				assert_equal(9, icontext);

				// INIT
				icontext = 13828245; // Some value. Must not be disturbed.

				// ACT
				s(reference3, icontext);

				// ASSERT
				vector<int> read3;

				d(read3);
				assert_equal(reference3_, read3);
				assert_equal(13828245, icontext);
			}

		end_test_suite
	}
}
