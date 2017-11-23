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

		template <typename ArchiveT>
		void serialize(ArchiveT &archive, const A &v)
		{
			archive(v.a);
			archive(v.b);
			archive(v.c);
		}

		template <typename ArchiveT>
		void serialize(ArchiveT &archive, const B &v)
		{
			archive(v.a);
			archive(v.b);
		}

		template <typename ArchiveT>
		void serialize(ArchiveT &archive, const C &v)
		{
			archive(v.b);
			archive(v.a);
		}
	}
}
