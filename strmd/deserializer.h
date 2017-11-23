#pragma once

#include "type_traits.h"

namespace strmd
{
	template <typename StreamT>
	class deserializer
	{
	public:
		deserializer(StreamT &reader);

		template <typename T>
		void operator ()(T &data);

	private:
		template <typename T>
		void process_arithmetic(T &data);

		template <typename T>
		void process_container(T &data);

		template <typename T>
		void process_regular(T &data);

		void operator =(const deserializer &other);

	private:
		StreamT &_reader;

	private:
		template <bool enable>
		friend struct as_arithmetic;

		template <bool enable>
		friend struct as_container;

		template <bool enable>
		friend struct as_regular;
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

	template <typename KeyT, typename ValueT>
	struct container_reader< std::unordered_map<KeyT, ValueT> >
	{
		template <typename ArchiveT>
		void operator()(ArchiveT &archive, size_t count, std::unordered_map<KeyT, ValueT> &data)
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
	inline deserializer<StreamT>::deserializer(StreamT &reader)
		: _reader(reader)
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
	{	_reader.read(&data, sizeof(T));	}

	template <typename StreamT>
	template <typename T>
	inline void deserializer<StreamT>::process_container(T &data)
	{
		unsigned int size;
		container_reader<T> reader;

		(*this)(size);
		reader(*this, size, data);
	}

	template <typename StreamT>
	template <typename T>
	inline void deserializer<StreamT>::process_regular(T &data)
	{	serialize(*this, data);	}
}
