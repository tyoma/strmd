//	Copyright (c) 2011-2020 by Artem A. Gevorkyan (gevorkyan.org)
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.

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
