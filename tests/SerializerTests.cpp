#include <strmd/serializer.h>

#include "helpers.h"
#include "types.h"

#include <ut/assert.h>
#include <ut/test.h>

using namespace std;

namespace strmd
{
	namespace tests
	{
		begin_test_suite( SerializerTests )
			vector<unsigned char> buffer;

			test( SerializationStructure )
			{
				// INIT
				vector_writer w(buffer);
				serializer<vector_writer> s(w);
				A data[] = { { 123456, 12, "lorem ipsum" }, { -2123456, 128, "amet dolor" }, };
				
				// ACT / ASSERT (must compile)
				s(data[0]);
				s(data[1]);
			}

			
			test( TrivialStructuresAreSerializedAccordinglyToSchema )
			{
				// INIT
				vector_writer w(buffer);
				serializer<vector_writer> s(w);
				B b = { 0x01, 0x02 };
				C c = { 0x01, 0x02 };

				// ACT
				s(b);
				s(c);

				// ASSERT
				unsigned char reference1[] = { 0x01, 0x02, 0x02, 0x01 };

				assert_equal(reference1, buffer);

				// INIT
				b.a = 0x12;
				b.b = 0xFF;

				// ACT
				s(b);

				// ASSERT
				unsigned char reference2[] = { 0x01, 0x02, 0x02, 0x01, 0x12, 0xFF };

				assert_equal(reference2, buffer);
			}


			test( EmptyContainersAreSerializedAsZeroLengthsBuffers )
			{
				// INIT
				vector_writer w(buffer);
				serializer<vector_writer> s(w);
				string str;

				// ACT
				s(str);

				// ASSERT
				unsigned char reference1[] = { 0x00, 0x00, 0x00, 0x00 };

				assert_equal(reference1, buffer);

				// INIT
				vector<A> va;
				vector<B> vb;

				// ACT
				s(va);
				s(vb);

				// ASSERT
				assert_equal((vector<unsigned char>(3 * 4)), buffer);
			}


			test( ContainersOfTrivialTypesAreSerializedAsSizeAndRepetitionOfElements )
			{
				// INIT
				vector_writer w(buffer);
				serializer<vector_writer> s(w);
				short b[] = { 1232, -322, 11, 1, };
				unsigned int c[] = { 2100111, 3, };
				vector<short> vs(b, b + 4);
				vector<unsigned int> vui(c, c + 2);
				string str = "Lorem ipsum amet dolor";

				// ACT
				s(vs);

				// ASSERT
				unsigned char reference1[] = {
					0x04, 0x00, 0x00, 0x00,
					0xD0, 0x04, 0xBE, 0xFE, 0x0B, 0x00, 0x01, 0x00,
				};

				assert_equal(reference1, buffer);

				// ACT
				s(vui);
				s(str);

				// ASSERT
				unsigned char reference2[] = {
					0x04, 0x00, 0x00, 0x00,
					0xD0, 0x04, 0xBE, 0xFE, 0x0B, 0x00, 0x01, 0x00,
					0x02, 0x00, 0x00, 0x00,
					0x8F, 0x0B, 0x20, 0x00, 0x03, 0x00, 0x00, 0x00,
					0x16, 0x00, 0x00, 0x00,
					'L', 'o', 'r', 'e', 'm', ' ', 'i', 'p', 's', 'u', 'm', ' ', 'a', 'm', 'e', 't', ' ', 'd', 'o', 'l', 'o', 'r',
				};

				assert_equal(reference2, buffer);
			}


			test( ContainersOfStructsAreSerializedAsSizeAndRepetitionOfElements )
			{
				// INIT
				vector_writer w(buffer);
				serializer<vector_writer> s(w);
				B b[] = { { 1, 2 }, { 54, 17 }, };
				C c[] = { { 1, 2 }, { 54, 17 }, { 17, 21 } };
				vector<B> vb(b, b + 2);
				vector<C> vc(c, c + 3);

				// ACT
				s(vb);

				// ASSERT
				unsigned char reference1[] = {
					0x02, 0x00, 0x00, 0x00,
					1, 2, 54, 17,
				};

				assert_equal(reference1, buffer);

				// ACT
				s(vc);

				// ASSERT
				unsigned char reference2[] = {
					0x02, 0x00, 0x00, 0x00,
					1, 2, 54, 17,
					0x03, 0x00, 0x00, 0x00,
					2, 1, 17, 54, 21, 17,
				};

				assert_equal(reference2, buffer);
			}


