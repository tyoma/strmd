#pragma once

#include <strmd/type_traits.h>

#include <cstring>
#include <ut/assert.h>
#include <vector>

namespace strmd
{
	namespace tests
	{
		enum type_category { is_arithmetic, is_container, is_user_type };

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


		class vector_reader
		{
		public:
			vector_reader(const std::vector<unsigned char> &buffer)
				: _buffer(buffer), _ptr(0)
			{	}

			void read(void *data, size_t size)
			{
				assert_is_true(size <= _buffer.size() - _ptr);
				if (size)
					std::memcpy(data, &_buffer[_ptr], size);
				_ptr += size;
			}

		private:
			void operator =(const vector_reader &rhs);

		private:
			const std::vector<unsigned char> &_buffer;
			size_t _ptr;
		};



		inline type_category category(arithmetic_type_tag)
		{	return is_arithmetic;	}

		inline type_category category(container_type_tag)
		{	return is_container;	}

		inline type_category category(user_type_tag)
		{	return is_user_type;	}
	}
}
