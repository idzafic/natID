// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <mu/mu.h>
namespace mu
{
	MAINUTILS_API td::UINT4 loadTranslation(const char* fileName, const char* extension = nullptr);
}

#define TRANSLATION(x) protected: td::UINT4 _trHashFile = 0; \
template <size_t size>  inline const char* trc(const char(&ctStrIn)[size]) const {td::UINT4 trHashFile = _trHashFile; if (trHashFile == 0) trHashFile = mu::loadTranslation(x); size_t inOutSize = size - 1; return mu::trc(trHashFile, &ctStrIn[0], inOutSize); } \
template <size_t size>  inline const char* trc(const char(&ctStrIn)[size]) { if (_trHashFile == 0) _trHashFile = mu::loadTranslation(x); size_t inOutSize = size - 1; return mu::trc(_trHashFile, &ctStrIn[0], inOutSize); } \
template <size_t size> 	inline QString trq(const char(&ctStrIn)[size]) const { td::UINT4 trHashFile = _trHashFile; if (trHashFile == 0) trHashFile = mu::loadTranslation(x); size_t inOutSize = size - 1; const char* pStr = mu::trc(trHashFile, &ctStrIn[0], inOutSize); return QString::fromUtf8(pStr, (int) inOutSize);} \
template <size_t size> 	inline QString trq(const char(&ctStrIn)[size]) {if (_trHashFile == 0) _trHashFile = mu::loadTranslation(x); size_t inOutSize = size - 1;const char* pStr = mu::trc(_trHashFile, &ctStrIn[0], inOutSize); return QString::fromUtf8(pStr, (int) inOutSize);} \
template <size_t size> inline td::String trs(const char(&ctStrIn)[size]) const { td::UINT4 trHashFile = _trHashFile; if (trHashFile == 0) trHashFile = mu::loadTranslation(x);  size_t inOutSize = size - 1; const char* pStr = mu::trc(trHashFile, &ctStrIn[0], inOutSize); return td::String(pStr, inOutSize); } \
template <size_t size> inline td::String trs(const char(&ctStrIn)[size]) { if (_trHashFile == 0) _trHashFile = mu::loadTranslation(x);  size_t inOutSize = size - 1; const char* pStr = mu::trc(_trHashFile, &ctStrIn[0], inOutSize); return td::String(pStr, inOutSize); } \
template <typename TSTR> inline QString trqt(const TSTR& str) const {td::UINT4 trHashFile = _trHashFile; if (trHashFile == 0) trHashFile = mu::loadTranslation(x); size_t inOutSize = str.length(); td::String strIn(str); const char* pStr = mu::trc(trHashFile, strIn.c_str(), inOutSize); return QString::fromUtf8(pStr); } \
template <typename TSTR> inline QString trqt(const TSTR& str) {if (_trHashFile == 0) _trHashFile = mu::loadTranslation(x); size_t inOutSize = str.length(); td::String strIn(str); const char* pStr = mu::trc(_trHashFile, strIn.c_str(), inOutSize); return QString::fromUtf8(pStr); } \
template <typename TSTR> inline td::String trs(const TSTR& str) const {td::UINT4 trHashFile = _trHashFile; if (trHashFile == 0) trHashFile = mu::loadTranslation(x); size_t inOutSize = str.length(); const char* pStr = mu::trc(trHashFile, str.c_str(), inOutSize); return td::String(pStr, inOutSize); }	\
template <typename TSTR> inline td::String trs(const TSTR& str){if (_trHashFile == 0) _trHashFile = mu::loadTranslation(x); size_t inOutSize = str.length(); const char* pStr = mu::trc(_trHashFile, str.c_str(), inOutSize); return td::String(pStr, inOutSize); }	\
template <typename TSTR> inline QString trqs(const TSTR& str) const {td::UINT4 trHashFile = _trHashFile; if (trHashFile == 0) trHashFile = mu::loadTranslation(x); size_t inOutSize = str.length(); const char* pStr = mu::trc(trHashFile, str.c_str(), inOutSize); return QString::fromUtf8(pStr, (int) inOutSize); } \
template <typename TSTR> inline QString trqs(const TSTR& str) {if (_trHashFile == 0) _trHashFile = mu::loadTranslation(x); size_t inOutSize = str.length(); const char* pStr = mu::trc(_trHashFile, str.c_str(), inOutSize); return QString::fromUtf8(pStr, (int) inOutSize); }
