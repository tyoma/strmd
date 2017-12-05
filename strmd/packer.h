#pragma once

#include "type_traits.h"

#include <limits>

namespace strmd
{
	struct direct
	{
		template <typename StreamT, typename T>
		static void pack(StreamT &stream, T value);

		template <typename StreamT, typename T>
		static void unpack(StreamT &stream, T &value);
	};

	class varint
	{
	public:
		enum {
			bits_per_byte = 7,

			terminator = 1 << bits_per_byte,
			mask = terminator - 1
		};

	public:
		template <typename StreamT, typename T>
		static void pack(StreamT &stream, T value);

		template <typename StreamT, typename T>
		static void unpack(StreamT &stream, T &value);

	private:
		template <typename T, typename StreamT>
		static void pack_unsigned(StreamT &stream, T value);

		template <typename StreamT, typename T>
		static void unpack_unsigned(StreamT &stream, T &value);
	};



	template <typename StreamT, typename T>
	inline void direct::pack(StreamT &stream, T value)
	{	stream.write(&value, sizeof(value));	}

	template <typename StreamT, typename T>
	inline void direct::unpack(StreamT &stream, T &value)
	{	stream.read(&value, sizeof(value));	}


	template <bool enable>
	struct zigzag
	{
		template <typename T> static T code(T value) { return value; }
		template <typename T> static T decode(T value) { return value; }
	};

	template <>
	struct zigzag<true>
	{
		template <typename T> static typename remove_sign<T>::type code(T value) { return (value << 1) ^ (value >> (sizeof(T) * 8 - 1)); }
		template <typename T> static T decode(typename remove_sign<T>::type value) { return (value >> 1) ^ (-(T(value) & 0x01)); }
	};

	template <typename StreamT, typename T>
	inline void varint::pack(StreamT &stream, T value)
	{	pack_unsigned(stream, zigzag<std::numeric_limits<T>::is_signed>::code(value));	}

	template <typename StreamT, typename T>
	inline void varint::unpack(StreamT &stream, T &value)
	{
		unpack_unsigned(stream, value);
		value = zigzag<std::numeric_limits<T>::is_signed>::template decode<T>(value);
	}

	template <typename T, typename StreamT>
	inline void varint::pack_unsigned(StreamT &stream, T value)
	{
		do
		{
			unsigned char b = value & mask;

			value >>= bits_per_byte;
			if (!value)
				b |= terminator;
			stream.write(&b, 1);
		} while (value);
	}

	template <typename StreamT, typename T>
	inline void varint::unpack_unsigned(StreamT &stream, T &value)
	{
		unsigned char b, shift = 0;

		value = 0;
		do
		{
			stream.read(&b, 1);
			value |= static_cast<T>(b & mask) << shift;
			shift += bits_per_byte;
		} while (!(b & terminator));
	}
}
