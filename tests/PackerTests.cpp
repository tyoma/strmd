#include <strmd/packer.h>

#include "helpers.h"

#include <limits>
#include <ut/assert.h>
#include <ut/test.h>

using namespace std;

namespace strmd
{
	namespace tests
	{
		begin_test_suite( PackerTests )

			vector<unsigned char> buffer;

			test( ZeroTakesOneByteWithTerminator )
			{
				// INIT
				vector_writer stream(buffer);

				// ACT
				varint::pack(stream, static_cast<unsigned char>(0));

				// ASSERT
				unsigned char reference1[] = { varint::terminator, };

				assert_equal(reference1, buffer);

				// ACT
				varint::pack(stream, static_cast<unsigned short>(0));
				varint::pack(stream, static_cast<unsigned int>(0));
				varint::pack(stream, static_cast<unsigned long long int>(0));

				// ASSERT
				unsigned char reference2[] = {
					varint::terminator, varint::terminator, varint::terminator, varint::terminator,
				};

				assert_equal(reference2, buffer);
			}


			test( SevenBitValuesAreEncodedIntoOneByte )
			{
				// INIT
				vector_writer stream(buffer);

				// ACT
				varint::pack(stream, static_cast<unsigned char>(0x01));
				varint::pack(stream, static_cast<unsigned short>(0x01));
				varint::pack(stream, static_cast<unsigned int>(0x01));
				varint::pack(stream, static_cast<unsigned long long int>(0x01));

				// ASSERT
				unsigned char reference1[] = {
					0x01 | varint::terminator,
					0x01 | varint::terminator,
					0x01 | varint::terminator,
					0x01 | varint::terminator,
				};

				assert_equal(reference1, buffer);

				// INIT
				buffer.clear();

				// ACT
				varint::pack(stream, static_cast<unsigned char>(0x13));
				varint::pack(stream, static_cast<unsigned short>(0x17));
				varint::pack(stream, static_cast<unsigned int>(0x19));
				varint::pack(stream, static_cast<unsigned long long int>(0x31));

				// ASSERT
				unsigned char reference2[] = {
					0x13 | varint::terminator,
					0x17 | varint::terminator,
					0x19 | varint::terminator,
					0x31 | varint::terminator,
				};

				assert_equal(reference2, buffer);
			}


			test( EightBitValuesTakeTwoBytesToPack )
			{
				// INIT
				vector_writer stream(buffer);

				// ACT
				varint::pack(stream, static_cast<unsigned char>(0x81));

				// ASSERT
				unsigned char reference1[] = {
					0x01, 0x81 | varint::terminator,
				};

				assert_equal(reference1, buffer);

				// INIT
				buffer.clear();

				// ACT
				varint::pack(stream, static_cast<unsigned short>(0x8F));
				varint::pack(stream, static_cast<unsigned int>(0xF0));
				varint::pack(stream, static_cast<unsigned long long>(0xFF));

				// ASSERT
				unsigned char reference2[] = {
					0x0F, 0x81 | varint::terminator,
					0x70, 0x81 | varint::terminator,
					0x7F, 0x81 | varint::terminator,
				};

				assert_equal(reference2, buffer);
			}


			test( Arbitrary64BitNumbersPackedUpToTenBytes )
			{
				// INIT
				vector_writer stream(buffer);

				// ACT
				varint::pack(stream, 0x800000000u);

				// ASSERT
				unsigned char reference1[] = {
					0x00, 0x00, 0x00, 0x00, 0x00, 0x01 | varint::terminator,
				};

				assert_equal(reference1, buffer);

				// INIT
				buffer.clear();

				// ACT
				varint::pack(stream, 0xF4844212000000F0u);

				// ASSERT
				unsigned char reference2[] = {
					0x70, 0x01, 0x00, 0x00, 0x20, 0x42, 0x10, 0x42, 0x74, 0x01 | varint::terminator,
				};

				assert_equal(reference2, buffer);
			}


			test( SignedValuesAreZigZagedOnEncoding )
			{
				// INIT
				vector_writer stream(buffer);

				// ACT
				varint::pack(stream, short(-16));

				// ASSERT
				unsigned char reference1[] = {
					varint::terminator | 0x1F,
				};

				assert_equal(reference1, buffer);

				// ACT
				varint::pack(stream, static_cast<char>(0x1B));  // treated as unsigned
				varint::pack(stream, static_cast<unsigned char>(0x1D));
				varint::pack(stream, static_cast<signed char>(0x7E));  // treated as unsigned
				varint::pack(stream, static_cast<int>(-65536));
				varint::pack(stream, numeric_limits<int>::min());
				varint::pack(stream, numeric_limits<long long>::min());

				// ASSERT
				unsigned char reference2[] = {
					0x1F | varint::terminator,
					0x1B | varint::terminator,
					0x1D | varint::terminator,
					0x7E | varint::terminator,
					0x7F, 0x7F, 0x07 | varint::terminator,
					0x7F, 0x7F, 0x7F, 0x7F, 0x0F | varint::terminator,
					0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x01 | varint::terminator,
				};

				assert_equal(reference2, buffer);
			}


