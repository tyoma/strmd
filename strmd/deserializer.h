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
	class deserializer
	{
	public:
		deserializer(StreamT &stream);

		template <typename T>
		void operator ()(T &data);

		template <typename T, typename ContextT>
		void operator ()(T &data, ContextT &context);

	private:
		void operator =(const deserializer &other);

		template <typename T>
		void process(T &data, arithmetic_type_tag);

		template <typename T, typename ContextT>
		void process(T &data, ContextT &context, arithmetic_type_tag);

		template <typename T>
		void process(T &container, container_type_tag);

		template <typename T, typename ContextT>
		void process(T &container, ContextT &context, container_type_tag);

		template <typename T>
		void process(T &data, user_type_tag);

		template <typename T, typename ContextT>
		void process(T &data, ContextT &context, user_type_tag);

	private:
		StreamT &_stream;
	};



	template <typename StreamT, typename PackerT>
	inline deserializer<StreamT, PackerT>::deserializer(StreamT &stream)
		: _stream(stream)
	{	}

	template <typename StreamT, typename PackerT>
	template <typename T>
	inline void deserializer<StreamT, PackerT>::operator ()(T &data)
	{	process(data, typename type_traits<T>::category());	}

	template <typename StreamT, typename PackerT>
	template <typename T, typename ContextT>
	inline void deserializer<StreamT, PackerT>::operator ()(T &data, ContextT &context)
	{	process(data, context, typename type_traits<T>::category());	}

	template <typename StreamT, typename PackerT>
	template <typename T>
	inline void deserializer<StreamT, PackerT>::process(T &data, arithmetic_type_tag)
	{	PackerT::unpack(_stream, data);	}

	template <typename StreamT, typename PackerT>
	template <typename T, typename ContextT>
	inline void deserializer<StreamT, PackerT>::process(T &data, ContextT &/*context*/, arithmetic_type_tag)
	{	PackerT::unpack(_stream, data);	}

	template <typename StreamT, typename PackerT>
	template <typename T>
	inline void deserializer<StreamT, PackerT>::process(T &container, container_type_tag)
	{
		unsigned int count;
		typename type_traits<T>::item_reader_type reader;

		(*this)(count);
		reader.prepare(container, count);
		while (count--)
			reader.read_item(*this, container);
		reader.complete(container);
	}

	template <typename StreamT, typename PackerT>
	template <typename T, typename ContextT>
	inline void deserializer<StreamT, PackerT>::process(T &container, ContextT &context, container_type_tag)
	{
		unsigned int count;
		typename type_traits<T>::item_reader_type reader;

		(*this)(count);
		reader.prepare(container, count);
		while (count--)
			reader.read_item(*this, container, context);
		reader.complete(container);
	}

	template <typename StreamT, typename PackerT>
	template <typename T>
	inline void deserializer<StreamT, PackerT>::process(T &data, user_type_tag)
	{	serialize(*this, data, 0u);	}

	template <typename StreamT, typename PackerT>
	template <typename T, typename ContextT>
	inline void deserializer<StreamT, PackerT>::process(T &data, ContextT &context, user_type_tag)
	{	serialize(*this, data, 0u, context);	}
}
