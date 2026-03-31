// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <td/Types.h>
#include <td/String.h>
#include <crpt/Hex.h>

namespace crpt
{
	//typedef struct {
	//	td::BYTE _ctxKey[32];
	//	td::BYTE _ctxEncKey[32];
	//	td::BYTE _ctxDecKey[32];
	//} aes256_context;
	//Vidi na http://www.literatecode.com/aes256

#define AES256_FD(x)  (((x) >> 1) ^ (((x) & 1) ? 0x8d : 0))

	class AES256
	{
		td::BYTE _ctxKey[32];
		td::BYTE _ctxEncKey[32];
		td::BYTE _ctxDecKey[32];
		char _trailedZeros = -1;

		//bool _addTrailZero = true; //encrypted content len will be multiplier of 16

	public:
		typedef enum _tOut { Raw =0, Hex} EncryptedType;
	protected:
		inline td::BYTE gf_alog(td::BYTE x) const // calculate anti-logarithm gen 3
		{
			td::BYTE y = 1, i;

			for (i = 0; i < x; i++) y ^= rj_xtime(y);

			return y;
		} // gf_alog 
		  
		inline td::BYTE gf_log(td::BYTE x) const // calculate logarithm gen 3
		{
			td::BYTE y, i = 0;

			if (x)
				for (i = 1, y = 1; i > 0; i++)
				{
					y ^= rj_xtime(y);
					if (y == x) 
						break;
				}

			return i;
		} // gf_log 


		  // -------------------------------------------------------------------------- 
		inline td::BYTE gf_mulinv(td::BYTE x) const  // calculate multiplicative inverse
		{
			return (x) ? gf_alog(255 - gf_log(x)) : 0;
		} // gf_mulinv 

		  // -------------------------------------------------------------------------- 
		inline td::BYTE rj_sbox(td::BYTE x) const
		{
			td::BYTE y, sb;

			sb = y = gf_mulinv(x);
			y = (td::BYTE)(y << 1) | (y >> 7), sb ^= y;
			y = (td::BYTE)(y << 1) | (y >> 7), sb ^= y;
			y = (td::BYTE)(y << 1) | (y >> 7), sb ^= y;
			y = (td::BYTE)(y << 1) | (y >> 7), sb ^= y;

			return (sb ^ 0x63);
		} // rj_sbox 

		  // -------------------------------------------------------------------------- 
		inline td::BYTE rj_sbox_inv(td::BYTE x) const
		{
			td::BYTE y, sb;

			y = x ^ 0x63;
			sb = y = (td::BYTE)(y << 1) | (y >> 7);
			y = (td::BYTE)(y << 2) | (y >> 6);
			sb ^= y;
			y = (td::BYTE)(y << 3) | (y >> 5);
			sb ^= y;

			return gf_mulinv(sb);
		} // rj_sbox_inv 

		inline td::BYTE rj_xtime(td::BYTE x) const
		{
			td::BYTE y = (td::BYTE)(x << 1);
			return (x & 0x80) ? (y ^ 0x1b) : y;
		} //rj_xtime 

		  //-------------------------------------------------------------------------- 
		inline void aes_subBytes(td::BYTE *buf) const
		{
			td::BYTE i = 16;

			while (i--) 
				buf[i] = rj_sbox(buf[i]);
		} //aes_subBytes 

		  //-------------------------------------------------------------------------- 
		inline void aes_subBytes_inv(td::BYTE *buf) const
		{
			td::BYTE i = 16;

			while (i--) buf[i] = rj_sbox_inv(buf[i]);
		} //aes_subBytes_inv 

		  //-------------------------------------------------------------------------- 
		inline void aes_addRoundKey(td::BYTE *buf, td::BYTE *key) const
		{
			td::BYTE i = 16;

			while (i--) 
				buf[i] ^= key[i];
		} //aes_addRoundKey 

		  //-------------------------------------------------------------------------- 
		inline void aes_addRoundKey_cpy(td::BYTE *buf, td::BYTE *key, td::BYTE *cpk) const
		{
			td::BYTE i = 16;

			while (i--)  buf[i] ^= (cpk[i] = key[i]), cpk[16 + i] = key[16 + i];
		} //aes_addRoundKey_cpy 


		  //-------------------------------------------------------------------------- 
		inline void aes_shiftRows(td::BYTE *buf) const
		{
			td::BYTE i, j; //to make it potentially parallelable :) 

			i = buf[1], buf[1] = buf[5], buf[5] = buf[9], buf[9] = buf[13], buf[13] = i;
			i = buf[10], buf[10] = buf[2], buf[2] = i;
			j = buf[3], buf[3] = buf[15], buf[15] = buf[11], buf[11] = buf[7], buf[7] = j;
			j = buf[14], buf[14] = buf[6], buf[6] = j;

		} //aes_shiftRows 

