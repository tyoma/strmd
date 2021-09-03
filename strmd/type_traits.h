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

namespace strmd
{
	enum {	unversioned = -1	};

	struct arithmetic_type_tag {};
	struct container_type_tag {};
	struct user_type_tag {};
	struct versioned_user_type_tag {};


	template <typename T> struct version {	enum {	value = unversioned	};	};


	template <typename T, bool versioned = false> struct type_traits { typedef user_type_tag /*default*/ category; };
	template <typename T> struct type_traits<T, true> { typedef versioned_user_type_tag /*default*/ category; };
	template <> struct type_traits<char> { typedef arithmetic_type_tag category; };
	template <> struct type_traits<wchar_t> { typedef arithmetic_type_tag category; };
	template <> struct type_traits<unsigned char> { typedef arithmetic_type_tag category; };
	template <> struct type_traits<short> { typedef arithmetic_type_tag category; };
	template <> struct type_traits<unsigned short> { typedef arithmetic_type_tag category; };
	template <> struct type_traits<int> { typedef arithmetic_type_tag category; };
	template <> struct type_traits<unsigned int> { typedef arithmetic_type_tag category; };
	template <> struct type_traits<long> { typedef arithmetic_type_tag category; };
	template <> struct type_traits<unsigned long> { typedef arithmetic_type_tag category; };
	template <> struct type_traits<long long> { typedef arithmetic_type_tag category; };
	template <> struct type_traits<unsigned long long> { typedef arithmetic_type_tag category; };
	template <> struct type_traits<float> { typedef arithmetic_type_tag category; };
	template <> struct type_traits<double> { typedef arithmetic_type_tag category; };


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
	template <typename T> struct is_versioned {	static const bool value = version<T>::value != static_cast<int>(unversioned);	};


	template <typename T> struct remove_const { typedef T type; };
	template <typename T> struct remove_const<const T> { typedef typename remove_const<T>::type type; };
}
