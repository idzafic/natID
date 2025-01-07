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

namespace crpt
{

	//inline size_t getBase64DecodeBufferLength(const char *bufCoded)
	//{
	//	const td::BYTE* bufin = (const td::BYTE*) bufCoded;
	//	while (pr2six[*(bufin++)] <= 63);

	//	size_t nprbytes = (bufin - (const td::BYTE*) bufCoded) - 1;
	//	size_t nbytesdecoded = (nprbytes/ 4) * 3;

	//	return nbytesdecoded; // +1; without zero
	//}

	inline size_t getBase64DecodeBufferLength(size_t inLen, size_t nPad)
	{		
		assert(inLen % 4 == 0);
		if (inLen % 4 != 0)
			return 0;
		size_t nBytesDecoded = (inLen  / 4) * 3 - nPad;

		return nBytesDecoded; // +1; without zero
	}

	inline size_t getNoPads(const char* inEncoded, size_t inLen)
	{
		size_t nPads = 0;
		if (inEncoded[inLen - 1] == '=')
		{
			++nPads;
			if (inEncoded[inLen - 2] == '=')
				++nPads;
		}
		return nPads;
	}


inline bool fromBase64(const char* inBase64Encoded, size_t inLen, td::BYTE* outBuff, size_t& outLen)
{
    //check length
    size_t a = inLen * 3 / 4;
    if ( a * 4 / 3 != inLen)
    {
        assert(false);
        return false;
    }
    
    auto nPads = getNoPads(inBase64Encoded, inLen);
    
    if (outLen < (a - nPads) )
    {
        assert(false);
        return false;
    }
    
    const char base46_map[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                         'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                         'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                         'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};
    
    char counts = 0;
    char buffer[4];
    //char* plain = malloc(strlen(cipher) * 3 / 4);
    size_t p = 0;

    for(size_t i = 0; i < inLen; i++)
    {
        char k=0;
        for(k=0; k < 64 && base46_map[k] != inBase64Encoded[i]; k++);
        buffer[counts++] = k;
        if(counts == 4)
        {
            if (p >= outLen)
                return false;
            outBuff[p++] = (buffer[0] << 2) + (buffer[1] >> 4);
            if(buffer[2] != 64)
            {
                if (p >= outLen)
                    return false;
                outBuff[p++] = (buffer[1] << 4) + (buffer[2] >> 2);
            }
            
            if(buffer[3] != 64)
            {
                if (p >= outLen)
                    return false;
                outBuff[p++] = (buffer[2] << 6) + buffer[3];
            }
                
            counts = 0;
        }
    }
    outLen = p;
    //plain[p] = '\0';    /* string padding character */
    return true;
}

inline bool fromBase64(const td::String& strBase64Encoded, td::String& strDecoded)
{
    size_t inLen = strBase64Encoded.length();
    const char* inEncoded = strBase64Encoded.c_str();
    size_t nPads = getNoPads(inEncoded, inLen);

    size_t decodedLen = getBase64DecodeBufferLength(inLen, nPads);
    if (decodedLen == 0)
        return false;
    if (inLen <= nPads)
        return false;
    strDecoded.reserve(decodedLen);
    //inLen -= nPads;
    return fromBase64(inEncoded, inLen, (td::BYTE*) strDecoded.begin(), decodedLen);
}

	//inLen should be without paddings (=)
//	inline bool fromBase64(const char* inBase64Encoded, size_t inLen, td::BYTE* out, size_t& outLen)
//	{
//		//if (inLen == 0)
//		//	return false;
//
//		// 64  white space
//		// 65  = (equality character)
//		// 66 invalid character
//		//ASCI                 '+'        '/''0''1'   .....             '9'
//		const td::BYTE d[] = { 62,66,66,66,63,52,53,54,55,56,57,58,59,60,61,
//			//       = (padding)
//			66,66,66,65,66,66,66,
//		//   A                                                                         Z
//			 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
//			66,66,66,66,66,66,
//		//   a                                                                         z
//			26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51 };
//
//		const char* end = inBase64Encoded + inLen;
//		char iter = 0;
//		td::UINT4 buf = 0;
//		size_t len = 0;
//
//		while (inBase64Encoded < end)
//		{
//			char ch = *inBase64Encoded++;
//			if ((ch < '+') || (ch > 'z'))
//				return false;
//			td::BYTE c = d[ch - '+'];
//
//			if (c <= 64)
//			{
//				buf = (buf << 6) | c;
//				iter++; // increment the number of iteration
//						// If the buffer is full, split it into bytes
//				if (iter == 4)
//				{
//					if ((len += 3) > outLen)
//						return false; // buffer overflow
//					*(out++) = (td::BYTE) ((buf >> 16) & 0xFF);
//					*(out++) = (td::BYTE) ((buf >> 8) & 0xFF);
//					*(out++) = (td::BYTE) (buf & 0xFF);
//					buf = 0;
//					iter = 0;
//				}
//			}
//			else
//			{
//				return false;
//			}
//		}
//
//		if (iter == 3)
//		{
//			if ((len += 2) > outLen)
//				return false; // buffer overflow
//			*(out++) = (td::BYTE) ((buf >> 10) & 0xFF);
//			*(out++) = (td::BYTE) ((buf >> 2) & 0xFF);
//		}
//		else if (iter == 2)
//		{
//			if (++len > outLen)
//				return false; // buffer overflow
//			*(out++) = (td::BYTE) ((buf >> 4) & 0xFF);
//		}
//
//		outLen = len; // modify to reflect the actual output size
//		return true;
//	}

//	inline bool fromBase64(const td::String& strBase64Encoded, td::String& strDecoded)
//	{
//		size_t inLen = strBase64Encoded.length();
//		const char* inEncoded = strBase64Encoded.c_str();
//		size_t nPads = getNoPads(inEncoded, inLen);
//
//		size_t decodedLen = getBase64DecodeBufferLength(inLen, nPads);
//		if (decodedLen == 0)
//			return false;
//		if (inLen <= nPads)
//			return false;
//		strDecoded.reserve(decodedLen);
//		inLen -= nPads;
//		return fromBase64(inEncoded, inLen, (td::BYTE*) strDecoded.begin(), decodedLen);
//	}
	
	inline size_t getBase64EncodeBufferLength(size_t len)
	{
		return ((len + 2) / 3 * 4); // +1; this is without zero ch
	}

	//trailing zero is not written in base64Encoded
	inline size_t toBase64(const td::BYTE* strIn, size_t len, char* base64Encoded)
	{
		size_t i = 0;
		//value                  0                    ...                                      63
		const char basis_64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		char* p = base64Encoded;
		if (len >= 2)
		{
			for (i = 0; i < len - 2; i += 3)
			{
				*p++ = basis_64[(strIn[i] >> 2) & 0x3F];
				*p++ = basis_64[((strIn[i] & 0x3) << 4) |
					(/*(int) */(strIn[i + 1] & 0xF0) >> 4)];
				*p++ = basis_64[((strIn[i + 1] & 0xF) << 2) |
					(/*(int)*/(strIn[i + 2] & 0xC0) >> 6)];
				*p++ = basis_64[strIn[i + 2] & 0x3F];
			}
		}
		
		if (i < len)
		{
			*p++ = basis_64[(strIn[i] >> 2) & 0x3F];
			if (i == (len - 1)) {
				*p++ = basis_64[((strIn[i] & 0x3) << 4)];
				*p++ = '=';
			}
			else {
				*p++ = basis_64[((strIn[i] & 0x3) << 4) |
					(/*(int)*/(strIn[i + 1] & 0xF0) >> 4)];
				*p++ = basis_64[((strIn[i + 1] & 0xF) << 2)];
			}
			*p++ = '=';
		}

		//*p++ = '\0';
		return p - base64Encoded;
	}

	inline bool toBase64(const td::String& strIn, td::String& base64Encoded)
	{
		size_t nLen = getBase64EncodeBufferLength(strIn.length());
		if (nLen == 0)
			return false;
		if (!base64Encoded.reserve(nLen))
			return false;
		size_t nWritten = toBase64( (td::BYTE*) strIn.c_str(), strIn.length(), base64Encoded.begin());		
		return (nWritten == nLen);		
		//return true;
	}

	inline bool toBase64(const td::BYTE* pIn, size_t inLen, td::String& base64Encoded)
	{
		size_t nLen = getBase64EncodeBufferLength(inLen);
		if (nLen == 0)
			return false;
		if (!base64Encoded.reserve(nLen))
			return false;
		size_t nWritten = toBase64(pIn, inLen, base64Encoded.begin());
		return (nWritten == nLen);
		//return true;
	}

	template <size_t size>
	inline bool toBase64(const td::BYTE (&strIn)[size], td::String& base64Encoded)
	{
		size_t nLen = getBase64EncodeBufferLength(size);
		if (!base64Encoded.reserve(nLen))
			return false;
		size_t nWritten = toBase64(strIn, size, base64Encoded.begin());
		return (nWritten == nLen);
		//return true;
	}

	template <size_t size>
	inline bool toBase64(const td::String& strIn, char(&base64Encoded)[size])
	{
		size_t nLen = getBase64EncodeBufferLength(strIn.length());
		if (nLen == 0)
			return false;
		if (nLen > size)
			return false;		
		size_t nWritten = toBase64( (td::BYTE*) strIn.c_str(), strIn.length(), base64Encoded);
		return (nWritten == nLen);		
		//return true;
	}

	template <size_t size>
	inline bool toBase64(const td::BYTE* strIn, size_t len, char(&base64Encoded)[size])
	{
		size_t nLen = getBase64EncodeBufferLength(len);
		if (nLen == 0)
			return false;
		if (nLen > size)
			return false;
		size_t nWritten = toBase64(strIn, len, (char*)&base64Encoded[0]);
		return (nWritten == nLen);		
		//return true;
	}

    template <size_t size>
    inline size_t toBase64CStr(const td::BYTE* strIn, size_t len, char(&base64Encoded)[size])
    {
        size_t nLen = getBase64EncodeBufferLength(len);
        if (nLen == 0)
            return 0;
        if (nLen >= size)
            return 0;
        size_t nWritten = toBase64(strIn, len, (char*)&base64Encoded[0]);
        base64Encoded[nWritten] = 0;
        return nWritten;
    }

}