		  //-------------------------------------------------------------------------- 
		inline void aes_shiftRows_inv(td::BYTE* buf) const
		{
			td::BYTE i, j; //same as above :) 

			i = buf[1], buf[1] = buf[13], buf[13] = buf[9], buf[9] = buf[5], buf[5] = i;
			i = buf[2], buf[2] = buf[10], buf[10] = i;
			j = buf[3], buf[3] = buf[7], buf[7] = buf[11], buf[11] = buf[15], buf[15] = j;
			j = buf[6], buf[6] = buf[14], buf[14] = j;

		} //aes_shiftRows_inv 

		  //-------------------------------------------------------------------------- 
		inline void aes_mixColumns(td::BYTE* buf) const
		{
			td::BYTE i, a, b, c, d, e;

			for (i = 0; i < 16; i += 4)
			{
				a = buf[i];
				b = buf[i + 1];
				c = buf[i + 2];
				d = buf[i + 3];
				e = a ^ b ^ c ^ d;
				buf[i] ^= e ^ rj_xtime(a ^ b);
				buf[i + 1] ^= e ^ rj_xtime(b ^ c);
				buf[i + 2] ^= e ^ rj_xtime(c ^ d);
				buf[i + 3] ^= e ^ rj_xtime(d ^ a);
			}
		} //aes_mixColumns 

		  //-------------------------------------------------------------------------- 
		inline void aes_mixColumns_inv(td::BYTE *buf) const
		{
			td::BYTE i, a, b, c, d, e, x, y, z;

			for (i = 0; i < 16; i += 4)
			{
				a = buf[i];
				b = buf[i + 1];
				c = buf[i + 2];
				d = buf[i + 3];
				e = a ^ b ^ c ^ d;
				z = rj_xtime(e);
				x = e ^ rj_xtime(rj_xtime(z ^ a ^ c));
				y = e ^ rj_xtime(rj_xtime(z ^ b ^ d));
				buf[i] ^= x ^ rj_xtime(a ^ b);
				buf[i + 1] ^= y ^ rj_xtime(b ^ c);
				buf[i + 2] ^= x ^ rj_xtime(c ^ d);
				buf[i + 3] ^= y ^ rj_xtime(d ^ a);
			}
		} //aes_mixColumns_inv 

		  //-------------------------------------------------------------------------- 
		inline void aes_expandEncKey(td::BYTE *k, td::BYTE *rc) const
		{
			td::BYTE i;

			k[0] ^= rj_sbox(k[29]) ^ (*rc);
			k[1] ^= rj_sbox(k[30]);
			k[2] ^= rj_sbox(k[31]);
			k[3] ^= rj_sbox(k[28]);
			*rc = rj_xtime(*rc);

			for (i = 4; i < 16; i += 4)  k[i] ^= k[i - 4], k[i + 1] ^= k[i - 3],
				k[i + 2] ^= k[i - 2], k[i + 3] ^= k[i - 1];
			k[16] ^= rj_sbox(k[12]);
			k[17] ^= rj_sbox(k[13]);
			k[18] ^= rj_sbox(k[14]);
			k[19] ^= rj_sbox(k[15]);

			for (i = 20; i < 32; i += 4) k[i] ^= k[i - 4], k[i + 1] ^= k[i - 3],
				k[i + 2] ^= k[i - 2], k[i + 3] ^= k[i - 1];

		} //aes_expandEncKey 

		  //-------------------------------------------------------------------------- 
		inline void aes_expandDecKey(td::BYTE *k, td::BYTE *rc) const
		{
			td::BYTE i;

			for (i = 28; i > 16; i -= 4) k[i + 0] ^= k[i - 4], k[i + 1] ^= k[i - 3],
				k[i + 2] ^= k[i - 2], k[i + 3] ^= k[i - 1];

			k[16] ^= rj_sbox(k[12]);
			k[17] ^= rj_sbox(k[13]);
			k[18] ^= rj_sbox(k[14]);
			k[19] ^= rj_sbox(k[15]);

			for (i = 12; i > 0; i -= 4)  k[i + 0] ^= k[i - 4], k[i + 1] ^= k[i - 3],
				k[i + 2] ^= k[i - 2], k[i + 3] ^= k[i - 1];

			*rc = AES256_FD(*rc);
			k[0] ^= rj_sbox(k[29]) ^ (*rc);
			k[1] ^= rj_sbox(k[30]);
			k[2] ^= rj_sbox(k[31]);
			k[3] ^= rj_sbox(k[28]);
		} //aes_expandDecKey 


