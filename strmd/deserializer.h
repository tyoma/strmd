//	Copyright (c) 2011-2018 by Artem A. Gevorkyan (gevorkyan.org)
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



	template <typename StreamT, typename PackerT>
	inline deserializer<StreamT, PackerT>::deserializer(StreamT &stream)
		: _stream(stream)
	{	}

	template <typename StreamT, typename PackerT>
	template <typename T>
	inline void deserializer<StreamT, PackerT>::operator ()(T &data)
	{
		as_arithmetic<is_arithmetic<T>::value>::process(*this, data);
		as_container<is_container<T>::value>::process(*this, data);
		as_regular<!is_arithmetic<T>::value && !is_container<T>::value>::process(*this, data);
	}

	template <typename StreamT, typename PackerT>
	template <typename T>
	inline void deserializer<StreamT, PackerT>::process_arithmetic(T &data)
	{	PackerT::unpack(_stream, data);	}

	template <typename StreamT, typename PackerT>
	template <typename T>
	inline size_t deserializer<StreamT, PackerT>::process_container(T &data)
	{
		unsigned int size;
		container_reader<T> reader;

		(*this)(size);
		reader(*this, size, data);
		return size;
	}

	template <typename StreamT, typename PackerT>
	template <typename T>
	inline void deserializer<StreamT, PackerT>::process_regular(T &data)
	{	serialize(*this, data);	}
}
