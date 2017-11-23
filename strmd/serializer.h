#pragma once

#include "type_traits.h"

namespace std { namespace tr1 { } using namespace tr1; }

namespace strmd
{
	template <typename StreamT>
	class serializer
	{
	public:
		serializer(StreamT &writer);

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
		StreamT &_writer;
	};


	template <bool enable>
	struct write_as_arithmetic
	{
		template <typename ArchiveT, typename T> static void write(ArchiveT &, const T &) { }
	};

	template <>
	struct write_as_arithmetic<true>
	{
		template <typename ArchiveT, typename T> static void write(ArchiveT &s, T data) { s.write_arithmetic(data); }
	};


	template <bool enable>
	struct write_as_container
	{
		template <typename ArchiveT, typename T> static void write(ArchiveT &, const T &) { }
	};

	template <>
	struct write_as_container<true>
	{
		template <typename ArchiveT, typename T> static void write(ArchiveT &s, const T &data) { s.write_container(data); }
	};


	template <bool enable>
	struct write_as_regular
	{
		template <typename ArchiveT, typename T> static void write(ArchiveT &, const T &) { }
	};

	template <>
	struct write_as_regular<true>
	{
		template <typename ArchiveT, typename T> static void write(ArchiveT &s, const T &data) { s.write_regular(data); }
	};



	template <typename StreamT>
	inline serializer<StreamT>::serializer(StreamT &writer)
		: _writer(writer)
	{	}

	template <typename StreamT>
	template <typename T>
	inline void serializer<StreamT>::operator ()(const T &data)
	{
		write_as_arithmetic<is_arithmetic<T>::value>::write(*this, data);
		write_as_container<is_container<T>::value>::write(*this, data);
		write_as_regular<!is_arithmetic<T>::value && !is_container<T>::value>::write(*this, data);
	}

	template <typename StreamT>
	template <typename T>
	inline void serializer<StreamT>::write_arithmetic(T data)
	{	_writer.write(&data, sizeof(data));	}

	template <typename StreamT>
	template <typename ContainerT>
	inline void serializer<StreamT>::write_container(const ContainerT &data)
	{
		(*this)(static_cast<const unsigned int>(data.size()));
		for (typename ContainerT::const_iterator i = data.begin(); i != data.end(); ++i)
			(*this)(*i);
	}

	template <typename StreamT>
	template <typename T>
	inline void serializer<StreamT>::write_regular(const T &data)
	{	serialize(*this, const_cast<T &>(data));	}


	template <typename ArchiveT, typename T1, typename T2>
	inline void serialize(ArchiveT &archive, std::pair<T1, T2> &data)
	{
		archive(data.first);
		archive(data.second);
	}
}
