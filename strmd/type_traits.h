//	Copyright (c) 2011-2015 by Artem A. Gevorkyan (gevorkyan.org)
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

#include <string>
#include <unordered_map>
#include <vector>

namespace std { namespace tr1 { } using namespace tr1; }

namespace strmd
{
	template <typename T> struct is_arithmetic { static const bool value = false; };
	template <> struct is_arithmetic<char> { static const bool value = true; };
	template <> struct is_arithmetic<unsigned char> { static const bool value = true; };
	template <> struct is_arithmetic<short> { static const bool value = true; };
	template <> struct is_arithmetic<unsigned short> { static const bool value = true; };
	template <> struct is_arithmetic<int> { static const bool value = true; };
	template <> struct is_arithmetic<unsigned int> { static const bool value = true; };
	template <> struct is_arithmetic<long> { static const bool value = true; };
	template <> struct is_arithmetic<unsigned long> { static const bool value = true; };
	template <> struct is_arithmetic<long long> { static const bool value = true; };
	template <> struct is_arithmetic<unsigned long long> { static const bool value = true; };
	template <> struct is_arithmetic<float> { static const bool value = true; };
	template <> struct is_arithmetic<double> { static const bool value = true; };


	template <typename T> struct is_container { static const bool value = false; };
	template <typename T> struct is_container< std::basic_string<T> > { static const bool value = true; };
	template <typename T> struct is_container< std::vector<T> > { static const bool value = true; };
	template <typename KeyT, typename ValueT> struct is_container< std::unordered_map<KeyT, ValueT> > { static const bool value = true; };
}
