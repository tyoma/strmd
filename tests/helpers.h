#pragma once

#include <cstring>
#include <ut/assert.h>
#include <vector>

namespace strmd
{
	namespace tests
	{
		class vector_writer
		{
		public:
			vector_writer(std::vector<unsigned char> &buffer)
				: _buffer(buffer)
			{	}

			void write(const void *data, size_t size)
			{
				_buffer.insert(_buffer.end(), static_cast<const unsigned char *>(data),
					static_cast<const unsigned char *>(data)+size);
			}

		private:
			void operator =(const vector_writer &other);

		private:
			std::vector<unsigned char> &_buffer;
		};
		
		class buffer_reader
		{
		public:
			template <size_t N>
			buffer_reader(unsigned char (&p)[N])
				: _ptr(p), _remaining(N)
			{	}

			void read(void *data, size_t size)
			{
				assert_is_true(size <= _remaining);
				std::memcpy(data, _ptr, size);
				_ptr += size;
				_remaining -= size;
			}

		private:
			const unsigned char *_ptr;
			size_t _remaining;
		};
	}
}
