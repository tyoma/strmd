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

		std::size_t written() const;

	private:
		serializer(const serializer &other);
		void operator =(const serializer &rhs);

		template <typename T, typename CtxBinderT>
		void process(T data, const CtxBinderT &context, arithmetic_type_tag);

		template <typename T, typename CtxBinderT>
		void process(const T &container, const CtxBinderT &context, container_type_tag);

		template <typename T, typename CtxBinderT>
		void process(const T &data, const CtxBinderT &context, user_type_tag);

		template <typename T, typename CtxBinderT>
		void process(const T &data, const CtxBinderT &context, versioned_user_type_tag);

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
	{	process(data, context_binder<void>(), typename type_traits<T, is_versioned<T>::value>::category());	}

	template <typename StreamT, typename PackerT>
	template <typename T, typename Context>
	inline void serializer<StreamT, PackerT>::operator ()(const T &data, Context &context)
	{
		context_binder<Context &> ctx_binder(context);
		process(data, ctx_binder, typename type_traits<T, is_versioned<T>::value>::category());
	}

	template <typename StreamT, typename PackerT>
	inline std::size_t serializer<StreamT, PackerT>::written() const
	{	return _stream.written;	}

	template <typename StreamT, typename PackerT>
	template <typename T, typename CtxBinderT>
	inline void serializer<StreamT, PackerT>::process(T data, const CtxBinderT &/*context*/, arithmetic_type_tag)
	{	PackerT::pack(_stream, data);	}

	template <typename StreamT, typename PackerT>
	template <typename T, typename CtxBinderT>
	inline void serializer<StreamT, PackerT>::process(const T &container, const CtxBinderT &context, container_type_tag)
	{
		(*this)(static_cast<unsigned int>(container.size()));
		for (typename T::const_iterator i = container.begin(); i != container.end(); ++i)
			context.serialize_full(*this, *i);
	}

	template <typename StreamT, typename PackerT>
	template <typename T, typename CtxBinderT>
	inline void serializer<StreamT, PackerT>::process(const T &data, const CtxBinderT &context, user_type_tag)
	{	context.serialize_raw(*this, const_cast<T &>(data));	}


	template <typename StreamT, typename PackerT, typename T, typename CtxBinderT>
	inline void process_versioned(serializer<StreamT, PackerT> &s, const T &data, const CtxBinderT &context)
	{
		writes_counter counter;
		serializer<writes_counter, PackerT> sc(counter);

		context.serialize_raw(sc, const_cast<T &>(data), version<T>::value);
		s(version_header::make(version<T>::value, counter.written));
		context.serialize_raw(s, const_cast<T &>(data), version<T>::value);
	}

	template <typename PackerT, typename T, typename CtxBinderT>
	inline void process_versioned(serializer<writes_counter, PackerT> &s, const T &data, const CtxBinderT &context)
	{
		const std::size_t written = s.written();

		// Header is normally serialized before the object, but since we're just counting bytes - we don't care.
		context.serialize_raw(s, const_cast<T &>(data), version<T>::value);
		s(version_header::make(version<T>::value, s.written() - written));
	}

	template <typename StreamT, typename PackerT>
	template <typename T, typename CtxBinderT>
	inline void serializer<StreamT, PackerT>::process(const T &data, const CtxBinderT &context, versioned_user_type_tag)
	{	process_versioned(*this, data, context);	}
}
