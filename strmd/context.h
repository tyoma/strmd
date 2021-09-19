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
	template <typename ContextT>
	class context_binder
	{
	public:
		context_binder(ContextT context)
			: _context(context)
		{	}

		template <typename ArchiveT, typename T>
		void serialize_full(ArchiveT &archive, T &data) const
		{	archive(data, _context);	}

		template <typename ArchiveT, typename T>
		void serialize_raw(ArchiveT &archive, T &data) const
		{	serialize(archive, data, _context);	}

		template <typename ArchiveT, typename T>
		void serialize_raw(ArchiveT &archive, T &data, unsigned int version_) const
		{	serialize(archive, data, _context, version_);	}

		template <typename ReaderT, typename ContainerT>
		void prepare(ReaderT &reader, ContainerT &container, unsigned int count) const
		{	reader.prepare(container, count);	}

		template <typename ReaderT, typename ArchiveT, typename ContainerT>
		void read_item(ReaderT &reader, ArchiveT &archive, ContainerT &container) const
		{	reader.read_item(archive, container, _context);	}

		template <typename ReaderT, typename ContainerT>
		void complete(ReaderT &reader, ContainerT &container) const
		{	reader.complete(container);	}

	private:
		context_binder(const context_binder &other);
		void operator =(const context_binder &rhs);

	private:
		ContextT _context;
	};

	template <>
	class context_binder<void>
	{
	public:
		template <typename ArchiveT, typename T>
		void serialize_full(ArchiveT &archive, T &data) const
		{	archive(data);	}

		template <typename ArchiveT, typename T>
		void serialize_raw(ArchiveT &archive, T &data) const
		{	serialize(archive, data);	}

		template <typename ArchiveT, typename T>
		void serialize_raw(ArchiveT &archive, T &data, unsigned int version_) const
		{	serialize(archive, data, version_);	}

		template <typename ReaderT, typename ContainerT>
		void prepare(ReaderT &reader, ContainerT &container, unsigned int count) const
		{	reader.prepare(container, count);	}

		template <typename ReaderT, typename ArchiveT, typename ContainerT>
		void read_item(ReaderT &reader, ArchiveT &archive, ContainerT &container) const
		{	reader.read_item(archive, container);	}

		template <typename ReaderT, typename ContainerT>
		void complete(ReaderT &reader, ContainerT &container) const
		{	reader.complete(container);	}
	};
}
