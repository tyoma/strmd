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

#include "utility.h"

#include <deque>
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace strmd
{
	struct container_reader_base
	{
		template <typename ContainerT>
		void prepare(ContainerT &container, size_t /*count*/) const
		{	container.clear();	}

		template <typename ContainerT>
		void complete(ContainerT &/*container*/) const
		{	}
	};

	struct sequence_container_reader : container_reader_base
	{
		template <typename ArchiveT, typename ContainerT>
		void read_item(ArchiveT &archive, ContainerT &container) const
		{
			typename ContainerT::value_type value;
			archive(value), container.push_back(value);
		}

		template <typename ArchiveT, typename ContainerT, typename ContextT>
		void read_item(ArchiveT &archive, ContainerT &container, ContextT &context) const
		{
			typename ContainerT::value_type value;
			archive(value, context), container.push_back(value);
		}
	};

	struct associative_container_reader : container_reader_base
	{
		template <typename ArchiveT, typename ContainerT>
		void read_item(ArchiveT &archive, ContainerT &container) const
		{
			typename remove_const<typename ContainerT::value_type>::type value;
			archive(value), container.insert(value);
		}

		template <typename ArchiveT, typename ContainerT, typename ContextT>
		void read_item(ArchiveT &archive, ContainerT &container, ContextT &context) const
		{
			typename remove_const<typename ContainerT::value_type>::type value;
			archive(value, context), container.insert(value);
		}
	};

	struct indexed_associative_container_reader : container_reader_base
	{
		template <typename ArchiveT, typename ContainerT>
		void read_item(ArchiveT &archive, ContainerT &container) const
		{
			typename remove_const<typename ContainerT::key_type>::type key;
			archive(key), archive(container[key]);
		}

		template <typename ArchiveT, typename ContainerT, typename ContextT>
		void read_item(ArchiveT &archive, ContainerT &container, ContextT &context) const
		{
			typename remove_const<typename ContainerT::key_type>::type key;
			archive(key, context), archive(container[key], context);
		}
	};


	struct sequence_container_traits
	{
		typedef container_type_tag category;
		typedef sequence_container_reader item_reader_type;
	};

	struct associative_container_traits
	{
		typedef container_type_tag category;
		typedef associative_container_reader item_reader_type;
	};

	struct indexed_associative_container_traits
	{
		typedef container_type_tag category;
		typedef indexed_associative_container_reader item_reader_type;
	};



	template <typename T, typename TraitsT, typename A> struct type_traits< std::basic_string<T, TraitsT, A> > : sequence_container_traits { };
	template <typename T, typename A> struct type_traits< std::deque<T, A> > : sequence_container_traits { };
	template <typename T, typename A> struct type_traits< std::list<T, A> > : sequence_container_traits { };
	template <typename T, typename A> struct type_traits< std::vector<T, A> > : sequence_container_traits { };

	template <typename T, typename CompT, typename A> struct type_traits< std::set<T, CompT, A> > : associative_container_traits { };
	template <typename T, typename CompT, typename A> struct type_traits< std::multiset<T, CompT, A> > : associative_container_traits { };
	template <typename KeyT, typename T, typename CompT, typename A> struct type_traits< std::map<KeyT, T, CompT, A> > : indexed_associative_container_traits { };
	template <typename KeyT, typename T, typename CompT, typename A> struct type_traits< std::multimap<KeyT, T, CompT, A> > : associative_container_traits { };
}
