#pragma once

#include <string>

namespace strmd
{
	namespace tests
	{
		struct A
		{
			int a;
			unsigned char b;
			std::string c;
		};

		struct B
		{
			unsigned char a;
			unsigned char b;
		};

		struct C
		{
			unsigned char a;
			unsigned char b;
		};

		struct MyKey
		{
			MyKey(int v = 0)
				: value(v)
			{	}

			int value;

			bool operator <(MyKey rhs) const
			{	return value < rhs.value;	}
		};

		template <typename ArchiveT>
		inline void serialize(ArchiveT &archive, A &v)
		{
			archive(v.a);
			archive(v.b);
			archive(v.c);
		}

		template <typename ArchiveT>
		inline void serialize(ArchiveT &archive, B &v)
		{
			archive(v.a);
			archive(v.b);
		}

		template <typename ArchiveT>
		inline void serialize(ArchiveT &archive, C &v)
		{
			archive(v.b);
			archive(v.a);
		}

		template <typename ArchiveT>
		inline void serialize(ArchiveT &archive, MyKey &v)
		{	archive(v.value);	}
	}
}
