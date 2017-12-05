#pragma once

#include "type_traits.h"

namespace strmd
{
	template <typename StreamT>
	class deserializer
	{
	public:
		deserializer(StreamT &stream);

		template <typename T>
		void operator ()(T &data);

		template <typename T>
		void process_arithmetic(T &data);

		template <typename T>
		size_t process_container(T &data);

		template <typename T>
		void process_regular(T &data);

	private:
		void operator =(const deserializer &other);

	private:
		StreamT &_stream;
	};

	template <typename ContainerT>
	struct container_reader
	{
		template <typename ArchiveT>
		void operator()(ArchiveT &archive, size_t count, ContainerT &data)
		{
			typename ContainerT::value_type value;

			data.clear();
			while (count--)
			{
				archive(value);
				data.push_back(value);
			}
		}
	};

	template <typename KeyT, typename ValueT, typename CompT>
	struct container_reader< std::unordered_map<KeyT, ValueT, CompT> >
	{
		template <typename ArchiveT>
		void operator()(ArchiveT &archive, size_t count, std::unordered_map<KeyT, ValueT, CompT> &data)
		{
			std::pair<KeyT, ValueT> value;

			data.clear();
			while (count--)
			{
				archive(value);
				data.insert(value);
			}
		}
	};



	template <typename StreamT>
	inline deserializer<StreamT>::deserializer(StreamT &stream)
		: _stream(stream)
	{	}

	template <typename StreamT>
	template <typename T>
	inline void deserializer<StreamT>::operator ()(T &data)
	{
		as_arithmetic<is_arithmetic<T>::value>::process(*this, data);
		as_container<is_container<T>::value>::process(*this, data);
		as_regular<!is_arithmetic<T>::value && !is_container<T>::value>::process(*this, data);
	}

	template <typename StreamT>
	template <typename T>
	inline void deserializer<StreamT>::process_arithmetic(T &data)
	{	_stream.read(&data, sizeof(T));	}

	template <typename StreamT>
	template <typename T>
	inline size_t deserializer<StreamT>::process_container(T &data)
	{
		unsigned int size;
		container_reader<T> reader;

		(*this)(size);
		reader(*this, size, data);
		return size;
	}

	template <typename StreamT>
	template <typename T>
	inline void deserializer<StreamT>::process_regular(T &data)
	{	serialize(*this, data);	}
}
