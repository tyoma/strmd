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

namespace strmd
{
	template <typename T> struct is_arithmetic { static const bool value = false; };
	template <> struct is_arithmetic<char> { static const bool value = true; };
	template <> struct is_arithmetic<wchar_t> { static const bool value = true; };
	template <> struct is_arithmetic<unsigned char> { static const bool value = true; };
	template <> struct is_arithmetic<short> { static const bool value = true; };
	template <> struct is_arithmetic<unsigned short> { static const bool value = true; };
	template <> struct is_arithmetic<int> { static const bool value = true; };
	template <> struct is_arithmetic<unsigned int> { static const bool value = true; };
//	template <> struct is_arithmetic<long> { static const bool value = true; };
//	template <> struct is_arithmetic<unsigned long> { static const bool value = true; };
	template <> struct is_arithmetic<long long> { static const bool value = true; };
	template <> struct is_arithmetic<unsigned long long> { static const bool value = true; };
	template <> struct is_arithmetic<float> { static const bool value = true; };
	template <> struct is_arithmetic<double> { static const bool value = true; };

	template <typename T> struct remove_sign { typedef T type; };
	template <> struct remove_sign<signed char> { typedef unsigned char type; };
	template <> struct remove_sign<signed short> { typedef unsigned short type; };
	template <> struct remove_sign<signed int> { typedef unsigned int type; };
	template <> struct remove_sign<signed long> { typedef unsigned long type; };
	template <> struct remove_sign<signed long long> { typedef unsigned long long type; };



	template <typename T> struct is_container { static const bool value = false; };
	template <typename T> struct is_container< std::basic_string<T> > { static const bool value = true; };
	template <typename T> struct is_container< std::vector<T> > { static const bool value = true; };
	template <typename KeyT, typename ValueT, typename CompT> struct is_container< std::unordered_map<KeyT, ValueT, CompT> > { static const bool value = true; };


	template <bool enable>
	struct as_arithmetic
	{
		template <typename ArchiveT, typename T> static void process(ArchiveT &, T &) { }
	};

	template <>
	struct as_arithmetic<true>
	{
		template <typename ArchiveT, typename T> static void process(ArchiveT &a, T &data) { a.process_arithmetic(data); }
	};


	template <bool enable>
	struct as_container
	{
		template <typename ArchiveT, typename T> static void process(ArchiveT &, T &) { }
	};

	template <>
	struct as_container<true>
	{
		template <typename ArchiveT, typename T> static void process(ArchiveT &a, T &data) { a.process_container(data); }
	};


	template <bool enable>
	struct as_regular
	{
		template <typename ArchiveT, typename T> static void process(ArchiveT &, T &) { }
	};

	template <>
	struct as_regular<true>
	{
		template <typename ArchiveT, typename T> static void process(ArchiveT &a, T &data) { a.process_regular(data); }
	};


	template <typename ArchiveT, typename T1, typename T2>
	inline void serialize(ArchiveT &archive, std::pair<T1, T2> &data)
	{
		archive(data.first);
		archive(data.second);
	}
}
