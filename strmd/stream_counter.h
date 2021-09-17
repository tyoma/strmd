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

#include <cstddef>

namespace strmd
{
	struct writes_counter
	{
		void write(const void *data, std::size_t size);

		std::size_t written;
	};


	template <typename U>
	struct reads_counter
	{
		reads_counter(U *underlying);
		reads_counter(reads_counter *underlying);

		void read(void *data, std::size_t size);
		void skip(std::size_t size);

		U *underlying_other;
		reads_counter *underlying_same;
		size_t remaining;
	};

	template <typename U> struct nested_reads_counter {	typedef reads_counter<U> type;	};
	template <typename U> struct nested_reads_counter< reads_counter<U> > {	typedef reads_counter<U> type;	};



	inline void writes_counter::write(const void * /*data*/, std::size_t size)
	{	written += size;	}


	template <typename U>
	inline reads_counter<U>::reads_counter(U *underlying)
		: underlying_other(underlying), underlying_same(0)
	{	}

	template <typename U>
	inline reads_counter<U>::reads_counter(reads_counter *underlying)
		: underlying_other(0), underlying_same(underlying)
	{	}

	template <typename U>
	inline void reads_counter<U>::read(void *data, std::size_t size)
	{
		underlying_other ? underlying_other->read(data, size) : underlying_same->read(data, size);
		remaining -= size;
	}

	template <typename U>
	inline void reads_counter<U>::skip(std::size_t size)
	{
		underlying_other ? underlying_other->skip(size) : underlying_same->skip(size);
		remaining -= size;
	}
}
