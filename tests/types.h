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
		inline void serialize(ArchiveT &archive, A &v, unsigned /*version*/)
		{
			archive(v.a);
			archive(v.b);
			archive(v.c);
		}

		template <typename ArchiveT>
		inline void serialize(ArchiveT &archive, B &v, unsigned /*version*/)
		{
			archive(v.a);
			archive(v.b);
		}

		template <typename ArchiveT>
		inline void serialize(ArchiveT &archive, C &v, unsigned /*version*/)
		{
			archive(v.b);
			archive(v.a);
		}
	}
}
