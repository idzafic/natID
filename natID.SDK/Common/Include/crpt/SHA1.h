// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file SHA1.h
    @brief Provides a SHA-1 cryptographic hash computation class. */
#pragma once
#include <td/Types.h>
#include <crpt/Hex.h>

// DBL_INT_ADD treats two unsigned ints a and b as one 64-bit integer and adds c to it
#define SHA1_ROTLEFT(a,b) ((a << b) | (a >> (32-b)))
#define SHA1_DBL_INT_ADD(a,b,c) if (a > 0xffffffff - c) ++b; a += c;
#define SHA1_DIGEST_SIZE (20)


namespace crpt
{
	/// @brief Computes a SHA-1 cryptographic hash digest.
	class SHA1
	{
		td::BYTE _data[64]; ///< Internal data block buffer.
		td::UINT4 _dataLen; ///< Number of bytes currently buffered in _data.
		td::UINT4 _bitLen[2]; ///< Total message length in bits, stored as two 32-bit words.
		td::UINT4 _state[5]; ///< Intermediate hash state words (H0..H4).
		td::UINT4 _k[4]; ///< SHA-1 round constants.

		/// @brief Processes a 64-byte data block and updates the internal hash state.
		/// @param data Pointer to the 64-byte input block.
		inline void transform(td::BYTE data[])
		{
			td::UINT4 i, j, t, m[80];

			for (i = 0, j = 0; i < 16; ++i, j += 4)
				m[i] = (data[j] << 24) + (data[j + 1] << 16) + (data[j + 2] << 8) + (data[j + 3]);
			for (; i < 80; ++i) {
				m[i] = (m[i - 3] ^ m[i - 8] ^ m[i - 14] ^ m[i - 16]);
				m[i] = (m[i] << 1) | (m[i] >> 31);
			}

			td::UINT4 a = _state[0];
			td::UINT4 b = _state[1];
			td::UINT4 c = _state[2];
			td::UINT4 d = _state[3];
			td::UINT4 e = _state[4];

			for (i = 0; i < 20; ++i)
			{
				t = SHA1_ROTLEFT(a, 5) + ((b & c) ^ (~b & d)) + e + _k[0] + m[i];
				e = d;
				d = c;
				c = SHA1_ROTLEFT(b, 30);
				b = a;
				a = t;
			}
			for (; i < 40; ++i)
			{
				t = SHA1_ROTLEFT(a, 5) + (b ^ c ^ d) + e + _k[1] + m[i];
				e = d;
				d = c;
				c = SHA1_ROTLEFT(b, 30);
				b = a;
				a = t;
			}
			for (; i < 60; ++i)
			{
				t = SHA1_ROTLEFT(a, 5) + ((b & c) ^ (b & d) ^ (c & d)) + e + _k[2] + m[i];
				e = d;
				d = c;
				c = SHA1_ROTLEFT(b, 30);
				b = a;
				a = t;
			}
			for (; i < 80; ++i)
			{
				t = SHA1_ROTLEFT(a, 5) + (b ^ c ^ d) + e + _k[3] + m[i];
				e = d;
				d = c;
				c = SHA1_ROTLEFT(b, 30);
				b = a;
				a = t;
			}

			_state[0] += a;
			_state[1] += b;
			_state[2] += c;
			_state[3] += d;
			_state[4] += e;
		}

		/// @brief Initializes the SHA-1 context with standard initial hash values and constants.
		inline void init()
		{
			_dataLen = 0;
			_bitLen[0] = 0;
			_bitLen[1] = 0;
			_state[0] = 0x67452301;
			_state[1] = 0xEFCDAB89;
			_state[2] = 0x98BADCFE;
			_state[3] = 0x10325476;
			_state[4] = 0xc3d2e1f0;
			_k[0] = 0x5a827999;
			_k[1] = 0x6ed9eba1;
			_k[2] = 0x8f1bbcdc;
			_k[3] = 0xca62c1d6;
		}

