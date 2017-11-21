#pragma once

#include "type_traits.h"

namespace std { namespace tr1 { } using namespace tr1; }

namespace strmd
{
	class vector_writer
	{
	public:
		vector_writer(std::vector<unsigned char> &buffer);

		void write(const void *buffer, size_t size);

	private:
		void operator =(const vector_writer &other);

	private:
		std::vector<unsigned char> &_buffer;
	};

	class serializer
	{
	public:
		serializer(vector_writer &writer);

		template <typename T>
		void operator ()(const T &data);

		template <typename T>
		void write_arithmetic(T data);

		template <typename ContainerT>
		void write_container(const ContainerT &data);

		template <typename T>
		void write_regular(const T &data);

	private:
		void operator =(const serializer &other);

	private:
		vector_writer &_writer;
	};


	template <bool enable>
	struct write_as_arithmetic
	{
		template <typename T> static void write(serializer &, const T &) { }
	};

	template <>
	struct write_as_arithmetic<true>
	{
		template <typename T> static void write(serializer &s, T data) { s.write_arithmetic(data); }
	};


	template <bool enable>
	struct write_as_container
	{
		template <typename T> static void write(serializer &, const T &) { }
	};

	template <>
	struct write_as_container<true>
	{
		template <typename T> static void write(serializer &s, const T &data) { s.write_container(data); }
	};


	template <bool enable>
	struct write_as_regular
	{
		template <typename T> static void write(serializer &, const T &) { }
	};

	template <>
	struct write_as_regular<true>
	{
		template <typename T> static void write(serializer &s, const T &data) { s.write_regular(data); }
	};



	inline vector_writer::vector_writer(std::vector<unsigned char> &buffer)
		: _buffer(buffer)
	{	}

	inline void vector_writer::write(const void *data, size_t size)
	{
		_buffer.insert(_buffer.end(), static_cast<const unsigned char *>(data),
			static_cast<const unsigned char *>(data) + size);
	}


	inline serializer::serializer(vector_writer &writer)
		: _writer(writer)
	{	}

	template <typename T>
	inline void serializer::operator ()(const T &data)
	{
		write_as_arithmetic<is_arithmetic<T>::value>::write(*this, data);
		write_as_container<is_container<T>::value>::write(*this, data);
		write_as_regular<!is_arithmetic<T>::value && !is_container<T>::value>::write(*this, data);
	}

	template <typename T>
	inline void serializer::write_arithmetic(T data)
	{	_writer.write(&data, sizeof(data));	}

	template <typename ContainerT>
	inline void serializer::write_container(const ContainerT &data)
	{
		(*this)(static_cast<const unsigned int>(data.size()));
		for (typename ContainerT::const_iterator i = data.begin(); i != data.end(); ++i)
			(*this)(*i);
	}

	template <typename T>
	inline void serializer::write_regular(const T &data)
	{	serialize(*this, const_cast<T &>(data));	}


	template <typename ArchiveT, typename T1, typename T2>
	inline void serialize(ArchiveT &archive, std::pair<T1, T2> &data)
	{
		archive(data.first);
		archive(data.second);
	}
}
