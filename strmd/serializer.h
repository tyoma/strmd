#pragma once

#include "packer.h"
#include "type_traits.h"

namespace strmd
{
	template <typename StreamT, typename PackerT = direct>
	class serializer
	{
	public:
		serializer(StreamT &writer);

		template <typename T>
		void operator ()(const T &data);

		template <typename T>
		void process_arithmetic(T data);

		template <typename ContainerT>
		void process_container(const ContainerT &data);

		template <typename T>
		void process_regular(const T &data);

	private:
		void operator =(const serializer &other);

	private:
		StreamT &_stream;
	};


	template <typename StreamT, typename PackerT>
	inline serializer<StreamT, PackerT>::serializer(StreamT &stream)
		: _stream(stream)
	{	}

	template <typename StreamT, typename PackerT>
	template <typename T>
	inline void serializer<StreamT, PackerT>::operator ()(const T &data)
	{
		as_arithmetic<is_arithmetic<T>::value>::process(*this, data);
		as_container<is_container<T>::value>::process(*this, data);
		as_regular<!is_arithmetic<T>::value && !is_container<T>::value>::process(*this, data);
	}

	template <typename StreamT, typename PackerT>
	template <typename T>
	inline void serializer<StreamT, PackerT>::process_arithmetic(T data)
	{	PackerT::pack(_stream, data);	}

	template <typename StreamT, typename PackerT>
	template <typename ContainerT>
	inline void serializer<StreamT, PackerT>::process_container(const ContainerT &data)
	{
		(*this)(static_cast<const unsigned int>(data.size()));
		for (typename ContainerT::const_iterator i = data.begin(); i != data.end(); ++i)
			(*this)(*i);
	}

	template <typename StreamT, typename PackerT>
	template <typename T>
	inline void serializer<StreamT, PackerT>::process_regular(const T &data)
	{	serialize(*this, const_cast<T &>(data));	}
}
