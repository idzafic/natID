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
#include <simd/Alignment.h>

namespace mem
{
	template <typename T>
	inline void fastCopy(T* pDest, const T* pSource, size_t size)
	{
	    //if (size > 0)
	        memcpy((void*)pDest, (const void*)pSource, size * sizeof(T));
	}

	template <typename T>
	inline bool fastCompare(const T* pDest, const T* pSource, size_t size)
	{
	    //if (size > 0)
	    if (memcmp((void*)pDest, (const void*)pSource, size * sizeof(T)) != 0)
	        return false;
	    return true;
	}


    #define SSE_MMREG_SIZE	16

    template <typename T>
    inline void fastZero(T* pDest, size_t size)
    {
#ifdef USE_SIMD
        //void *zeromem_sse( void *dst, int, size_t size )
        //{
            //void   *ret   = dst;
            char   *p_dst = (char*)pDest;
            size_t sizeInBytes = size * sizeof(T);

            if( sizeInBytes >= 64 )
            {
                size_t delta;
                if( ( delta = ((size_t)p_dst) & ( SSE_MMREG_SIZE - 1 ) ) != 0 ) /* Align destinition */
                {
                    delta = SSE_MMREG_SIZE - delta;
                    sizeInBytes -= delta;
                    memset( p_dst, 0, delta );
                    p_dst += delta;
                }

                //Get number of even blocks and save reminder
                size_t i = sizeInBytes >> 6;
                sizeInBytes &= 63;
             
                __m128 xmm0 = _mm_setzero_ps();
                __m128 xmm1 = _mm_setzero_ps();
                __m128 xmm2 = _mm_setzero_ps();
                __m128 xmm3 = _mm_setzero_ps();
                for( ; i; --i, p_dst += 64 )
                {
                    _mm_stream_ps( (float*)p_dst     , xmm0 );
                    _mm_stream_ps( (float*)p_dst + 4 , xmm1 );
                    _mm_stream_ps( (float*)p_dst + 8 , xmm2 );
                    _mm_stream_ps( (float*)p_dst + 12, xmm3 );
                }
            }

            if( sizeInBytes)
                memset( p_dst, 0, sizeInBytes );
#else
            memset((void*)pDest, 0, size *  sizeof(T));
#endif
    }
	
}
