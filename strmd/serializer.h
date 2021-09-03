//	Copyright (c) 2011-2021 by Artem A. Gevorkyan (gevorkyan.org)
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

#include "container.h"
#include "packer.h"

namespace strmd
{
	template <typename StreamT, typename PackerT = direct>
	class serializer
	{
	public:
		serializer(StreamT &writer);

		template <typename T>
		void operator ()(const T &data);

		template <typename T, typename ContextT>
		void operator ()(const T &data, ContextT &context);

	private:
		void operator =(const serializer &other);

		template <typename T>
		void process(T data, arithmetic_type_tag);

		template <typename T, typename ContextT>
		void process(T data, ContextT &context, arithmetic_type_tag);

		template <typename T>
		void process(const T &container, container_type_tag);

		template <typename T, typename ContextT>
		void process(const T &container, ContextT &context, container_type_tag);

		template <typename T>
		void process(const T &data, user_type_tag);

		template <typename T, typename ContextT>
		void process(const T &data, ContextT &context, user_type_tag);

		template <typename T>
		void process(const T &data, versioned_user_type_tag);

		template <typename T, typename ContextT>
		void process(const T &data, ContextT &context, versioned_user_type_tag);

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
	{	process(data, typename type_traits<T, is_versioned<T>::value>::category());	}

	template <typename StreamT, typename PackerT>
	template <typename T, typename ContextT>
	inline void serializer<StreamT, PackerT>::operator ()(const T &data, ContextT &context)
	{	process(data, context, typename type_traits<T, is_versioned<T>::value>::category());	}

	template <typename StreamT, typename PackerT>
	template <typename T>
	inline void serializer<StreamT, PackerT>::process(T data, arithmetic_type_tag)
	{	PackerT::pack(_stream, data);	}

	template <typename StreamT, typename PackerT>
	template <typename T, typename ContextT>
	inline void serializer<StreamT, PackerT>::process(T data, ContextT &/*context*/, arithmetic_type_tag)
	{	PackerT::pack(_stream, data);	}

	template <typename StreamT, typename PackerT>
	template <typename T>
	inline void serializer<StreamT, PackerT>::process(const T &container, container_type_tag)
	{
		(*this)(static_cast<unsigned int>(container.size()));
		for (typename T::const_iterator i = container.begin(); i != container.end(); ++i)
			(*this)(*i);
	}

	template <typename StreamT, typename PackerT>
	template <typename T, typename ContextT>
	inline void serializer<StreamT, PackerT>::process(const T &container, ContextT &context, container_type_tag)
	{
		(*this)(static_cast<unsigned int>(container.size()));
		for (typename T::const_iterator i = container.begin(); i != container.end(); ++i)
			(*this)(*i, context);
	}

	template <typename StreamT, typename PackerT>
	template <typename T>
	inline void serializer<StreamT, PackerT>::process(const T &data, user_type_tag)
	{	serialize(*this, const_cast<T &>(data));	}

	template <typename StreamT, typename PackerT>
	template <typename T, typename ContextT>
	inline void serializer<StreamT, PackerT>::process(const T &data, ContextT &context, user_type_tag)
	{	serialize(*this, const_cast<T &>(data), context);	}


	struct counting_writer
	{
		void write(const void *, size_t size)
		{	written += size;	}

		size_t written;
	};

	template <typename StreamT, typename PackerT>
	template <typename T>
	inline void serializer<StreamT, PackerT>::process(const T &data, versioned_user_type_tag)
	{
		counting_writer counter = { };
		serializer<counting_writer, PackerT> s(counter);

		(*this)(static_cast<unsigned int>(version<T>::value));
		serialize(s, const_cast<T &>(data), static_cast<unsigned int>(version<T>::value));
		(*this)(counter.written);
		serialize(*this, const_cast<T &>(data), static_cast<unsigned int>(version<T>::value));
	}

	template <typename StreamT, typename PackerT>
	template <typename T, typename ContextT>
	inline void serializer<StreamT, PackerT>::process(const T &data, ContextT &context, versioned_user_type_tag)
	{
		counting_writer counter = { };
		serializer<counting_writer, PackerT> s(counter);

		(*this)(static_cast<unsigned int>(version<T>::value));
		serialize(s, const_cast<T &>(data), context, static_cast<unsigned int>(version<T>::value));
		(*this)(counter.written);
		serialize(*this, const_cast<T &>(data), context, static_cast<unsigned int>(version<T>::value));
	}
}