			test( SignedMinLongIsPackedTheSameWayAsUnsignedMaxLong )
			{
				// INIT
				vector_writer stream(buffer);

				// ACT (hello x64 UNIX)
				varint::pack(stream, numeric_limits<long>::min());

				// ASSERT
				vector<unsigned char> actual = buffer;

				buffer.clear();
				varint::pack(stream, numeric_limits<unsigned long>::max());

				assert_equal(buffer, actual);
			}

		end_test_suite


		begin_test_suite( UnpackerTests )
			test( ZeroesAreDecodedFromSingleBytes )
			{
				// INIT
				unsigned char buffer[] = {
					varint::terminator, varint::terminator, varint::terminator, varint::terminator, varint::terminator,
				};
				buffer_reader stream(buffer);
				unsigned char uc = 1;
				unsigned short us = 1;
				unsigned int ui = 1;
				unsigned long int ul = 1;
				unsigned long long int ull = 1;

				// ACT
				varint::unpack(stream, uc);

				// ASSERT
				assert_equal(0u, uc);

				// ACT
				varint::unpack(stream, us);
				varint::unpack(stream, ui);
				varint::unpack(stream, ul);
				varint::unpack(stream, ull);

				// ASSERT
				assert_equal(0u, us);
				assert_equal(0u, ui);
				assert_equal(0u, ul);
				assert_equal(0u, ull);
			}


			test( SingleByteValuesAreExtractedWithTerminatorRemoved )
			{
				// INIT
				unsigned char buffer[] = {
					0x18 | varint::terminator,
					0x79 | varint::terminator,
					0x5B | varint::terminator,
					0x4C | varint::terminator,
					0x7F | varint::terminator,
				};
				buffer_reader stream(buffer);
				unsigned char uc = 1;
				unsigned short us = 1;
				unsigned int ui = 1;
				unsigned long int ul = 1;
				unsigned long long int ull = 1;

				// ACT
				varint::unpack(stream, uc);

				// ASSERT
				assert_equal(0x18u, uc);

				// ACT
				varint::unpack(stream, us);
				varint::unpack(stream, ui);
				varint::unpack(stream, ul);
				varint::unpack(stream, ull);

				// ASSERT
				assert_equal(0x79u, us);
				assert_equal(0x5Bu, ui);
				assert_equal(0x4Cu, ul);
				assert_equal(0x7Fu, ull);
			}


			test( MultiByteValuesAreExtracted )
			{
				// INIT
				unsigned char buffer[] = {
					0x1D, 0x01 | varint::terminator,
					0x1D, 0x03 | varint::terminator,
					0x50, 0x79, 0x16 | varint::terminator,
					0x12, 0x18, 0x01 | varint::terminator,
					0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x3F | varint::terminator,
				};
				buffer_reader stream(buffer);
				unsigned char uc = 1;
				unsigned short us = 1;
				unsigned int ui = 1;
				unsigned long int ul = 1;
				unsigned long long int ull = 1;

				// ACT
				varint::unpack(stream, uc);

				// ASSERT
				assert_equal(0x9Du, uc);

				// ACT
				varint::unpack(stream, us);
				varint::unpack(stream, ui);
				varint::unpack(stream, ul);
				varint::unpack(stream, ull);

				// ASSERT
				assert_equal(0x019Du, us);
				assert_equal(0x05BCD0u, ui);
				assert_equal(0x4C12u, ul);
				assert_equal(0xFF0000000000u, ull);
			}


			test( SingleByteSignedValuesAreProperlyRetrieved )
			{
				// INIT
				unsigned char buffer[] = {
					0x19 | varint::terminator,
					0x18 | varint::terminator,
					0x79 | varint::terminator,
					0x5B | varint::terminator,
					0x4C | varint::terminator,
					0x7F | varint::terminator,
				};
				buffer_reader stream(buffer);
				signed char c = 1;
				signed char sc = 1;
				signed short ss = 1;
				signed int si = 1;
				signed long int sl = 1;
				signed long long int sll = 1;

				// ACT
				varint::unpack(stream, c); // treated as unsigned
				varint::unpack(stream, sc); // treated as unsigned

				// ASSERT
				assert_equal(0x19, c);
				assert_equal(0x18, sc);

				// ACT
				varint::unpack(stream, ss);
				varint::unpack(stream, si);
				varint::unpack(stream, sl);
				varint::unpack(stream, sll);

				// ASSERT
				assert_equal(-0x3D, ss);
				assert_equal(-0x2E, si);
				assert_equal(0x26, sl);
				assert_equal(-0x40, sll);
			}


			test( MultiByteSignedValuesAreProperlyRetrieved )
			{
				// INIT
				unsigned char buffer[] = {
					0x7F, 0x7F, 0x07 | varint::terminator,
					0x7F, 0x7F, 0x7F, 0x7F, 0x0F | varint::terminator,
					0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x01 | varint::terminator,
				};
				buffer_reader stream(buffer);
				signed int si;
				signed long int sl;
				signed long long int sll;

				// ACT
				varint::unpack(stream, si);
				varint::unpack(stream, sl);
				varint::unpack(stream, sll);

				// ASSERT
				assert_equal(-65536, si);
				assert_equal(numeric_limits<int>::min(), sl);
				assert_equal(numeric_limits<long long>::min(), sll);
			}
		end_test_suite
	}
}
