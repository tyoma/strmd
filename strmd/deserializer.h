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

		void operator =(const deserializer &other);

	private:
		StreamT &_reader;

	private:
		template <bool enable>
		friend struct process_as_arithmetic;

		template <bool enable>
		friend struct process_as_container;

		template <bool enable>
		friend struct process_as_regular;
	};



	template <typename StreamT>
	inline deserializer<StreamT>::deserializer(StreamT &reader)
		: _reader(reader)
	{	}

	template <typename StreamT>
	template <typename T>
	inline void deserializer<StreamT>::operator ()(T &data)
	{
		process_as_arithmetic<is_arithmetic<T>::value>::process(*this, data);
		process_as_container<is_container<T>::value>::process(*this, data);
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
		typename T::value_type value;

		(*this)(size);
		while (size--)
		{
			(*this)(value);
			data.push_back(value);
		}
	}
}
