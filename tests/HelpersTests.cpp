#include <strmd/stream_special.h>

#include "helpers.h"

#include <ut/assert.h>
#include <ut/test.h>

using namespace std;

namespace strmd
{
	namespace tests
	{
		namespace
		{
			unsigned char c_sample[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.";
		}

		begin_test_suite( HelpersTests )
			vector<unsigned char> buffer;

			test( WritesCounterInitializesItsValue )
			{
				// INIT / ACT
				writes_counter w;

				// ASSERT
				assert_equal(0u, w.written);
			}


			test( WritesCounterAccumulatesBytesSaved )
			{
				// INIT
				unsigned char data[100];

				// INIT / ACT
				writes_counter w;

				// ACT
				w.write(data, 3);

				// ASSERT
				assert_equal(3u, w.written);

				// INIT
				w.written = 0;

				// ACT
				w.write(0, 95);

				// ASSERT
				assert_equal(95u, w.written);

				// ACT
				w.write(0, 99);

				// ASSERT
				assert_equal(194u, w.written);
			}


			test( ReadsCounterReadsFromUnderlying )
			{
				// INIT
				unsigned char data[2048];
				buffer_reader u(c_sample);

				// INIT / ACT
				fragment_reader<buffer_reader> r(u, 1023);

				// ACT
				r.read(data, 43);

				// ASSERT
				assert_equal(vector<unsigned char>(c_sample, c_sample + 43), vector<unsigned char>(data, data + 43));

				// ACT
				r.read(data, 11);

				// ASSERT
				assert_equal(vector<unsigned char>(c_sample + 43, c_sample + 43 + 11), vector<unsigned char>(data, data + 11));
			}


			test( NestedReadsCounterReadsFromUnderlying )
			{
				// INIT
				unsigned char data[2048];
				buffer_reader u(c_sample);
				fragment_reader<buffer_reader> r1(u, 100);
				fragment_reader<buffer_reader> r2(r1, 50);

				// ACT
				r2.read(data, 43);

				// ASSERT
				assert_equal(vector<unsigned char>(c_sample, c_sample + 43), vector<unsigned char>(data, data + 43));

				// ACT
				r1.read(data, 11);

				// ASSERT
				assert_equal(vector<unsigned char>(c_sample + 43, c_sample + 54), vector<unsigned char>(data, data + 11));
			}


			test( ReadsCounterSkipsFromUnderlying )
			{
				// INIT
				buffer_reader u(c_sample);
				fragment_reader<buffer_reader> r(u, 35);
				unsigned char data[100];

				// ACT
				r.read(data, 6);
				r.skip();

				// ASSERT
				assert_equal(29u, u.skipped);
			}


			test( SecondSkipDoesNotSeekForward )
			{
				// INIT
				buffer_reader u(c_sample);
				fragment_reader<buffer_reader> r(u, 47);
				unsigned char data[100];

				// ACT
				r.read(data, 2);
				r.skip();
				r.skip();
				u.read(data, 5);

				// ASSERT
				assert_equal(vector<unsigned char>(c_sample + 47, c_sample + 52), vector<unsigned char>(data, data + 5));
			}


			test( SkippingRemainingSeeksUnderlyingToTheEndOfTheFragment )
			{
				// INIT
				buffer_reader u1(c_sample);
				buffer_reader u2(c_sample);
				fragment_reader<buffer_reader> r1(u1, 10);
				fragment_reader<buffer_reader> r2(u2, 12);
				unsigned char data[100] = { 0 };

				// ACT
				r1.skip();
				u1.read(data, 5);

				// ASSERT
				assert_equal(vector<unsigned char>(c_sample + 10, c_sample + 15), vector<unsigned char>(data, data + 5));

				// ACT
				r2.skip();
				u2.read(data, 7);

				// ASSERT
				assert_equal(vector<unsigned char>(c_sample + 12, c_sample + 19), vector<unsigned char>(data, data + 7));
			}


			test( SkippingRemainingTakesReadBytesIntoAccount )
			{
				// INIT
				buffer_reader u1(c_sample);
				buffer_reader u2(c_sample);
				fragment_reader<buffer_reader> r1(u1, 10);
				fragment_reader<buffer_reader> r2(u2, 12);
				unsigned char data[100] = { 0 };

				// ACT
				r1.read(data, 2);
				r1.skip();
				u1.read(data, 5);

				// ASSERT
				assert_equal(vector<unsigned char>(c_sample + 10, c_sample + 15), vector<unsigned char>(data, data + 5));

				// ACT
				r2.read(data, 3);
				r2.skip();
				u2.read(data, 7);

				// ASSERT
				assert_equal(vector<unsigned char>(c_sample + 12, c_sample + 19), vector<unsigned char>(data, data + 7));
			}


			test( NestedReadingSeeksRecursiveUnderlyings )
			{
				// INIT
				buffer_reader u(c_sample);
				fragment_reader<buffer_reader> r1(u, 31);
				fragment_reader<buffer_reader> r2(r1, 23);
				fragment_reader<buffer_reader> r3(r2, 17);
				unsigned char data[100];

				// ACT
				r3.read(data, 7);
				r3.skip();

				// ASSERT
				assert_equal(10u, u.skipped);

				// ACT
				r2.skip();

				// ASSERT
				assert_equal(16u, u.skipped);

				// ACT
				r1.skip();

				// ASSERT
				assert_equal(24u, u.skipped);
			}
		end_test_suite
	}
}