			test( APairIsSerializedAsTwoElements )
			{
				// INIT
				vector_writer w(buffer);
				serializer<vector_writer> s(w);
				B b[] = { { 1, 2 }, { 54, 17 }, };
				C c[] = { { 1, 2 }, { 17, 21 } };
				pair<B, C> p1[] = {	make_pair(b[0], c[0]), make_pair(b[1], c[1]),	};
				pair<C, B> p2[] = {	make_pair(c[0], b[0]), make_pair(c[1], b[1]),	};

				// ACT
				s(p1[0]);
				s(p1[1]);

				// ASSERT
				unsigned char reference1[] = {
					1, 2, 2, 1, 54, 17, 21, 17,
				};

				assert_equal(reference1, buffer);

				// ACT
				s(p2[0]);
				s(p2[1]);

				// ASSERT
				unsigned char reference2[] = {
					1, 2, 2, 1, 54, 17, 21, 17,
					2, 1, 1, 2, 21, 17, 54, 17,
				};

				assert_equal(reference2, buffer);
			}


			test( AllPrimitiveTypesAreSerialized )
			{
				// INIT
				vector_writer w(buffer);
				serializer<vector_writer> s(w);

				// ACT
				s(static_cast<char>(-1));
				s(static_cast<unsigned char>(2));
				s(static_cast<short>(-300));
				s(static_cast<unsigned short>(1000));
				s(static_cast<int>(-100000));
				s(static_cast<unsigned int>(200000));
				s(static_cast<long long>(-10000000000));
				s(static_cast<unsigned long long>(10000000000));
				s(1.23f);
				s(1.1234);

				// ASSERT
				assert_equal(42u, buffer.size());
				assert_equal(-1, *reinterpret_cast<const char *>(&buffer[0]));
				assert_equal(2u, *reinterpret_cast<const unsigned char *>(&buffer[1]));
				assert_equal(-300, *reinterpret_cast<const short *>(&buffer[2]));
				assert_equal(1000u, *reinterpret_cast<const unsigned short *>(&buffer[4]));
				assert_equal(-100000, *reinterpret_cast<const int *>(&buffer[6]));
				assert_equal(200000u, *reinterpret_cast<const unsigned int *>(&buffer[10]));
				assert_equal(-10000000000, *reinterpret_cast<const long long *>(&buffer[14]));
				assert_equal(10000000000u, *reinterpret_cast<const unsigned long long *>(&buffer[22]));
				assert_equal(1.23f, *reinterpret_cast<const float *>(&buffer[30]));
				assert_equal(1.1234, *reinterpret_cast<const double *>(&buffer[34]));
			}


			test( UnorderedMapIsSerializedAsSequenceOfElements )
			{
				// INIT
				vector_writer w(buffer);
				serializer<vector_writer> s(w);
				unordered_map<int, B> mib;
				B b[] = { { 1, 2 }, { 54, 17 }, };

				mib[8] = b[0];
				mib[-126] = b[1];
				mib[126111] = b[0];

				// ACT
				s(mib);

				// ASSERT
				unordered_map<int, B>::const_iterator i = mib.begin();

				assert_equal(22u, buffer.size());
				assert_equal(3u, *reinterpret_cast<unsigned int *>(&buffer[0]));
				assert_equal(i->first, *reinterpret_cast<const int *>(&buffer[4]));
				assert_equal(i->second.a, *reinterpret_cast<const char *>(&buffer[8]));
				assert_equal(i->second.b, *reinterpret_cast<const char *>(&buffer[9]));
				++i;
				assert_equal(i->first, *reinterpret_cast<const int *>(&buffer[10]));
				assert_equal(i->second.a, *reinterpret_cast<const char *>(&buffer[14]));
				assert_equal(i->second.b, *reinterpret_cast<const char *>(&buffer[15]));
				++i;
				assert_equal(i->first, *reinterpret_cast<const int *>(&buffer[16]));
				assert_equal(i->second.a, *reinterpret_cast<const char *>(&buffer[20]));
				assert_equal(i->second.b, *reinterpret_cast<const char *>(&buffer[21]));
			}

		end_test_suite
	}
}
