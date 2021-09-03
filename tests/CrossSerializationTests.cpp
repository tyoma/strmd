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
	inline void serialize(ArchiveT &archive, pair<T1, T2> &data, string &context)
	{
		archive(data.first);
		archive(data.second);
		context = "archive";
	}

	template <typename ArchiveT, typename T1>
	inline void serialize(ArchiveT &archive, pair<T1, string> &data, long &context)
	{
		archive(data.first);
		archive(data.second);
		context += static_cast<long>(data.second.size());
	}

	template <typename ArchiveT, typename T1>
	inline void serialize(ArchiveT &archive, pair<T1, int> &data, int &context)
	{
		archive(data.first);
		archive(data.second);
		context = data.first;
	}

	template <typename ArchiveT, typename ContextT>
	inline void serialize(ArchiveT &archive, pair<string, int> &data, ContextT &context)
	{
		archive(data.first);
		archive(data.second);
		context.push_back(data.first);
	}

	template <typename ArchiveT>
	inline void serialize(ArchiveT &archive, pair<int, int> &data, unsigned long long &context)
	{
		archive(data.first);
		archive(data.second);
		context += data.first + data.second;
	}

	template <typename ArchiveT>
	inline void serialize(ArchiveT &archive, tests::MyKey &data, pair< vector<int>, vector<string> > &context)
	{
		archive(data.value);
		context.first.push_back(data.value);
	}

	template <typename ArchiveT>
	inline void serialize(ArchiveT &archive, tests::A &data, pair< vector<int>, vector<string> > &context)
	{
		archive(data.a);
		archive(data.b);
		archive(data.c);
		context.second.push_back(data.c);
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


			test( NormallySerializedObjectsCanBeContextuallyDeserialized )
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
				long lcontext = 0;
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
				s(reference1, lcontext);

				// ASSERT
				vector< pair<int, string> > read1;

				d(read1);
				assert_equal(reference1_, read1);
				assert_equal(17, lcontext);

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


			test( NormallySerializedContainerCanBeContextuallyDeserialized )
			{
				// INIT
				vector_writer w(buffer);
				vector_reader r(buffer);
				serializer<vector_writer> s(w);
				deserializer<vector_reader> d(r);
				unsigned long long ullcontext = 123;
				vector<string> vcontext;
				pair<string, int> reference1_[] = {
					make_pair("chapel", 1), make_pair("went", 1), make_pair("jingle", 2), make_pair("jangle", 3),
				};
				pair<int, int> reference2_[] = {
					make_pair(1, 11), make_pair(21, 1211), make_pair(11121111, 1),
				};
				vector< pair<string, int> >reference1(mkvector(reference1_)), read1;
				vector< pair<int, int> > reference2(mkvector(reference2_)), read2;
				map<string, int> read3;

				s(reference1);

				// ACT
				d(read1, vcontext);

				// ASSERT
				string reference_vcontext[] = { "chapel", "went", "jingle", "jangle", };

				assert_equal(reference1, read1);
				assert_equal(reference_vcontext, vcontext);

				// INIT
				s(reference2);

				// ACT
				d(read2, ullcontext);

				// ASSERT
				assert_equal(reference2, read2);
				assert_equal(123 + 1 + 11 + 21 + 1211 + 11121111 + 1, ullcontext);

				// INIT
				s(reference1);

				// ACT
				d(read3, vcontext);

				// ASSERT
				assert_equivalent(reference1_, read3);
			}


			template <typename ContainerT>
			void ContextIsPassedOnToElementsBeingReadImpl()
			{
				// INIT
				vector_writer w(buffer);
				vector_reader r(buffer);
				serializer<vector_writer> s(w);
				deserializer<vector_reader> d(r);
				A data[] = { { 0, 0, "foo" }, { 0, 0, "bar" }, { 0, 0, "A" }, { 0, 0, "B" }, { 0, 0, "C" } };
				pair<MyKey, A> data1[] = { make_pair(1, data[0]), make_pair(111, data[1]), };
				pair<MyKey, A> data2[] = { make_pair(10, data[2]), make_pair(20, data[3]), make_pair(30, data[4]) };
				ContainerT container;
				pair< vector<int>, vector<string> > context;

				s(mkvector(data1));
				s(mkvector(data2));

				// ACT
				d(container, context);

				// ASSERT
				int reference11[] = { 1, 111, };
				string reference12[] = { "foo", "bar", };

				assert_equal(reference11, context.first);
				assert_equal(reference12, context.second);

				// INIT
				context.first.clear();
				context.second.clear();

				// ACT
				d(container, context);

				// ASSERT
				int reference21[] = { 10, 20, 30, };
				string reference22[] = { "A", "B", "C", };

				assert_equal(reference21, context.first);
				assert_equal(reference22, context.second);
			}

			test( ContextIsPassedOnToElementsBeingRead )
			{
				ContextIsPassedOnToElementsBeingReadImpl< vector< pair<MyKey, A> > >();
				ContextIsPassedOnToElementsBeingReadImpl< list< pair<MyKey, A> > >();
				ContextIsPassedOnToElementsBeingReadImpl< deque< pair<MyKey, A> > >();

				ContextIsPassedOnToElementsBeingReadImpl< map<MyKey, A> >();
				ContextIsPassedOnToElementsBeingReadImpl< multimap<MyKey, A> >();
			}

		end_test_suite
	}
}
