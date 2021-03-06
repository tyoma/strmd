#include <strmd/deserializer.h>

#include "helpers.h"
#include "types.h"

#include <cstring>
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
				bool operator ()(const T &lhs, const T &rhs) const
				{	return lhs < rhs;	}
			};
		}

		begin_test_suite( DeserializerTests )
			test( DeserializationStructure )
			{
				// INIT
				unsigned char buffer[9] = { };
				buffer_reader r(buffer);
				deserializer<buffer_reader> d(r);
				A data;

				// ACT / ASSERT (must compile)
				d(data);
			}


			test( AllPrimitiveTypesAreDeserializedAsZeroValuesFromZeroBuffer )
			{
				// INIT
				unsigned char buffer[44] = { };
				buffer_reader r(buffer);
				deserializer<buffer_reader> d(r);
				char data_c = 1;
				wchar_t data_wc = L'X';
				unsigned char data_uc = 1;
				short data_si = 1;
				unsigned short data_usi = 1;
				int data_i = 1;
				unsigned int data_ui = 1;
				long long data_lli = 1;
				unsigned long long data_ulli = 1;
				float data_f = 1.0f;
				double data_d = 1;

				// ACT
				d(data_c);
				d(data_wc);
				d(data_uc);
				d(data_si);
				d(data_usi);
				d(data_i);
				d(data_ui);
				d(data_lli);
				d(data_ulli);
				d(data_f);
				d(data_d);

				// ASSERT
				assert_equal(0, data_c);
				assert_equal(L'\0', data_wc);
				assert_equal(0u, data_uc);
				assert_equal(0, data_si);
				assert_equal(0u, data_usi);
				assert_equal(0, data_i);
				assert_equal(0u, data_ui);
				assert_equal(0, data_lli);
				assert_equal(0u, data_ulli);
				assert_equal(0.0f, data_f);
				assert_equal(0, data_d);
			}


			test( PrimitiveTypesAreDeserialized )
			{
				// INIT
				unsigned char buffer[44] = { };
				buffer_reader r(buffer);
				deserializer<buffer_reader> d(r);
				char data_c = 1;
				wchar_t data_wc = 1;
				unsigned char data_uc = 1;
				short data_si = 1;
				unsigned short data_usi = 1;
				int data_i = 1;
				unsigned int data_ui = 1;
				long long data_lli = 1;
				unsigned long long data_ulli = 1;
				float data_f = 1.0f;
				double data_d = 1;

				*reinterpret_cast<char *>(&buffer[0]) = -1;
				*reinterpret_cast<wchar_t *>(&buffer[1]) = 0x1234;
				*reinterpret_cast<unsigned char *>(&buffer[3]) = 2u;
				*reinterpret_cast<short *>(&buffer[4]) = -300;
				*reinterpret_cast<unsigned short *>(&buffer[6]) = 1000u;
				*reinterpret_cast<int *>(&buffer[8]) = -100000;
				*reinterpret_cast<unsigned int *>(&buffer[12]) = 200000u;
				*reinterpret_cast<long long *>(&buffer[16]) = -10000000000;
				*reinterpret_cast<unsigned long long *>(&buffer[24]) = 10000000000u;
				*reinterpret_cast<float *>(&buffer[32]) = 1.23f;
				*reinterpret_cast<double *>(&buffer[36]) = 1.1234;

				// ACT
				d(data_c);
				d(data_wc);
				d(data_uc);
				d(data_si);
				d(data_usi);
				d(data_i);
				d(data_ui);
				d(data_lli);
				d(data_ulli);
				d(data_f);
				d(data_d);

				// ASSERT
				assert_equal(-1, data_c);
				assert_equal(0x1234, data_wc);
				assert_equal(2u, data_uc);
				assert_equal(-300, data_si);
				assert_equal(1000u, data_usi);
				assert_equal(-100000, data_i);
				assert_equal(200000u, data_ui);
				assert_equal(-10000000000, data_lli);
				assert_equal(10000000000u, data_ulli);
				assert_equal(1.23f, data_f);
				assert_equal(1.1234, data_d);
			}


			test( ContainersOfPrimitiveTypesAreSerialized )
			{
				// INIT
				unsigned char buffer[] = {
					0x11, 0x00, 0x00, 0x00,
					0x00, 0x10, 0x13, 0x30, 0x00, 0x10, 0x13, 0x30, 0x00, 0x10, 0x13, 0x30, 0x00, 0x10, 0x13, 0x30,
					0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					0xFF, 0xF0,
					0x05, 0x00, 0x00, 0x00,
					'l', 'o', 'r', 'e', 'm',
					0x0B, 0x00, 0x00, 0x00,
					'i', 'p', 's', 'u', 'm', ' ', 'd', 'o', 'l', 'o', 'r',
				};
				buffer_reader r(buffer);
				deserializer<buffer_reader> d(r);
				vector<unsigned short> vs;
				string str1;
				string str2;

				// ACT
				d(vs);
				d(str1);
				d(str2);

				// ASSERT
				unsigned short reference[] = {
					0x1000, 0x3013, 0x1000, 0x3013, 0x1000, 0x3013, 0x1000, 0x3013,
					0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
					0xF0FF,
				};

				assert_equal(reference, vs);
				assert_equal("lorem", str1);
				assert_equal("ipsum dolor", str2);
			}


			test( AssociativeContainerDeserialization )
			{
				unsigned char buffer[] = {
					0x03, 0x00, 0x00, 0x00,
					'a', 0x10, 0x13, 'c', 0x00, 0x10, 'Z', 0x30, 0x00,
					0x02, 0x00, 0x00, 0x00,
					0x34, 0x12, 0x00, 0x00, 0x10, 0x11, 0x43, 0x21, 0x10, 0x00, 0x00, 0x11,
				};
				buffer_reader r(buffer);
				deserializer<buffer_reader> d(r);
				map<char, unsigned short> ucus;
				map<unsigned short, int, custom_compare> uusi;

				// ACT
				d(ucus);
				d(uusi);

				// ASSERT
				pair<char, unsigned short> reference1[] = {
					make_pair('a', static_cast<unsigned short>(0x1310)),
					make_pair('c', static_cast<unsigned short>(0x1000)),
					make_pair('Z', static_cast<unsigned short>(0x0030)),
				};
				pair<unsigned short, int> reference2[] = {
					make_pair(static_cast<unsigned short>(0x1234), 0x11100000),
					make_pair(static_cast<unsigned short>(0x2143), 0x11000010),
				};

				assert_equivalent(reference1, (vector< pair<char, unsigned short> >(ucus.begin(), ucus.end())));
				assert_equivalent(reference2, (vector< pair<unsigned short, int> >(uusi.begin(), uusi.end())));
			}


			test( ContainersAreClearedOnBeforeDeserialize )
			{
				// INIT
				unsigned char buffer[] = {
					0x00, 0x00, 0x00, 0x00,
					0x00, 0x00, 0x00, 0x00,
				};
				buffer_reader r(buffer);
				deserializer<buffer_reader> d(r);
				map<char, unsigned short> ucus;
				vector<unsigned short> vs(4);

				ucus[3] = 22;

				// ACT
				d(ucus);
				d(vs);

				// ASSERT
				assert_is_empty(ucus);
				assert_is_empty(vs);
			}


			test( ComplexTypesAreDeserializedAsExpected )
			{
				// INIT
				unsigned char buffer[] = {
					0x78, 0x56, 0x34, 0x12, 0xF9,
					0x05, 0x00, 0x00, 0x00, 'l', 'o', 'r', 'e', 'm',
					0x21, 0x43, 0x65, 0x77, 0x1B,
					0x03, 0x00, 0x00, 0x00, 'T', 'h', 'e',
					0x67, 0x61,
				};
				buffer_reader r(buffer);
				deserializer<buffer_reader> d(r);
				A a;
				C c;

				// ACT
				d(a);

				// ASSERT
				assert_equal(0x12345678, a.a);
				assert_equal(0xF9, a.b);
				assert_equal("lorem", a.c);

				// ACT
				d(a);
				d(c);

				// ASSERT
				assert_equal(0x77654321, a.a);
				assert_equal(0x1B, a.b);
				assert_equal("The", a.c);
				assert_equal(0x61, c.a);
				assert_equal(0x67, c.b);
			}


		end_test_suite
	}
}