		  //-------------------------------------------------------------------------- 
		inline void aes256_init(td::BYTE *k)
		{
			td::BYTE rcon = 1;
			td::BYTE i;

			for (i = 0; i < sizeof(_ctxKey); i++) _ctxEncKey[i] = _ctxDecKey[i] = k[i];
			for (i = 8; --i;) aes_expandEncKey(_ctxDecKey, &rcon);
		} //aes256_init 

		  //-------------------------------------------------------------------------- 
		inline void aes256_done()
		{
			td::BYTE i;

			for (i = 0; i < sizeof(_ctxKey); i++)
				_ctxKey[i] = _ctxEncKey[i] = _ctxDecKey[i] = 0;
		} //aes256_done 

		  //-------------------------------------------------------------------------- 
		inline void aes256_encrypt_ecb(td::BYTE *buf)
		{
			td::BYTE i, rcon;

			aes_addRoundKey_cpy(buf, _ctxEncKey, _ctxKey);
			for (i = 1, rcon = 1; i < 14; ++i)
			{
				aes_subBytes(buf);
				aes_shiftRows(buf);
				aes_mixColumns(buf);
				if (i & 1) aes_addRoundKey(buf, &_ctxKey[16]);
				else aes_expandEncKey(_ctxKey, &rcon), aes_addRoundKey(buf, _ctxKey);
			}
			aes_subBytes(buf);
			aes_shiftRows(buf);
			aes_expandEncKey(_ctxKey, &rcon);
			aes_addRoundKey(buf, _ctxKey);
		} //aes256_encrypt 

		  //-------------------------------------------------------------------------- 
		inline void aes256_decrypt_ecb(td::BYTE *buf)
		{
			td::BYTE i, rcon;

			aes_addRoundKey_cpy(buf, _ctxDecKey, _ctxKey);
			aes_shiftRows_inv(buf);
			aes_subBytes_inv(buf);

			for (i = 14, rcon = 0x80; --i;)
			{
				if ((i & 1))
				{
					aes_expandDecKey(_ctxKey, &rcon);
					aes_addRoundKey(buf, &_ctxKey[16]);
				}
				else aes_addRoundKey(buf, _ctxKey);
				aes_mixColumns_inv(buf);
				aes_shiftRows_inv(buf);
				aes_subBytes_inv(buf);
			}
			aes_addRoundKey(buf, _ctxKey);
		} //aes256_decrypt 

	public:

		bool encrypt(const td::String& key, const td::BYTE* inData, size_t inLen, td::String& outEncryptedData, EncryptedType encType = Raw)
		{
			if (inLen == 0)
				return false;

			if (key.length() != 32)
				return false;

			//aes256_context ctx;
			aes256_init((td::BYTE*) key.c_str());

			size_t nFullBuffers = inLen / 16;
			size_t nRem = inLen % 16;
			{	
				size_t toReserve = nFullBuffers;
				if (nRem != 0)
					++toReserve;
				if (encType == Raw)
					outEncryptedData.reserve(toReserve * 16);
				else
					outEncryptedData.reserve(toReserve * 32);				
			}

			//uint8_t key[32];
			td::BYTE buf[16];

			td::BYTE* pOut = (td::BYTE*)outEncryptedData.begin();
			const td::BYTE* pIn = inData;
			for (size_t i = 0; i < nFullBuffers; ++i)
			{
				memcpy(buf, pIn, 16);
				aes256_encrypt_ecb(buf);
				if (encType == Raw)
				{
					memcpy(pOut, buf, 16);
				}
				else
				{
					char hexOut[32];
					crpt::toHex(buf, hexOut);
					memcpy(pOut, hexOut, 32);
				}


				pIn += 16;
				if (encType == Raw)
					pOut += 16;
				else
					pOut += 32;
			}

			//encrypt remaining part
			if (nRem > 0)
			{
				_trailedZeros = (char) 16 - (char) nRem;
				memset(buf, 0, 16);
				memcpy(buf, pIn, nRem);
				aes256_encrypt_ecb(buf);
				if (encType == Raw)
				{
					memcpy(pOut, buf, 16);
				}
				else
				{
					char hexOut[32];
					crpt::toHex(buf, hexOut);
					memcpy(pOut, hexOut, 32);
				}
			}
			else
				_trailedZeros = 0;

			return true;
		}

