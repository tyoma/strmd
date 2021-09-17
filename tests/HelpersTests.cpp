#include <strmd/stream_counter.h>

#include "helpers.h"

#include <ut/assert.h>
#include <ut/test.h>

using namespace std;

namespace strmd
{
	namespace tests
	{
		begin_test_suite( HelpersTests )
			vector<unsigned char> buffer;

			test( WritesCounterAccumulatesBytesSaved )
			{
				// INIT
				unsigned char data[100];

				// INIT / ACT
				writes_counter w = { 190u	};

				// ACT
				w.write(data, 3);

				// ASSERT
				assert_equal(193u, w.written);

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


			test( ReadsCounterSubtractsBytesReadFromRemainder )
			{
				// INIT
				unsigned char reference[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.";
				unsigned char data[2048];
				buffer_reader u(reference);

				// INIT / ACT
				reads_counter<buffer_reader> r(&u);

				r.remaining = 1023;

				// ACT
				r.read(data, 43);

				// ASSERT
				assert_equal(980u, r.remaining);

				// ACT
				r.read(data, 11);

				// ASSERT
				assert_equal(969u, r.remaining);
			}


			test( ReadsCounterSubtractsBytesSkippedFromRemainder )
			{
				// INIT
				unsigned char reference[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.";
				buffer_reader u(reference);

				// INIT / ACT
				reads_counter<buffer_reader> r(&u);

				r.remaining = 1023;

				// ACT
				r.skip(43);

				// ASSERT
				assert_equal(980u, r.remaining);

				// ACT
				r.skip(11);

				// ASSERT
				assert_equal(969u, r.remaining);
			}


			test( ReadsCounterReadsFromUnderlying )
			{
				// INIT
				unsigned char reference[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.";
				unsigned char data[2048];
				buffer_reader u(reference);

				// INIT / ACT
				reads_counter<buffer_reader> r(&u);
				
				r.remaining = 1023;

				// ACT
				r.read(data, 43);

				// ASSERT
				assert_equal(vector<unsigned char>(reference, reference + 43), vector<unsigned char>(data, data + 43));

				// ACT
				r.read(data, 11);

				// ASSERT
				assert_equal(vector<unsigned char>(reference + 43, reference + 43 + 11), vector<unsigned char>(data, data + 11));
			}


			test( ReadsCounterSkipsFromUnderlying )
			{
				// INIT
				unsigned char reference[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.";
				buffer_reader u(reference);

				// INIT / ACT
				reads_counter<buffer_reader> r(&u);

				r.remaining = 1023;

				// ACT
				r.skip(170u);

				// ASSERT
				assert_equal(170u, u.skipped);

				// ACT
				r.skip(11u);

				// ASSERT
				assert_equal(170u + 11u, u.skipped);
			}
		end_test_suite
	}
}
