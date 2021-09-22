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
		writes_counter();

		void write(const void *data, std::size_t size);

		std::size_t written;
	};


	template <typename U>
	class fragment_reader
	{
	public:
		fragment_reader(U &underlying, std::size_t remaining);
		fragment_reader(fragment_reader &underlying, std::size_t remaining);

		void read(void *data, std::size_t size);
		void skip();

	private:
		U &skip_recursive_and_get_final(std::size_t d);

	private:
		U *_underlying_other;
		fragment_reader *_underlying_same;
		size_t _remaining;
	};

	template <typename U> struct get_fragment_reader {	typedef fragment_reader<U> type;	};
	template <typename U> struct get_fragment_reader< fragment_reader<U> > {	typedef fragment_reader<U> type;	};



	inline writes_counter::writes_counter()
		: written(0)
	{	}

	inline void writes_counter::write(const void * /*data*/, std::size_t size)
	{	written += size;	}


	template <typename U>
	inline fragment_reader<U>::fragment_reader(U &underlying, std::size_t remaining)
		: _underlying_other(&underlying), _underlying_same(0), _remaining(remaining)
	{	}

	template <typename U>
	inline fragment_reader<U>::fragment_reader(fragment_reader &underlying, std::size_t remaining)
		: _underlying_other(0), _underlying_same(&underlying), _remaining(remaining)
	{	}

	template <typename U>
	inline void fragment_reader<U>::read(void *data, std::size_t size)
	{	skip_recursive_and_get_final(size).read(data, size);	}

	template <typename U>
	inline void fragment_reader<U>::skip()
	{
		const std::size_t r = _remaining;
		skip_recursive_and_get_final(r).skip(r);
	}

	template <typename U>
	U &fragment_reader<U>::skip_recursive_and_get_final(std::size_t d)
	{
		fragment_reader *i = this;

		for (i->_remaining -= d; i->_underlying_same; i->_remaining -= d)
			i = i->_underlying_same;
		return *i->_underlying_other;
	}
}