		bool encrypt(const td::String& key, const td::String& inData, td::String& outEncryptedData, EncryptedType encType = Raw)
		{
			return encrypt(key, (const td::BYTE*) inData.c_str(), inData.length(), outEncryptedData, encType);

			//size_t inLen = inData.length();

			//if (inLen == 0)
			//	return false;			

			//if (key.length() != 32)
			//	return false;

			////aes256_context ctx;
			//aes256_init( (td::BYTE*) key.c_str());
			//
			//size_t nFullBuffers = inLen / 16;
			//size_t nRem = inLen % 16;
			//{
			//	size_t toReserve = nFullBuffers;
			//	if (nRem != 0)
			//		++toReserve;
			//	if (encType == Raw)
			//		outEncryptedData.reserve(toReserve * 16);
			//	else
			//		outEncryptedData.reserve(toReserve * 32);
			//}

			////uint8_t key[32];
			//td::BYTE buf[16];
			//
			//td::BYTE* pOut = (td::BYTE*)outEncryptedData.begin();
			//const td::BYTE* pIn = (td::BYTE*) inData.c_str();
			//for (size_t i = 0; i < nFullBuffers; ++i)
			//{
			//	memcpy(buf, pIn, 16);
			//	aes256_encrypt_ecb(buf);
			//	if (encType == Raw)
			//	{
			//		memcpy(pOut, buf, 16);
			//	}
			//	else
			//	{
			//		char hexOut[32];
			//		crpt::toHex(buf, hexOut);
			//		memcpy(pOut, hexOut, 32);
			//	}
			//	
			//	
			//	pIn += 16;
			//	if (encType == Raw)
			//		pOut += 16;
			//	else
			//		pOut += 32;
			//}
			//
			////encrypt remaining part
			//if (nRem > 0)
			//{
			//	memset(buf, 0, 16);
			//	memcpy(buf, pIn, nRem);
			//	aes256_encrypt_ecb(buf);
			//	if (encType == Raw)
			//	{
			//		memcpy(pOut, buf, 16);
			//	}
			//	else
			//	{
			//		char hexOut[32];
			//		crpt::toHex(buf, hexOut);
			//		memcpy(pOut, hexOut, 32);
			//	}
			//}
			//
			//return true;
		}

		bool decrypt(const td::String& key, const td::BYTE* inData, size_t inLen, td::String& outDecryptedData, EncryptedType encType = Raw)
		{
			if (inLen == 0)
				return false;

			if (encType == Raw)
			{
				if (inLen % 16 != 0)
					return false;
			}
			else if (encType == Hex)
			{
				if (inLen % 32 != 0)
					return false;
			}

			if (key.length() != 32)
				return false;


			//aes256_context ctx;
			aes256_init((td::BYTE*) key.c_str());

			size_t nFullBuffers = inLen / 16;
			size_t nRem = inLen % 16;

			if (encType == Hex)
			{
				nFullBuffers /= 2;
				nRem /= 2;
			}

			if (encType == Raw)
				outDecryptedData.reserve(inLen);
			else if (encType == Hex)
			{
				outDecryptedData.reserve(inLen / 2);
			}

			td::BYTE buf[16];

			td::BYTE* pOut = (td::BYTE*)outDecryptedData.begin();
			const td::BYTE* pIn = inData;
			for (size_t i = 0; i < nFullBuffers; ++i)
			{
				if (encType == Raw)
				{
					memcpy(buf, pIn, 16);
					pIn += 16;
				}
				else if (encType == Hex)
				{
					crpt::fromHex((const char*)pIn, 32, buf);
					pIn += 32;
				}

				aes256_decrypt_ecb(buf);
				memcpy(pOut, buf, 16);
				pOut += 16;
			}

			size_t deltaN = 0;
			if (_trailedZeros < 0)
			{
				for (size_t i = 16; i > 0; --i)
				{
					td::BYTE ch = buf[i - 1];
					if (ch == 0)
						++deltaN;
					else
						break;
				}
			}
			else
				deltaN = _trailedZeros;

			if (deltaN != 0)
				outDecryptedData.reduceSize(deltaN);
			return true;
		}

		bool decrypt(const td::String& key, const td::String& inData, td::String& outDecryptedData, EncryptedType encType = Raw)
		{
			return decrypt(key, (const td::BYTE*) inData.c_str(), inData.length(), outDecryptedData, encType);			
		}

		void setTrailZeros(char nTrailedZeros)
		{
			_trailedZeros = nTrailedZeros;
		}

		char getTrailZeros() const
		{
			return _trailedZeros;
		}
	};
}
