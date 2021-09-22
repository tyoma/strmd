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
#include "context.h"
#include "packer.h"
#include "stream_special.h"
#include "version.h"

#ifdef _MSC_VER
	#pragma warning(disable: 4127)
#endif

namespace strmd
{
	template <typename StreamT, typename PackerT = direct, int static_version = unversioned>
	class deserializer
	{
	public:
		deserializer(StreamT &stream);

		template <typename T>
		void operator ()(T &data);

		template <typename T, typename ContextT>
		void operator ()(T &data, ContextT &context);

	private:
		deserializer(const deserializer &other); // TODO: implement copying if StreamT allows it.
		void operator =(const deserializer &rhs);

		template <typename T, typename CtxBinderT>
		void process(T &data, const CtxBinderT &context, arithmetic_type_tag);

		template <typename T, typename CtxBinderT>
		void process(T &container, const CtxBinderT &context, container_type_tag);

		template <typename T, typename CtxBinderT>
		void process(T &data, const CtxBinderT &context, user_type_tag);

		template <typename T, typename CtxBinderT>
		void process(T &data, const CtxBinderT &context, versioned_user_type_tag);

	private:
		StreamT &_stream;
	};



	template <typename StreamT, typename PackerT, int static_version>
	inline deserializer<StreamT, PackerT, static_version>::deserializer(StreamT &stream)
		: _stream(stream)
	{	}

	template <typename StreamT, typename PackerT, int static_version>
	template <typename T>
	inline void deserializer<StreamT, PackerT, static_version>::operator ()(T &data)
	{	process(data, context_binder<void>(), typename type_traits<T, is_versioned<T>::value>::category());	}

	template <typename StreamT, typename PackerT, int static_version>
	template <typename T, typename ContextT>
	inline void deserializer<StreamT, PackerT, static_version>::operator ()(T &data, ContextT &context)
	{
		context_binder<ContextT &> ctx_binder(context);
		process(data, ctx_binder, typename type_traits<T, is_versioned<T>::value>::category());
	}

	template <typename StreamT, typename PackerT, int static_version>
	template <typename T, typename CtxBinderT>
	inline void deserializer<StreamT, PackerT, static_version>::process(T &data, const CtxBinderT &/*context*/, arithmetic_type_tag)
	{	PackerT::unpack(_stream, data);	}

	template <typename StreamT, typename PackerT, int static_version>
	template <typename T, typename CtxBinderT>
	inline void deserializer<StreamT, PackerT, static_version>::process(T &container, const CtxBinderT &context, container_type_tag)
	{
		unsigned int count;
		typename type_traits<T>::item_reader_type reader;

		(*this)(count);
		context.prepare(reader, container, count);
		while (count--)
			context.read_item(reader, *this, container);
		context.complete(reader, container);
	}

	template <typename StreamT, typename PackerT, int static_version>
	template <typename T, typename CtxBinderT>
	inline void deserializer<StreamT, PackerT, static_version>::process(T &data, const CtxBinderT &context, user_type_tag)
	{	context.serialize_raw(*this, data);	}

	template <typename StreamT, typename PackerT, int static_version>
	template <typename T, typename CtxBinderT>
	inline void deserializer<StreamT, PackerT, static_version>::process(T &data, const CtxBinderT &context,
		versioned_user_type_tag)
	{
		typedef typename get_fragment_reader<StreamT>::type fragment_reader_t;

		if (static_version == unversioned)
		{
			version_header h;

			(*this)(h);

			fragment_reader_t r(_stream, h.size);
			deserializer<fragment_reader_t, PackerT> d(r);

			context.serialize_raw(d, data, h.version);
			r.skip();
			return;
		}
		context.serialize_raw(*this, data, static_cast<unsigned int>(static_version));
	}
}
