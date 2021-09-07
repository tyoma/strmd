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
		void read(void *data, std::size_t size);
		void skip(std::size_t size);

		U *underlying;
		size_t remaining;
	};



	inline void writes_counter::write(const void * /*data*/, std::size_t size)
	{	written += size;	}


	template <typename U>
	inline void reads_counter<U>::read(void *data, std::size_t size)
	{
		underlying->read(data, size);
		remaining -= size;
	}

	template <typename U>
	inline void reads_counter<U>::skip(std::size_t size)
	{
		underlying->skip(size);
		remaining -= size;
	}
}
