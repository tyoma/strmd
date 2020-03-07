//	Copyright (c) 2011-2020 by Artem A. Gevorkyan (gevorkyan.org)
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
	template <> struct is_arithmetic<long> { static const bool value = true; };
	template <> struct is_arithmetic<unsigned long> { static const bool value = true; };
	template <> struct is_arithmetic<long long> { static const bool value = true; };
	template <> struct is_arithmetic<unsigned long long> { static const bool value = true; };
	template <> struct is_arithmetic<float> { static const bool value = true; };
	template <> struct is_arithmetic<double> { static const bool value = true; };


	template <typename T> struct is_char { static const bool value = false; };
	template <> struct is_char<char> { static const bool value = true; };
	template <> struct is_char<signed char> { static const bool value = true; };
	template <> struct is_char<unsigned char> { static const bool value = true; };


	template <typename T> struct make_unsigned { typedef T type; static const bool was_signed = false; };
	template <> struct make_unsigned<char> { typedef unsigned char type; static const bool was_signed = true; };
	template <> struct make_unsigned<signed char> { typedef unsigned char type; static const bool was_signed = true; };
	template <> struct make_unsigned<signed short> { typedef unsigned short type; static const bool was_signed = true; };
	template <> struct make_unsigned<signed int> { typedef unsigned int type; static const bool was_signed = true; };
	template <> struct make_unsigned<signed long> { typedef unsigned long type; static const bool was_signed = true; };
	template <> struct make_unsigned<signed long long> { typedef unsigned long long type; static const bool was_signed = true; };


	template <typename T> struct is_signed { static const bool value = make_unsigned<T>::was_signed; };


	template <typename T> struct remove_const { typedef T type; };
	template <typename T> struct remove_const<const T> { typedef typename remove_const<T>::type type; };


	template <typename T> struct container_traits { static const bool is_container = false; };


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
		template <typename ArchiveT, typename T, typename ContextT> static void process(ArchiveT &, T &, ContextT &) { }
	};

	template <>
	struct as_container<true>
	{
		template <typename ArchiveT, typename T> static void process(ArchiveT &a, T &data) { a.process_container(data); }
		template <typename ArchiveT, typename T, typename ContextT> static void process(ArchiveT &a, T &data, ContextT &context) { a.process_container(data, context); }
	};


	template <bool enable>
	struct as_regular
	{
		template <typename ArchiveT, typename T> static void process(ArchiveT &, T &) { }
		template <typename ArchiveT, typename T, typename ContextT> static void process(ArchiveT &, T &, ContextT &) { }
	};

	template <>
	struct as_regular<true>
	{
		template <typename ArchiveT, typename T> static void process(ArchiveT &a, T &data) { a.process_regular(data); }
		template <typename ArchiveT, typename T, typename ContextT> static void process(ArchiveT &a, T &data, ContextT &context) { a.process_regular(data, context); }
	};
}