		/// @brief Feeds bytes into the SHA-1 computation, processing full blocks as they become available.
		/// @param data Pointer to the input byte data.
		/// @param len Number of bytes to process.
		inline void update(td::BYTE* data, size_t len)
		{
			//td::UINT4 i;

			for (size_t i = 0; i < len; ++i)
			{
				_data[_dataLen] = data[i];
				_dataLen++;
				if (_dataLen == 64)
				{
					transform(_data);
					SHA1_DBL_INT_ADD(_bitLen[0], _bitLen[1], 512);
					_dataLen = 0;
				}
			}
		}

		/// @brief Pads the message and produces the final 20-byte SHA-1 digest.
		/// @param hash Output buffer of at least SHA1_DIGEST_SIZE (20) bytes to receive the digest.
		inline void finalize(td::BYTE* hash)
		{
			td::UINT4 i = _dataLen;

			// Pad whatever data is left in the buffer.
			if (_dataLen < 56)
			{
				_data[i++] = 0x80;
				while (i < 56)
					_data[i++] = 0x00;
			}
			else
			{
				_data[i++] = 0x80;
				while (i < 64)
					_data[i++] = 0x00;
				transform(_data);
				memset(_data, 0, 56);
			}

			// Append to the padding the total message's length in bits and transform.
			SHA1_DBL_INT_ADD(_bitLen[0], _bitLen[1], 8 * _dataLen);
			_data[63] = (td::BYTE) _bitLen[0];
			_data[62] = (td::BYTE) (_bitLen[0] >> 8);
			_data[61] = (td::BYTE) (_bitLen[0] >> 16);
			_data[60] = (td::BYTE) (_bitLen[0] >> 24);
			_data[59] = (td::BYTE) _bitLen[1];
			_data[58] = (td::BYTE) (_bitLen[1] >> 8);
			_data[57] = (td::BYTE) (_bitLen[1] >> 16);
			_data[56] = (td::BYTE) (_bitLen[1] >> 24);
			transform(_data);

			// Since this implementation uses little endian byte ordering and MD uses big endian,
			// reverse all the bytes when copying the final state to the output hash.
			for (i = 0; i < 4; ++i)
			{
				hash[i] = (_state[0] >> (24 - i * 8)) & 0x000000ff;
				hash[i + 4] = (_state[1] >> (24 - i * 8)) & 0x000000ff;
				hash[i + 8] = (_state[2] >> (24 - i * 8)) & 0x000000ff;
				hash[i + 12] = (_state[3] >> (24 - i * 8)) & 0x000000ff;
				hash[i + 16] = (_state[4] >> (24 - i * 8)) & 0x000000ff;
			}
		}
	public:

		/// @brief Computes the SHA-1 digest of a raw character buffer.
		/// @param pStr Pointer to the input character data.
		/// @param len Number of characters to hash.
		/// @param digest Output buffer of at least SHA1_DIGEST_SIZE (20) bytes to receive the digest.
		inline void calc(const char* pStr, size_t len, td::BYTE* digest)
		{
			init();
			update((td::BYTE*) pStr, len);
			finalize(digest);
		}

		/// @brief Computes the SHA-1 digest of a td::String and writes it to a raw byte buffer.
		/// @param strIn Input string to hash.
		/// @param digest Output buffer of at least SHA1_DIGEST_SIZE (20) bytes to receive the digest.
		inline void calc(const td::String& strIn, td::BYTE* digest)
		{
			init();
			update((td::BYTE*) strIn.c_str(), strIn.length());
			finalize(digest);
		}

		/// @brief Computes the SHA-1 digest of a td::String and stores the raw bytes in a td::String.
		/// @param strIn Input string to hash.
		/// @param hashStr Output string reserved to hold the 20-byte raw digest.
		inline void calc(const td::String& strIn, td::String& hashStr)
		{
			init();
			update((td::BYTE*) strIn.c_str(), strIn.length());
			hashStr.reserve(20);
			finalize((td::BYTE*)hashStr.begin());
		}

		/// @brief Computes the SHA-1 digest of a td::String and stores it as an uppercase hex string.
		/// @param str Input string to hash.
		/// @param hashOut Output string that receives the 40-character hexadecimal digest.
		inline void calcToHex(const td::String& str, td::String& hashOut)
		{
			init();
			update((td::BYTE*)str.c_str(), str.length());
			td::BYTE digest[20];
			finalize(digest);
			crpt::toHex(digest, hashOut);
		}
	};
}
