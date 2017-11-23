#include <strmd/deserializer.h>

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
			class vector_reader
			{
			public:
				template <size_t N>
				vector_reader(unsigned char (&p)[N])
					: _ptr(p), _remaining(N)
				{	}

				void read(void *data, size_t size)
				{
					assert_is_true(size <= _remaining);
					memcpy(data, _ptr, size);
					_ptr += size;
					_remaining -= size;
				}

			private:
				const unsigned char *_ptr;
				size_t _remaining;
			};
		}

		begin_test_suite( DeserializerTests )
			test( DeserializationStructure )
			{
				// INIT
				unsigned char buffer[9] = { };
				vector_reader r(buffer);
				deserializer<vector_reader> d(r);
				A data;

				// ACT / ASSERT (must compile)
				d(data);
			}


			test( AllPrimitiveTypesAreDeserializedAsZeroValuesFromZeroBuffer )
			{
				// INIT
				unsigned char buffer[42] = { };
				vector_reader r(buffer);
				deserializer<vector_reader> d(r);
				char data_c = 1;
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
				unsigned char buffer[42] = { };
				vector_reader r(buffer);
				deserializer<vector_reader> d(r);
				char data_c = 1;
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
				*reinterpret_cast<unsigned char *>(&buffer[1]) = 2u;
				*reinterpret_cast<short *>(&buffer[2]) = -300;
				*reinterpret_cast<unsigned short *>(&buffer[4]) = 1000u;
				*reinterpret_cast<int *>(&buffer[6]) = -100000;
				*reinterpret_cast<unsigned int *>(&buffer[10]) = 200000u;
				*reinterpret_cast<long long *>(&buffer[14]) = -10000000000;
				*reinterpret_cast<unsigned long long *>(&buffer[22]) = 10000000000u;
				*reinterpret_cast<float *>(&buffer[30]) = 1.23f;
				*reinterpret_cast<double *>(&buffer[34]) = 1.1234;

				// ACT
				d(data_c);
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
				vector_reader r(buffer);
				deserializer<vector_reader> d(r);
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


		end_test_suite
	}
}
