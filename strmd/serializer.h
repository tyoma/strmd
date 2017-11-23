#pragma once

#include "type_traits.h"

namespace strmd
{
	template <typename StreamT>
	class serializer
	{
	public:
		serializer(StreamT &writer);

		template <typename T>
		void operator ()(const T &data);

	private:
		template <typename T>
		void process_arithmetic(T data);

		template <typename ContainerT>
		void process_container(const ContainerT &data);

		template <typename T>
		void process_regular(const T &data);

		void operator =(const serializer &other);

	private:
		StreamT &_writer;

	private:
		template <bool enable>
		friend struct as_arithmetic;

		template <bool enable>
		friend struct as_container;

		template <bool enable>
		friend struct as_regular;
	};


	template <typename StreamT>
	inline serializer<StreamT>::serializer(StreamT &writer)
		: _writer(writer)
	{	}

	template <typename StreamT>
	template <typename T>
	inline void serializer<StreamT>::operator ()(const T &data)
	{
		as_arithmetic<is_arithmetic<T>::value>::process(*this, data);
		as_container<is_container<T>::value>::process(*this, data);
		as_regular<!is_arithmetic<T>::value && !is_container<T>::value>::process(*this, data);
	}

	template <typename StreamT>
	template <typename T>
	inline void serializer<StreamT>::process_arithmetic(T data)
	{	_writer.write(&data, sizeof(data));	}

	template <typename StreamT>
	template <typename ContainerT>
	inline void serializer<StreamT>::process_container(const ContainerT &data)
	{
		(*this)(static_cast<const unsigned int>(data.size()));
		for (typename ContainerT::const_iterator i = data.begin(); i != data.end(); ++i)
			(*this)(*i);
	}

	template <typename StreamT>
	template <typename T>
	inline void serializer<StreamT>::process_regular(const T &data)
	{	serialize(*this, const_cast<T &>(data));	}
}
