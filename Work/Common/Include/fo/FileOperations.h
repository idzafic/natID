// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <mu/muLib.h>

#include <td/Types.h>
#include <cnt/ListSL.h>

#ifdef USE_BOOST_REGEX
include <boost/regex.hpp>
#else
#include <regex>
#endif

#include <fstream>
#include <iostream>

#include <cnt/StringBuilder.h>
#include <td/BufferString.h>
#include <mem/PointerDeleter.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef MU_WINDOWS
#include <windows.h>
#include <io.h>
#include <share.h>
#else
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#endif
#include <fo/Definitions.h>
#include <td/MutableString.h>
#include <cnt/PushBackVector.h>


//#ifdef MU_WINDOWS
#define FO_BINARY_OPEN_TRUNCATE	  (std::ios_base::binary | std::ios_base::trunc)
#define FO_BINARY_OPEN_EXISTING   (std::ios_base::binary | std::ios_base::in)

#define FO_TXT_OPEN_EXISTING   (std::ios_base::in)

#define FO_TEXT_OPEN_TRUNCATE      (std::ios_base::trunc)
//#else
//#define FO_BINARY_OPEN_TRUNCATE	  (std::ios_base::binary | std::ios_base::trunc)
//#define FO_BINARY_OPEN_EXISTING   (std::ios_base::binary)
//#endif


namespace fo
{

enum class CopyOption : td::BYTE { None = 0, SkipExisting, OverwriteExisting, CopySimLinks, CreateSimLinks };

//for using with getLine (fo::getLine(f, buffer))

typedef td::BufferString<td::UTF8, 256, 256>  LineSmall;
typedef td::BufferString<td::UTF8, 1024 * 4, 1024 * 4>  LineNormal;
typedef td::BufferString<td::UTF8, 1024 * 64, 1024 * 64>  LineLarge;

typedef std::ifstream InFile;
typedef std::ofstream OutFile;


typedef enum {FE_FILEISNOTVALID=0, FE_FILEDOESNOTEXIST, FE_FILEEXIST} FileExist;

typedef enum _FT{ FT_File, FT_Folder, FT_Other } FileType;

typedef enum { FT_GLOBAL_PATH = 0, FT_APPFOLDER, FT_HOMEFOLDER, FT_HOMELOCALDATAFOLDER, FT_TMPFOLDER, FT_DP_FOLDER, FT_REP_FOLDER, FT_TR_FOLDER, FT_RES_FOLDER, FT_GUI_FOLDER, FT_HELP_FOLDER } FolderType;

typedef struct _FileDesc
{
    fo::fs::path name;
    FileType type;
}FileDesc;

inline int createFolderContentList(const fs::path& folderPath, cnt::ListSL<FileDesc>& outList)
{
    int noErrors = 0;
    if (fs::is_directory(folderPath))
    {
        fs::directory_iterator end_iter;
        for (fs::directory_iterator dir_itr(folderPath); dir_itr != end_iter; ++dir_itr)
        {
            try
            {
                if (fs::is_directory(dir_itr->status()))
                {
                    FileDesc& fd = outList.push();
                    fd.name = dir_itr->path();
                    fd.type = FT_Folder;
                }
                else if (fs::is_regular_file(dir_itr->status()))
                {
                    FileDesc& fd = outList.push();
                    fd.name = dir_itr->path();
                    fd.type = FT_File;
                }
                else
                {
                    FileDesc& fd = outList.push();
                    fd.name = dir_itr->path();
                    fd.type = FT_Other;
                }
            }
            catch (const std::exception&)
            {
                ++noErrors;
            }
        }
    }
    return noErrors;
}

inline void getFileNameInCurrentFolder(const char* fileName, td::String& strCurrPath)
{
    fs::path currPath(fs::current_path() / fileName);
    currPath.make_preferred();
    strCurrPath = currPath.string();
}


inline void getCurrentPath(td::String& strCurrPath)
{
    fs::path currPath(fs::current_path());
    currPath.make_preferred();
    strCurrPath = currPath.string();
}

#ifdef MU_WINDOWS
inline void buildLongWinFileName(const td::String& inFilePath, td::StringUTF16& outLongFilePath)
{
    //fs::path filePath(inFilePath.c_str());
    //filePath = filePath.make_preferred();

    if (inFilePath.length() > 240)
    {
        //td::StringUTF16 str(filePath.string().c_str());
        td::StringUTF16 str(inFilePath);
        outLongFilePath = L"\\\\?\\";
        outLongFilePath += str;
    }
    else
    {
        outLongFilePath = inFilePath; // filePath.string().c_str();
    }
}

inline void buildLongWinFileName(const td::StringUTF16& inFilePath, td::StringUTF16& outLongFilePath)
{
    //fs::path path(inFilePath.c_str());

    if (inFilePath.unitLength() > 240)
    {
        //td::StringUTF16 str(path.string());
        //td::String str(inFilePath);
        outLongFilePath = L"\\\\?\\";
        outLongFilePath += inFilePath;
    }
    else
    {
        outLongFilePath = inFilePath; // path.string().c_str();
    }
}
#endif


template<typename T>
bool createBinaryFile(T& f, const fs::path& fileName)
{
#ifdef MU_WINDOWS
    try
    {
        td::String strFileName(fileName.string().c_str());
        td::StringUTF16 str;
        buildLongWinFileName(strFileName, str);

        f.open((wchar_t*)str.c_str(), FO_BINARY_OPEN_TRUNCATE);
    }
    catch(...)
    {
        return false;
    }
#else
            f.open(fileName.native(), FO_BINARY_OPEN_TRUNCATE);
#endif
    return f.is_open();
}

//	template<typename T>
//	bool openExistingBinaryFile(T& f, const fs::path& fileName)
//	{
//#ifdef MU_WINDOWS
//		try
//		{
//			td::String strFileName(fileName.string().c_str());
//			td::StringUTF16 str;
//			f.open((wchar_t*)str.c_str(), FO_BINARY_OPEN_EXISTING);
//		}
//		catch(...)
//		{
//			return false;
//		}
//#else
//		f.open(fileName, FO_BINARY_OPEN_EXISTING);
//#endif
//		return f.is_open();
//	}

template<typename T>
bool createTextFile(T& f, const td::String& fileName, bool writeUTFBOM = false)
{
#ifdef MU_WINDOWS
    try
    {
        td::StringUTF16 str;
        buildLongWinFileName(fileName, str);

        f.open((wchar_t*)str.c_str(), FO_TEXT_OPEN_TRUNCATE);
    }
    catch(...)
    {
        return false;
    }
#else
    f.open(fileName.c_str(), FO_TEXT_OPEN_TRUNCATE);
#endif
    bool isOpen = f.is_open();
    if (isOpen && writeUTFBOM)
    {
        //write utf8 signature
        unsigned char bom[] = {0xEF, 0xBB, 0xBF};
        f.write((const char*) bom, 3);
    }
    return isOpen;
}

template<typename T>
bool createBinaryFile(T& f, const td::String& fileName)
{
#ifdef MU_WINDOWS
    try
    {
        td::StringUTF16 str;
        buildLongWinFileName(fileName, str);

        f.open((wchar_t*)str.c_str(), FO_BINARY_OPEN_TRUNCATE);
    }
    catch(...)
    {
        return false;
    }
#else
            f.open(fileName.c_str(), FO_BINARY_OPEN_TRUNCATE);
#endif
    return f.is_open();
}

template<typename T>
bool createBinaryFile(T& f, const char* fName)
{
#ifdef MU_WINDOWS
    try
    {
        td::String fileName(fName);
        td::StringUTF16 str;
        buildLongWinFileName(fileName, str);

        f.open((wchar_t*)str.c_str(), FO_BINARY_OPEN_TRUNCATE);
    }
    catch (...)
    {
        return false;
    }
#else
    f.open(fName, FO_BINARY_OPEN_TRUNCATE);
#endif
    return f.is_open();
}

//MAINUTILS_API bool createSharedReadFile(std::ofstream& f, const td::String& fileName);
//MAINUTILS_API void unlockSharedReadFile(std::ofstream& f);

template <typename T>
bool createSharedReadFile(T& f, const td::String& fileName, int& fid)
{
    if (!openFile(f, fileName))
        return false;
    fid = f.filedesc();
    return true;
    
//#ifdef MU_WINDOWS
//		HANDLE hf = _get_osfhandle(f.fd());
//		int status = LockFile(hf, 0, 0, 0, 0);
//#else
//		int status = lockf(f.fd, F_TLOCK, 0)
//#endif
    //fcntl()

//#ifdef MU_WINDOWS
//		td::StringUTF16 str;
//		buildLongWinFileName(fileName, str);
//		fid = _wsopen(str.c_str(), O_TRUNC | _O_CREAT | _O_TEXT | _O_SEQUENTIAL | _O_WRONLY, _SH_DENYWR, S_IREAD | _S_IWRITE);
//
//#else
//
//		fid = open(fileName.c_str(), _O_WRONLY);
//		int status = lockf(fildes, F_TLOCK, 0);
//#endif
//		if (fid < 0)
//			return false;
//
//		f.attach(fid);
//		return f.is_open();
}
        

template<typename T>
bool openExistingBinaryFile(T& f, const td::String& fileName)
{
#ifdef MU_WINDOWS
    try
    {
        td::StringUTF16 str;
        buildLongWinFileName(fileName, str);
        f.open((wchar_t*)str.c_str(), FO_BINARY_OPEN_EXISTING);
    }
    catch(...)
    {
        return false;
    }
#else
    f.open(fileName.c_str(), FO_BINARY_OPEN_EXISTING);
#endif
    return f.is_open();
}

template<typename T>
bool openExistingBinaryFile(T& f, const char* fileName)
{
#ifdef MU_WINDOWS
try
{
    td::StringUTF16 str;
    buildLongWinFileName(fileName, str);
    f.open((wchar_t*)str.c_str(), FO_BINARY_OPEN_EXISTING);
}
catch(...)
{
    return false;
}
#else
f.open(fileName, FO_BINARY_OPEN_EXISTING);
#endif
return f.is_open();
}

template <typename T>
bool openFile(T& f, const fs::path& fileName)
{
#ifdef MU_WINDOWS
    try
    {
        td::String strFileName(fileName.string().c_str());
        td::StringUTF16 str;
        buildLongWinFileName(strFileName, str);
        f.open((wchar_t*)str.c_str());
    }
    catch(...)
    {
        return false;
    }
#else
    f.open(fileName);
#endif
    return f.is_open();
}

template<typename T>
bool openFile(T& f, const fs::path& fileName, std::ios_base::openmode mode)
{
#ifdef MU_WINDOWS
    try
    {
        td::String strFileName(fileName.string().c_str());
        td::StringUTF16 str;
        buildLongWinFileName(strFileName, str);
        f.open((wchar_t*)str.c_str(), mode);
    }
    catch(...)
    {
        return false;
    }
#else
    f.open(fileName, mode);
#endif
    return f.is_open();
}


template <typename T>
bool openFile(T& f, const td::UTF8* fileName)
{
#ifdef MU_WINDOWS
    try
    {
        td::StringUTF16 strFileName(fileName);
        td::StringUTF16 str;
        buildLongWinFileName(strFileName, str);
        f.open((wchar_t*)str.c_str());
    }
    catch(...)
    {
        return false;
    }

#else
    try
    {
        f.open(fileName);
    }
    catch(...)
    {
        return false;
    }
#endif
    return f.is_open();
}

template <typename T>
bool openFile(T& f, const td::UTF8* fileName, std::ios_base::openmode mode)
{
#ifdef MU_WINDOWS
    try
    {
        td::String strFileName(fileName);
        td::StringUTF16 str;
        buildLongWinFileName(strFileName, str);
        f.open((wchar_t*)str.c_str(), mode);
    }
    catch(...)
    {
        return false;
    }

#else
    try
    {
        f.open(fileName, mode);
    }
    catch(...)
    {
        return false;
    }
#endif
    return f.is_open();
}

template <typename T>
bool openFile(T& f, const td::String& fileName)
{
#ifdef MU_WINDOWS
    try
    {
        td::StringUTF16 str;
        buildLongWinFileName(fileName, str);
        f.open((wchar_t*)str.c_str());
    }
    catch(...)
    {
        return false;
    }

#else
    try
    {
            f.open(fileName.c_str());
    }
    catch(...)
    {
        return false;
    }
#endif
    return f.is_open();
}

template <typename T>
bool openFile(T& f, const td::String& fileName, std::ios_base::openmode mode)
{
#ifdef MU_WINDOWS
    try
    {
        td::StringUTF16 str;
        buildLongWinFileName(fileName, str);
        f.open((wchar_t*)str.c_str(),  mode);
    }
    catch(...)
    {
        return false;
    }

#else
    try
    {
            f.open(fileName.c_str(), mode);
    }
    catch(...)
    {
        return false;
    }
#endif
    return f.is_open();
}

template <typename T>
bool openFile(T& f, const td::UTF16* fileName)
{
#ifdef MU_WINDOWS
    try
    {
        td::StringUTF16 str;
        buildLongWinFileName(td::StringUTF16(fileName), str);
        f.open((wchar_t*)str.c_str());
    }
    catch(...)
    {
        return false;
    }

#else
    try
    {
        td::String str(fileName);
        f.open(str.c_str());
    }
    catch(...)
    {
        return false;
    }
#endif
    return f.is_open();
}

template <typename T>
bool openFile(T& f, const td::UTF16* fileName, std::ios_base::openmode mode)
{
#ifdef MU_WINDOWS
    try
    {
        td::StringUTF16 str;
        td::StringUTF16 fileName16(fileName);
        buildLongWinFileName(fileName16, str);
        f.open((wchar_t*)str.c_str(), mode);
    }
    catch(...)
    {
        return false;
    }

#else
    try
    {
        td::String str(fileName);
        f.open(str.c_str(), mode);
    }
    catch(...)
    {
        return false;
    }
#endif
    return f.is_open();
}

template <typename T>
bool openFile(T& f, const td::UTF32* fileName)
{
#ifdef MU_WINDOWS
    try
    {
        td::String str8(fileName);
        td::StringUTF16 str16;
        buildLongWinFileName(str8, str16);
        f.open((wchar_t*) str16.c_str());
    }
    catch(...)
    {
        return false;
    }
#else
    try
    {
        td::String str(fileName);
        f.open(str.c_str());
    }
    catch(...)
    {
        return false;
    }
#endif
    return f.is_open();
}

template <typename T>
bool openFile(T& f, const td::UTF32* fileName, std::ios_base::openmode mode)
{
#ifdef MU_WINDOWS
    try
    {
        td::String str8(fileName);
        td::StringUTF16 str16;
        buildLongWinFileName(str8, str16);
        f.open((wchar_t*) str16.c_str(), mode);
    }
    catch(...)
    {
        return false;
    }
#else
    try
    {
        td::String str(fileName);
        f.open(str.c_str(), mode);
    }
    catch(...)
    {
        return false;
    }
#endif
    return f.is_open();
}


//inline bool openFileNoBlockDelete(std::ofstream& f, const std::filesystem::path& path) 
//{
//#ifdef MU_WINDOWS
//    // Windows-specific code using CreateFile
//    td::StringUTF16 str16(path.string());
//    HANDLE hFile = CreateFileW(
//        str16.c_str(),            // File name
//        GENERIC_WRITE,           // Desired access
//        FILE_SHARE_READ, // Sharing mode (no FILE_SHARE_DELETE)
//        NULL,                    // Security attributes
//        CREATE_ALWAYS,           // Creates or overwrites the file
//        FILE_ATTRIBUTE_NORMAL,   // File attributes
//        NULL                     // Template file handle
//    );
//
//    if (hFile == INVALID_HANDLE_VALUE) 
//    {
//        std::cerr << "Failed to open file on Windows." << std::endl;
//        return false;  // Return an empty ofstream
//    }
//
//    // Get the file descriptor from the HANDLE
//    int fd = _open_osfhandle(reinterpret_cast<intptr_t>(hFile), _O_WRONLY);
//
//    if (fd == -1) {
//        std::cerr << "Failed to get file descriptor on Windows." << std::endl;
//        CloseHandle(hFile);
//        return false;  // Return an empty ofstream
//    }
//
//    // Create an ofstream using the file descriptor
//    f.rdbuf()
//    f.attach(fd);
//    return true;
//
//#else
//    // Linux/Unix-specific code using open and flock
//    int fd = open(path.c_str(), O_WRONLY | O_CREAT, 0644);
//    if (fd == -1) {
//        std::cerr << "Failed to open file on Unix/Linux." << std::endl;
//        return std::ofstream();  // Return an empty ofstream
//    }
//
//    // Apply an exclusive lock to prevent others from writing
//    if (flock(fd, LOCK_EX | LOCK_NB) == -1) {
//        std::cerr << "Failed to lock file on Unix/Linux." << std::endl;
//        close(fd);
//        return std::ofstream();  // Return an empty ofstream
//    }
//
//    // Create an ofstream using the file descriptor
//    FILE* file = fdopen(fd, "w");
//    if (file == nullptr) {
//        std::cerr << "Failed to associate file descriptor with FILE* on Unix/Linux." << std::endl;
//        close(fd);
//        return std::ofstream();  // Return an empty ofstream
//    }
//
//    std::ofstream ofs;
//    ofs.attach(fileno(file));
//    return ofs;
//
//#endif
//}


template <typename T>
void writeString(T& f, const td::UTF8* txt)
{
    if (txt)
        f << txt;
        //f.write(txt, strlen(txt));
}

template <typename T>
void writeString(T& f, const td::UTF16* txt)
{
    if (txt)
    {
        td::String str(txt);
        int nLen = str.length();
        if (nLen > 0)
            f.write(str.c_str(), nLen);
    }
}

template <typename T>
void writeString(T& f, const td::UTF32* txt)
{
    if (txt)
    {
        td::String str(txt);
        int nLen = str.length();
        if (nLen > 0)
            f.write(str.c_str(), nLen);
    }
}

template <typename T>
void writeString(T& f, const td::String& txt, bool isBinary = true)
{
    int nLen = txt.length();
    if (nLen > 0)
    {
        if (isBinary)
            f.write(txt.c_str(), nLen);
        else
            f.write(txt.c_str(), nLen-1);
    }
}

template <typename T>
void writeString(T& f, const td::StringUTF16& txt)
{
    td::String str(txt);
    int nLen = str.length();
    if (nLen > 0)
        f.write(str.c_str(), nLen);
}

template <typename T>
void writeString(T& f, const td::StringUTF32& txt)
{
        td::String str(txt);
    int nLen = str.length();
    if (nLen > 0)
        f.write(str.c_str(), nLen);
}

template <typename T>
void writeString(T& f, const td::MidStringUTF8& txt)
{
    int nLen = txt.length();
    if (nLen > 0)
        f.write(txt.c_str(), nLen);
}

template <typename T>
void writeString(T& f, const td::MidStringUTF16& txt)
{
        td::MidStringUTF8 str(txt);
    int nLen = str.length();
    if (nLen > 0)
        f.write(str.c_str(), nLen);
}

template <typename T>
void writeString(T& f, const td::MidStringUTF32& txt)
{
        td::MidStringUTF8 str(txt);
    int nLen = str.length();
    if (nLen > 0)
        f.write(str.c_str(), nLen);
}
template <class TFILE, class TBUILDER>
void writeBuilder(TFILE& f, TBUILDER& builder)
{
    builder.serialize(f);
    //typename TBUILDER::const_iterator it(builder.begin());
    //typename TBUILDER::const_iterator itEnd(builder.end());
    //while (it != itEnd)
    //{
    //	writeString(f, *it);
    //	++it;
    //}
}


template <class TFILE, class TBUILDER>
void writeBinaryBuilder(TFILE& f, TBUILDER& builder)
{
    builder.serialize(f);
    //typename TBUILDER::const_iterator it(builder.begin());
    //typename TBUILDER::const_iterator itLast(builder.lastIter());

    //while (it != itLast)
    //{
    //	f.write(builder.getDataPtr(it), builder.getBuffLen());
    //	++it;
    //}
    //if (itLast != builder.end())
    //	f.write(builder.getDataPtr(itLast), builder.getLastLen());
}

template <class TFILE, class TBUILDER>
void writeBuilder(TFILE& f, TBUILDER& builder, const char* chSep, bool putEndLine = true)
{
    typename TBUILDER::iterator it(builder.begin());
    typename TBUILDER::const_iterator itEnd(builder.lastIter());
    int i = 0;
    while (it != itEnd)
    {
        if (chSep && (i > 0))
            f << chSep;
        else
            ++i;

        writeString(f, *it);
        ++it;
    }
    if (putEndLine && (i > 0))
        f << std::endl;
}

//template <class TFILE, class TBUILDER>
//void writeBinaryBuilder(TFILE& f, TBUILDER& builder)
//{
//	typename TBUILDER::const_iterator it = builder.begin();
//	typename TBUILDER::const_iterator itEnd = builder.end();
//	while (it != itEnd)
//	{
//		f.write((const char*) it->c_str(), it->length());
//		++it;
//	}
//}

//template <class TFILE, class TBUILDER>
//void writeBuilder(TFILE& f, TBUILDER& builder, const char* chSep, bool putEndLine= true)
//{
//	typename TBUILDER::iterator it = builder.begin();
//	typename TBUILDER::const_iterator itEnd = builder.end();
//	int i=0;
//	while (it != itEnd)
//	{
//		if (chSep && (i > 0) )
//			f << chSep;
//		else
//			++i;

//		writeString(f, *it);
//		++it;
//	}
//	if (putEndLine && (i > 0) )
//		f << std::endl;
//}

template <typename T>
std::streamoff tellReadPos(T& f)
{
    std::streamoff toRet = f.tellg();
    if (toRet < 0)
    {
        f.seekg( -1, std::ios_base::end );
        toRet = f.tellg();
        if (toRet < 0)
        {
            return 0;
        }
        //toRet++;
    }
    return toRet;
}

template <typename T>
std::streamoff tellWritePos(T& f)
{
    std::streamoff toRet = f.tellp();
    if (toRet < 0)
        return 0;
    return toRet;
}

template <typename T>
void seekReadPos(T& f, size_t pos)
{
    f.seekg(pos);
}

template <typename T>
void seekWritePos(T& f, size_t pos)
{
    f.seekp(pos);
}


inline void seek(std::istream& f, size_t pos)
{
    f.seekg(pos);
}

inline void seek(std::ostream& f, size_t pos)
{
    f.seekp(pos);
}

inline std::streamoff tell(std::istream& f)
{
    std::streamoff toRet = f.tellg();
    if (toRet < 0)
    {
        f.seekg( -1, std::ios_base::end );
        toRet = f.tellg();
        if (toRet < 0)
        {
            return 0;
        }
        //toRet++;
    }
    return toRet;
}

inline std::streamoff tell(std::ostream& f)
{
    std::streamoff toRet = f.tellp();
    if (toRet < 0)
        return 0;
    return toRet;
}


//template <typename T>
//int read(T& f, char* buffer, unsigned int buffSize)
//{
//	try
//	{
//		return (int) f.readsome(buffer, buffSize);
//	}
//	catch (...)
//	{
//		return -1;
//	}
//	return 0;
//}

template <typename T>
std::streamsize read(T& f, char* buffer, unsigned int buffSize)
{
    //std::streamsize p1= tellReadPos(f);
    f.read(buffer, buffSize);
    std::streamsize sz = f.gcount();
    if (sz > 0)
        return sz;
    if ( f.good())
        return f.gcount();
    if (f.eof())
        return 0;
    return -1;

//	unsigned int p2= tellReadPos(f);
//	if (p2 > p1)
//		return (int) (p2 - p1);
        //return 0;
}

template <class OSTREAM>
std::streamsize write(OSTREAM& f, const char* buffer, std::streamsize buffSize)
{
    f.write(buffer, buffSize);

    if (f.good())
        return buffSize;
    return -1;
}

template <typename T>
bool writeAll(T& f, const char* buffer, size_t buffSize)
{
    f.write(buffer, buffSize);

    if (f.good())
        return true;
    return false;
}

template <class T_STREAM, class T_CHAR, int NINITLEN, int INCREMENT>
bool getLine(T_STREAM& f, td::BufferString<T_CHAR, NINITLEN, INCREMENT>& s)
{
    typename std::ios_base::iostate _State = std::ios_base::goodbit;
    bool _Changed = false;

    if (f.good() && !f.eof())
    {
        // state okay, extract characters
        s.erase();
        T_CHAR END_LINE = TD_TO_NEWLINE;
        T_CHAR BEG_LINE = TD_TO_LINEBEG;
        T_CHAR END_OF_FILE = TD_EOF;
        T_CHAR ch = f.rdbuf()->sgetc();

        for (; ; ch = f.rdbuf()->snextc())
        {
            if (ch == END_OF_FILE)
            {
                // end of file, quit
                //check if it is really eof (some encodings placing FF chars)
                ch = f.rdbuf()->snextc();
                if (ch == TD_EOF)
                {
                    _State |= std::ios_base::eofbit;
                    if (s.length() == 0)
                    {
                        f.setstate(_State);
                        return false;
                    }
                    break;
                }
                s += ch;
                _Changed = true;
            }
            //else if (ch == BEG_LINE || ch == END_LINE)
            //{
            //	// got a delimiter, discard it, adn return
            //	_Changed = true;
            //	f.rdbuf()->sbumpc();
            //	break;
            //}
            else if (ch == BEG_LINE)
            {
                // got a delimiter, discard it
                _Changed = true;
                //f.rdbuf()->sbumpc();
            }
            else if (ch == END_LINE)
            {
                // got a delimiter, discard it and return
                _Changed = true;
                f.rdbuf()->sbumpc();
                break;
            }
            else if (s.max_size() <= s.size())
            {
                // string too large, quit
                _State |= std::ios_base::failbit;
                break;
            }
            else
            {	// got a character, add it to string
                s += ch;
                _Changed = true;
            }
        }
    }
    else
        return false; //eof of bad file indicator

    if (!_Changed)
        _State |= std::ios_base::failbit;

    f.setstate(_State);

    return true;
}

//fromPosition is input/output argument
template <class ISTREAM>
inline bool getLine(ISTREAM& f, td::MutableString& str, td::UINT4& pos)
{
    typename std::ios_base::iostate _State = std::ios_base::goodbit;
    bool _Changed = false;

    if (f.good() && !f.eof())
    {
        // state okay, extract characters
        if (str.capacity() == 0)
            str.reserve(4096);
        str.reset();
        
        char END_LINE = TD_TO_NEWLINE;
        char BEG_LINE = TD_TO_LINEBEG;
        char END_OF_FILE = TD_EOF;
        char ch = f.rdbuf()->sgetc();

        for (; ; ch = f.rdbuf()->snextc())
        {
            if (ch == END_OF_FILE)
            {
                // end of file, quit
                //check if it is really eof (some encodings placing FF chars)
                ch = f.rdbuf()->snextc();
                if (ch == TD_EOF)
                {
                    _State |= std::ios_base::eofbit;
                    if (str.length() == 0)
                    {
                        f.setstate(_State);
                        return false;
                    }
                    break;
                }
                str.append(ch);
                ++pos;
                _Changed = true;
            }
            else if (ch == BEG_LINE)
            {
                // got a delimiter, discard it
                _Changed = true;
                ++pos;
            }
            else if (ch == END_LINE)
            {
                // got a delimiter, discard it and return
                _Changed = true;
                f.rdbuf()->sbumpc();
                ++pos;
                break;
            }
            else
            {    // got a character, add it to string
                str.append(ch);
                ++pos;
                _Changed = true;
            }
        }
    }
    else
        return false; //eof of bad file indicator

    if (!_Changed)
        _State |= std::ios_base::failbit;

    f.setstate(_State);

    return true;
}
template <class ISTREAM, typename TPOS>
inline void goToPos(ISTREAM& f, TPOS pos, bool clearState)
{
    if (clearState)
        f.clear();
    f.seekg(pos, std::ios::beg);
}

[[nodiscard]]
inline bool fileExists(const td::String& fileName)
{
#ifdef MU_WINDOWS
    td::StringUTF16 strLongFolderName;
    buildLongWinFileName(fileName, strLongFolderName);

    DWORD flag = ::GetFileAttributesW(strLongFolderName.c_str());
    bool bExists = (flag != INVALID_FILE_ATTRIBUTES);
    bool isFile = ( (flag & FILE_ATTRIBUTE_DIRECTORY) == 0);

    return bExists && isFile;
#else
    //td::StringUTF16 str(fileName);
    fo::fs::path filePath(fileName.c_str());
    if (fo::fs::exists(filePath) && fo::fs::is_regular_file(filePath))
        return true;
    return false;
#endif
}

[[nodiscard]]
inline bool fileExists(const char* fileName)
{
#ifdef MU_WINDOWS
    td::StringUTF16 strLongFolderName;
    td::StringUTF16 strInFileName(fileName);
    buildLongWinFileName(strInFileName, strLongFolderName);

    DWORD flag = ::GetFileAttributesW(strLongFolderName.c_str());
    bool bExists = (flag != INVALID_FILE_ATTRIBUTES);
    bool isFile = ( (flag & FILE_ATTRIBUTE_DIRECTORY) == 0);

    return bExists && isFile;
#else
    //td::StringUTF16 str(fileName);
    fo::fs::path filePath(fileName);
    if (fo::fs::exists(filePath) && fo::fs::is_regular_file(filePath))
        return true;
    return false;
#endif
}

[[nodiscard]]
inline bool fileExists(const fo::fs::path& filePath)
{
    if (fo::fs::exists(filePath) && fo::fs::is_regular_file(filePath))
        return true;
    return false;
}

[[nodiscard]]
inline bool folderExists(const td::String& folderName)
{
#ifdef MU_WINDOWS
    td::StringUTF16 strLongFolderName;
    buildLongWinFileName(folderName, strLongFolderName);

    DWORD flag = ::GetFileAttributesW(strLongFolderName.c_str());
    bool bExists = (flag != INVALID_FILE_ATTRIBUTES);
    bool isDirectory = ( (flag & FILE_ATTRIBUTE_DIRECTORY) != 0);

    return bExists && isDirectory;
#else
    try
    {
        //td::StringUTF16 str(folderName);
        fo::fs::path folderPath(folderName.c_str());
        if (fo::fs::exists(folderPath) && fo::fs::is_directory(folderPath))
            return true;
        return false;
    }
    
    catch (...)
    {
    }
    
    return false;
#endif
}

[[nodiscard]]
inline bool folderExists(const char* folderNameCStr)
{
#ifdef MU_WINDOWS
    td::StringUTF16 strLongFolderName;
    td::StringUTF16 folderName(folderNameCStr);
    buildLongWinFileName(folderName, strLongFolderName);

    DWORD flag = ::GetFileAttributesW(strLongFolderName.c_str());
    bool bExists = (flag != INVALID_FILE_ATTRIBUTES);
    bool isDirectory = ( (flag & FILE_ATTRIBUTE_DIRECTORY) != 0);

    return bExists && isDirectory;
#else
    try
    {
        fo::fs::path folderPath(folderNameCStr);
        if (fo::fs::exists(folderPath) && fo::fs::is_directory(folderPath))
            return true;
        return false;
    }
    
    catch (...)
    {
    }
    
    return false;
#endif
}

[[nodiscard]]
inline bool folderExists(const fo::fs::path& folderPath)
{
    if (fo::fs::exists(folderPath) && fo::fs::is_directory(folderPath))
        return true;
    return false;
}

inline bool deleteFile(const td::String& fileName)
{
    try
    {
#ifdef MU_WINDOWS
        td::StringUTF16 strLongFolderName;
        buildLongWinFileName(fileName, strLongFolderName);
        bool toRet = (::DeleteFileW(strLongFolderName.c_str()) != 0);
#else
        fo::fs::path filePath(fileName.c_str());
        bool toRet = fo::fs::remove(filePath);
#endif
        return toRet;
    }
    catch (...)
    {
        return false;
    }

    return false;
}

inline bool deleteFile(const fs::path& fileNamePath)
{
    td::String str;
    str.fromKnownString(fileNamePath.string().c_str(), fileNamePath.string().length());
    return deleteFile(str);
}

inline bool createFolder(const td::String& folderName)
{
    if (!folderExists(folderName))
    {
        try
        {
#ifdef MU_WINDOWS
            td::StringUTF16 strLongFolderName;
            buildLongWinFileName(folderName, strLongFolderName);
            bool toRet = (::CreateDirectoryW(strLongFolderName.c_str(), 0) != 0);
#else
            fo::fs::path path(folderName.c_str());
            bool toRet = fo::fs::create_directory(path);
#endif
            return toRet;
        }
        catch (...)
        {
            return false;
        }

    }
    return true;
}

inline bool createFolder(const char* folderNameCStr)
{
    if (!folderExists(folderNameCStr))
    {
        try
        {
#ifdef MU_WINDOWS
            td::StringUTF16 strLongFolderName;
            td::StringUTF16 folderName(folderNameCStr);
            buildLongWinFileName(folderName, strLongFolderName);
            bool toRet = (::CreateDirectoryW(strLongFolderName.c_str(), 0) != 0);
#else
            fo::fs::path path(folderNameCStr);
            bool toRet = fo::fs::create_directory(path);
#endif
            return toRet;
        }
        catch (...)
        {
            return false;
        }

    }
    return true;
}

inline bool createFolder(const fs::path& path)
{
    td::String str;
    str.fromKnownString(path.string().c_str(), path.string().length());
    return createFolder(str);
}

inline bool createFolders(const fo::fs::path& pathRoot, const char* foldersToCreate)
{
    if (!fo::fs::exists(pathRoot))
        return false;
    fo::fs::path toCreate(pathRoot / foldersToCreate);
    return fo::fs::create_directories(toCreate);
}

inline bool createFolders(const fo::fs::path& folders)
{
    if (fo::fs::exists(folders))
        return true;
    return fo::fs::create_directories(folders);
}

inline bool deleteFolder(const fs::path folderPath, bool deleteContent = true)
{
    try
    {
        //check content of folder
        if (deleteContent)
        {
            cnt::ListSL<FileDesc> content;
            //fs::path fullPath(folderName.c_str());
            createFolderContentList(folderPath, content);
            auto it(content.begin());
            auto itEnd(content.end());
            while (it != itEnd)
            {
                if (it->type == FT_Folder)
                    deleteFolder(it->name);
                else if (it->type == FT_File)
                    deleteFile(it->name);
                ++it;
            }
        }

#ifdef MU_WINDOWS
        td::String folderName = folderPath.string();
        td::StringUTF16 strLongFolderName;
        buildLongWinFileName(folderName, strLongFolderName);
        bool toRet = (::RemoveDirectoryW(strLongFolderName.c_str()) != 0);
#else

        bool toRet = fo::fs::remove(folderPath);
#endif
        return toRet;
    }
    catch (...)
    {
        return false;
    }


    return false;
}

inline bool deleteFolder(const td::String& folderName, bool deleteContent=true)
{
    fs::path folderPath(folderName.c_str());
    return deleteFolder(folderPath, deleteContent);
}

MAINUTILS_API void deleteFilesInFolder(const std::filesystem::path& folderPath, const char* fileNamesThatStartWith);

inline void buildFileName(const char* folderName, const char* fileName, td::String& outFileName)
{
    fs::path path1(folderName);
    fs::path path(path1 / fileName);
    path.make_preferred();
    outFileName = path.string();
}

inline void buildFileName(const td::String& folderName, const td::String& fileName, td::String& outFileName)
{
    fs::path path1(folderName.c_str());
    fs::path path(path1 / fileName.c_str());
    outFileName = path.string();
}

inline const char* getTmpFileName(char* buff, size_t buffLen)
{
#ifdef MU_WINDOWS
    tmpnam_s (buff, buffLen);
#else
        char tmp[]="TMP_XXXXXXXXXX.tmp";
        int iRet = mkstemp(tmp);
        if (iRet < 0)
            return nullptr;

        char* pTmpName = tmp;
        size_t tmpLen = strlen(pTmpName);
        if (buffLen > tmpLen)
        {
            strcpy_s(buff, buffLen, pTmpName);
            return buff;
        }

        return nullptr;
#endif
    return buff;
}

inline void getTmpFileName(td::String& outFileName)
{
    char tmp[1024];
    outFileName = getTmpFileName(tmp, 1024);
}


inline void getDirectoryFromFullPath(const td::String& fullPathName, td::String& outDirectory)
{
    fs::path filePath(fullPathName.c_str());
    outDirectory =filePath.parent_path().string();
}

inline void getBaseNameFromFullPath(const td::String& fullPathName, td::String& outBaseName)
{
    fs::path filePath(fullPathName.c_str());
#ifdef USE_BOOST_FILESYSTEM
    outBaseName = fs::basename(filePath);
#else
    outBaseName = filePath.stem().string();
#endif
}

inline void getFileNameFromFullPath(const td::String& fullPathName, td::String& outFileName)
{
    fs::path filePath(fullPathName.c_str());
    outFileName = filePath.filename().string();
}


template <typename TBYTES, int INITSIZE, int INCSIZE>
bool readBytesFromFile(const td::String& fileName, td::BufferString<TBYTES, INITSIZE, INCSIZE>& buffer)
{
    buffer.erase();
    //TBYTES* fileBuf = new TBYTES[1024];
    //mem::PointerDeleter<TBYTES> _p(fileBuf);
    TBYTES fileBuf[1024];
    std::ifstream f;
    if (!openExistingBinaryFile(f, fileName))
        return false;
    
    int bytesRead = (int) read(f, (char*) fileBuf, 1024);
    while (bytesRead > 0)
    {
        buffer.append(bytesRead, fileBuf);
        bytesRead = (int) read(f, (char*) fileBuf, 1024);
    }

    f.close();
    return true;
}

inline td::LUINT8 getFileSize(const char* fileName)
{
    fs::path filePath(fileName);
    if (fo::fileExists(filePath))
        return (td::LUINT8) fs::file_size(filePath);
    return 0;
}

inline td::LUINT8 getFileSize(std::istream& f)
{
    f.seekg(0, std::ios_base::end);//seek to end
    //now get current position as length of file
    td::LUINT8 toRet = f.tellg();
    //move cursor to the first position
    f.seekg(0, std::ios_base::beg);//seek to end
    return toRet;
}

inline td::LUINT8 getFileSize(std::ostream& f)
{
    f.seekp(0, std::ios_base::end);//seek to end
    td::LUINT8 toRet = f.tellp();
    //now get current position as length of file
    f.seekp(0, std::ios_base::beg);//seek to end
    return toRet;
}

template <class TCONTAINER>
void getFileNamesInFolder(TCONTAINER& outFileNames, const td::String& folderName, const char* pExtension = 0)
{
    fs::path dir_path(folderName.c_str());
    fs::directory_iterator end_it;

    // loop through each file in the directory
    for(fs::directory_iterator it(dir_path); it != end_it; ++it)
    {
        // if it's not a directory and its extension is as given [2]
        if(fs::is_directory(it->status()))
            continue;

        if (pExtension != 0)
        {
            auto strExt = it->path().extension();
            if (strExt == pExtension)
            {
                // store filename for later use
                //std::string strTmp = it->path().string();
                td::String str(it->path().string().c_str());
                outFileNames.push_back(str);
            }
        }
        else
        {
            outFileNames.push_back( it->path().string().c_str() );
        }
    }
}

template <class TCONTAINER>
void getFileNamesInFolder(TCONTAINER& outFileNames, const fo::fs::path& folderPath, const char* pExtension = 0)
{
//    fs::path dir_path(folderName.c_str());
    fs::directory_iterator end_it;

    // loop through each file in the directory
    for(fs::directory_iterator it(folderPath); it != end_it; ++it)
    {
        // if it's not a directory and its extension is as given [2]
        if(fs::is_directory(it->status()))
            continue;

        if (pExtension != 0)
        {
#ifdef USE_BOOST_FILE_SYSTEM
            if (fs::extension(it->path()) == pExtension)
#else
            if (it->path().extension() == pExtension)
#endif
            {
                // store filename for later use
                //std::string strTmp = it->path().string();
                td::String str(it->path().string().c_str());
                outFileNames.push_back(str);
            }
        }
        else
        {
            outFileNames.push_back( it->path().string().c_str() );
        }
    }
}


inline td::String convertDOSWildCardToRegex(const td::String& dosWildcard)
{
    cnt::StringBuilderSmall strBuilder;

    for (char ch : dosWildcard) 
    {
        switch (ch) {
            case '*':
                strBuilder << ".*";
                break;
            case '?':
                strBuilder << '.';
                break;
            case '.':
            case '^':
            case '$':
            case '+':
            case '|':
            case '(':
            case ')':
            case '[':
            case ']':
            case '{':
            case '}':
            case '\\':
                // Escape special characters in regular expressions
                strBuilder << '\\';
                strBuilder << ch;
                break;
            default:
                strBuilder << ch;
        }
    }

    return strBuilder.toString();
}

//inline void convertWildCardToRegex(const td::String& strWildCard, td::String& regexString)
//{
//    
//    sdfd
//    td::String regexString2 = strWildCard.replace("*", "\\*");
//    td::String regexString3 = regexString2.replace("?", "\\?");
//    td::String regexString4 = regexString3.replace(".", "\\.");
////    td::String regexString5 = regexString4.replace("\\*", ".*");
//    regexString = regexString4.replace("\\?", ".*");
//}

template <class TCONTAINER>
void getFilePathsInFolder(TCONTAINER& vOut, const fs::path& folderPath, const char* strExtension, bool resetContent = true)
{
    if (resetContent)
    {
        if (vOut.capacity() == 0)
            vOut.reserve(64);
        else
            vOut.reset();
    }

    fs::directory_iterator end_itr; // Default ctor yields past-the-end
    for( fs::directory_iterator i( folderPath ); i != end_itr; ++i )
    {
        // Skip if not a file
        if( !fs::is_regular_file( i->status() ) )
            continue;

        //if (fs::is_directory(i->path()));
        //	continue;

        std::string strExt (i->path().extension());
        fs::path fileName(i->path().filename());
        td::String strFileExt;
        strFileExt.fromKnownString(strExt.c_str(), strExt.length());
        int nLen = strFileExt.length();
        if (nLen != 4)
            continue;
        
        if (strFileExt.cCompareNoCase(strExtension) == 0)
        {
            vOut.push_back(i->path());
        }
    }
}

template <class TCONTAINER>
bool getFileNamesInFolderWildCard(TCONTAINER& outFileNames, const td::String& folderName, const char* pWC)
{
    std::string target_path( folderName.c_str() );

    td::String regexStr = convertDOSWildCardToRegex(td::String(pWC));
#ifdef USE_BOOST_REGEX
    boost::regex my_filter( regexStr.c_str() );
#else
    std::regex my_filter(regexStr.c_str());
#endif

    try 
    {
        fs::directory_iterator end_itr; // Default ctor yields past-the-end
        for( fs::directory_iterator i( target_path ); i != end_itr; ++i )
        {
            // Skip if not a file
            if( !fs::is_regular_file( i->status() ) )
                continue;
    #ifdef USE_BOOST_REGEX
            boost::smatch what;
            // Skip if no match
            if (!boost::regex_match(i->path().string(), what, my_filter))
                continue;
    #else
            std::smatch what;
            // Skip if no match
            //std::string str1(i->path().string());
            std::string str1(i->path().filename().string());
            if (!std::regex_match(str1, what, my_filter))
                continue;
    #endif

            // File matches, store it
            td::String fn(str1);
            outFileNames.push_back(fn);
        }
    } 
    catch (...)
    {
        return false;
    }
    return true;
}

template <class TCONTAINER>
void getFileNamesInFolderWildCard_Old(TCONTAINER& outFileNames, const td::String& folderName, const char* pWC)
{
    std::string target_path(folderName.c_str());

    td::String regexStr = convertDOSWildCardToRegex(td::String(pWC));
#ifdef USE_BOOST_REGEX
    boost::regex my_filter(regexStr.c_str());
#else
    std::regex my_filter(regexStr.c_str());
#endif

    fs::directory_iterator end_itr; // Default ctor yields past-the-end
    for (fs::directory_iterator i(target_path); i != end_itr; ++i)
    {
        // Skip if not a file
        if (!fs::is_regular_file(i->status()))
            continue;
#ifdef USE_BOOST_REGEX
        boost::smatch what;
        // Skip if no match
        if (!boost::regex_match(i->path().string(), what, my_filter))
            continue;
#else
        std::smatch what;
        // Skip if no match
        std::string str1(i->path().string());

        if (!std::regex_match(str1, what, my_filter))
            continue;
#endif

        // File matches, store it
        td::String fn(i->path().string());
        outFileNames.push_back(fn);
    }
}

template<typename TSTR>
inline bool loadBinaryFileAtOnce(const TSTR& fileName, td::String& content)
{
    std::ifstream f;
    if (!openExistingBinaryFile(f, fileName))
        return false;
    size_t fileSize = (size_t) fo::getFileSize(f);
    if (fileSize == 0)
        return true;
    content.reserve(fileSize+1);
    const char *pStr = content.c_str();
    char* pData = const_cast<char*>(pStr);
    if ((size_t)(f.read(pData, fileSize).gcount()) == fileSize)
    {
        pData[fileSize] = '\0';
        return true;
    }
        
    return false;
}

template <class TSTRBUILDER>
inline bool loadFile(const td::String& fileName, TSTRBUILDER& builder)
{
    std::ifstream f;
    if (!openExistingBinaryFile(f, fileName))
        return false;
    //cnt::StringBuilder<td::String> builder;

    char buf[1024 * 4];
    while (f.read(buf, sizeof(buf)).gcount() > 0)
        builder.appendString(buf, (int)f.gcount());
    //content.append(buf, f.gcount());

    //builder.getString(content);
    return true;
}


inline bool loadFileContent(const td::String& fileName, td::String& content)
{
    std::ifstream f;
    if (!openExistingBinaryFile(f, fileName))
        return false;
    cnt::StringBuilder<td::String> builder;

    char buf[1024*4];
    while (f.read(buf, sizeof(buf)).gcount() > 0)
        builder.appendString(buf, (int)f.gcount());
    //content.append(buf, f.gcount());

    builder.getString(content);
    return true;
}

inline td::String loadFileContent(const td::String& fileName)
{
    td::String content;
    loadFileContent(fileName, content);
    return content;
}

inline bool loadFileContent(const td::String& fileName, td::MutableString& content)
{
    std::ifstream f;
    if (!openExistingBinaryFile(f, fileName))
        return false;

    char buf[1024*4];
    while (f.read(buf, sizeof(buf)).gcount() > 0)
        content.append(buf, (int)f.gcount());
    return true;
}


inline bool loadBinaryFile(const td::String& fileName, td::String& content)
{
    std::ifstream f;
    if (!openExistingBinaryFile(f, fileName))
        return false;
    cnt::StringBuilder<td::String> builder;

    char buf[1024*4];
    while (f.read(buf, sizeof(buf)).gcount() > 0)
        builder.appendString(buf, (int)f.gcount());
    //content.append(buf, f.gcount());

    builder.getString(content);
    return true;
}



inline bool loadBinaryFile(const char* fileName, td::String& content)
{
    td::String strFileName(fileName);
    return loadBinaryFile(strFileName, content);
}

inline bool loadBinaryFile(const std::string& filename, std::vector<char>& content)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) 
    {
        return false;
    }

    size_t fileSize = (size_t) file.tellg();
    content.reserve(fileSize);

    file.seekg(0);
    file.read(content.data(), fileSize);

    file.close();

    return true;
}

template <class ISTREAM>
inline void loadFromStream(ISTREAM& f, td::String& content)
{
    cnt::StringBuilder<td::String> builder;
    char buf[1024*4];
    while (f.read(buf, sizeof(buf)).gcount() > 0)
        builder.appendString(buf, (int) f.gcount());

    builder.getString(content);
}



inline int createTxtFileUsingOS(const td::String& fileName)
{
#ifdef MU_WINDOWS
    td::StringUTF16 strLongFolderName;
    buildLongWinFileName(fileName, strLongFolderName);
    int fd = 0;
    int err = _wsopen_s(&fd, strLongFolderName.c_str(), O_TRUNC | _O_CREAT | _O_TEXT | _O_SEQUENTIAL | _O_WRONLY, _SH_DENYWR, _S_IREAD | _S_IWRITE);
    err;
#else
            int fd = open(fileName.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IRGRP | S_IROTH);
#endif
    return fd;
}

inline int syncOSFile(int fd)
{
    if (fd < 0)
        return 0;

#ifdef MU_WINDOWS
    return _commit(fd);
#else
            return fsync(fd);
#endif
}

inline ssize_t writeToOSFile(int fd, const char* buffer, size_t len)
{
    if (fd < 0)
        return 0;

#ifdef MU_WINDOWS
    return _write(fd, buffer, (unsigned int) len);;
#else
    return ::write(fd, buffer, len);
#endif
    
}

template <size_t size>
inline ssize_t writeToOSFile(int fd, const char(&ctStrIn)[size])
{
    if (fd < 0)
        return 0;

    if (size == 0)
        return 0;

#ifdef MU_WINDOWS
    return _write(fd, ctStrIn, size - 1);;
#else
    return ::write(fd, ctStrIn, size - 1);
#endif
}

template <typename TSTR>
inline ssize_t writeToOSFile(int fd, const TSTR& str)
{
    if (fd < 0)
        return 0;

    size_t nLen = str.length();
    if (nLen == 0)
        return 0;

#ifdef MU_WINDOWS
    return _write(fd, str.c_str(), (unsigned int) nLen);
#else
    return ::write(fd, str.c_str(), nLen);
#endif
}

inline int closeOSFile(int fd)
{
    if (fd < 0)
        return 0;
#ifdef MU_WINDOWS
    return _close(fd);
#else
    return ::close(fd);
#endif
    
}

inline void resolveRelativePath(td::String& strPotentialRelativePath)
{
    if (strPotentialRelativePath.length() < 2)
        return;
    char ch0 = strPotentialRelativePath.at(0);
    if (ch0 == '.')
    {
        char ch1 = strPotentialRelativePath.at(1);
        if (ch1 == '/' || ch1 == '\\')
        {
            fs::path resolvedPath = fs::current_path() / (strPotentialRelativePath.c_str() + 2);
            strPotentialRelativePath = resolvedPath.c_str();
        }
        else if (ch1 == '.')
        {
            if (strPotentialRelativePath.length() < 3)
                return;
            char ch2 = strPotentialRelativePath.at(2);
            if (ch2 == '/' || ch2 == '\\')
            {
                fs::path resolvedPath = fs::current_path()  / strPotentialRelativePath.c_str();
                resolvedPath = resolvedPath.lexically_normal();
                strPotentialRelativePath = resolvedPath.c_str();
            }
        }
    }
}

inline void resolveRelativePath(const fs::path& rootPath, td::String& strPotentialRelativePath)
{
    if (strPotentialRelativePath.length() < 2)
        return;
    char ch0 = strPotentialRelativePath.at(0);
    if (ch0 == '.')
    {
        char ch1 = strPotentialRelativePath.at(1);
        if (ch1 == '/' || ch1 == '\\')
        {
            fs::path resolvedPath = rootPath / (strPotentialRelativePath.c_str() + 2);
            strPotentialRelativePath = resolvedPath.c_str();
        }
        else if (ch1 == '.')
        {
            if (strPotentialRelativePath.length() < 3)
                return;
            char ch2 = strPotentialRelativePath.at(2);
            if (ch2 == '/' || ch2 == '\\')
            {
                fs::path resolvedPath = rootPath / strPotentialRelativePath.c_str();
                resolvedPath = resolvedPath.lexically_normal();
                strPotentialRelativePath = resolvedPath.c_str();
            }
        }
    }
}

template <class TSTR>
bool copyFile(const TSTR& srcFile, const TSTR& destFileOrFolder, fo::CopyOption co)
{
    fo::fs::path p1(srcFile.c_str());
    fo::fs::path p2(destFileOrFolder.c_str());
    switch (co)
    {
        case CopyOption::None: return fo::fs::copy_file(p1, p2, fs::copy_options::none);
        case CopyOption::CopySimLinks: return fo::fs::copy_file(p1, p2, fs::copy_options::copy_symlinks);
        case CopyOption::CreateSimLinks: return fo::fs::copy_file(p1, p2, fs::copy_options::create_symlinks);
        case CopyOption::OverwriteExisting: return fo::fs::copy_file(p1, p2, fs::copy_options::overwrite_existing);
        case CopyOption::SkipExisting: return fo::fs::copy_file(p1, p2, fs::copy_options::skip_existing);
    }
    assert(false);
    return false;
    
}

//copy content of inFolder (including subdirs) to outFolder
inline bool copyFolder(const td::String& inFolder, const td::String& outFolder, fo::fs::copy_options copyOption = fo::fs::copy_options::overwrite_existing)
{
    fo::fs::path input = inFolder.c_str();
    fo::fs::path output = outFolder.c_str();

    if (!fo::fs::exists(input))
        return false;

    if (!fo::fs::exists(output))
        if (!fo::fs::create_directories(output))
            return false;
 

    for (const auto& dir : fo::fs::recursive_directory_iterator(input))
    {
        fo::fs::path destPath = output / dir.path().lexically_relative(input);

        if (fo::fs::is_regular_file(dir.path()))
        {
            if (!fo::fs::copy_file(dir.path(), destPath, copyOption))
                return false;
        }
        else if (fo:: fs::is_directory(dir.path()))
        {
            if (!fo::fs::exists(destPath))
                if (!fo::fs::create_directories(destPath))
                    return false;
        }

    }
    return true;
}

inline bool copyFolder(const fo::fs::path& inFolder, const fo::fs::path& outFolder, fo::fs::copy_options copyOption = fo::fs::copy_options::overwrite_existing)
{
    if (!fo::fs::exists(inFolder))
        return false;

    if (!fo::fs::exists(outFolder))
        if (!fo::fs::create_directories(outFolder))
            return false;
 

    for (const auto& dir : fo::fs::recursive_directory_iterator(inFolder))
    {
        fo::fs::path destPath = outFolder / dir.path().lexically_relative(inFolder);

        if (fo::fs::is_regular_file(dir.path()))
        {
            if (!fo::fs::copy_file(dir.path(), destPath, copyOption))
                return false;
        }
        else if (fo:: fs::is_directory(dir.path()))
        {
            if (!fo::fs::exists(destPath))
                if (!fo::fs::create_directories(destPath))
                    return false;
        }

    }
    return true;
}

template <class TSTRING>
td::String getFileNameWithoutExtension(const TSTRING& fullFileName)
{
    fo::fs::path fullPath(fullFileName.c_str());

    // Get the filename without extension
    td::String filenameWithoutExtension = fullPath.stem().string();
    return filenameWithoutExtension;
}

template <bool CheckIfOrigFilesExists, bool CheckIfNewFileExists = CheckIfOrigFilesExists>
inline fs::path replaceFileExtension(const fs::path& inputPath, const char* newExtension)
{
    fs::path newPath;
    if constexpr (CheckIfOrigFilesExists)
    {
        if (!fs::exists(inputPath))
            return newPath;
    }

    newPath = inputPath;
    newPath.replace_extension(newExtension);

    if constexpr(CheckIfNewFileExists)
    {
        if (fs::exists(newPath))
            return newPath;
        return fs::path();
    }
    return newPath;
}

template <bool CheckIfOrigFilesExists, bool CheckIfNewFileExists = CheckIfOrigFilesExists>
inline td::String replaceFileExtension(const td::String& inputFileName, const char* newExtension)
{
    fs::path filePath(inputFileName.c_str());

    if constexpr (CheckIfOrigFilesExists)
    {
        //check if input file name exists
        if (!fs::exists(filePath))
            return td::String();
    }

    filePath.replace_extension(newExtension);

    if constexpr(CheckIfNewFileExists)
    {
        if (fs::exists(filePath))
            return filePath.string();
        return td::String();
    }
    return filePath.string();
}

template <bool CheckIfOrigFilesExists, bool CheckIfNewFileExists = CheckIfOrigFilesExists>
inline fs::path replaceFileExtension(const fs::path& inputPath, const td::String& newExtension)
{
    return replaceFileExtension<CheckIfOrigFilesExists, CheckIfNewFileExists>(inputPath, newExtension.c_str());
}

template <bool CheckIfOrigFilesExists, bool CheckIfNewFileExists = CheckIfOrigFilesExists>
inline td::String replaceFileExtension(const td::String& inputFileName, const td::String& newExtension)
{
    return replaceFileExtension<CheckIfOrigFilesExists, CheckIfNewFileExists>(inputFileName, newExtension.c_str());
}


inline bool makeCopyWithExtensionChange(const fs::path& filePath, const char* newExtension, const fs::path newFolder = {})
{
    try
    {
        if (!fs::exists(filePath) || !fs::is_regular_file(filePath))
        {
            return false; // File doesn't exist or is not a regular file
        }

        // Create new file name with new extension (replace old one)
        fs::path newFileName = filePath.stem();
        newFileName += newExtension;

        // Determine the destination folder
        fs::path destinationFolder = newFolder.empty() ? filePath.parent_path() : newFolder;

        // Ensure destination folder exists
        if (!fs::exists(destinationFolder))
        {
            fs::create_directories(destinationFolder);
        }

        // Full destination path
        fs::path destinationPath = destinationFolder / newFileName;

        // Copy the file
        fs::copy_file(filePath, destinationPath, fs::copy_options::overwrite_existing);

        return true;
    }
    
    catch (const std::exception&)
    {
        return false;
    }
}


template <typename TSTRCONT>
inline bool matchesPattern(const fs::path& filePath, const TSTRCONT& patterns)
{
    std::string filename = filePath.filename().string();

    for (const td::String& pattern : patterns)
    {
        if (pattern.length() < 2 || pattern.at(0) != '*')
            continue;

        std::string ext = pattern.c_str() + 1; // remove '*'
        if (filename.size() >= ext.size() &&
            filename.compare(filename.size() - ext.size(), ext.size(), ext) == 0)
        {
            return true;
        }
    }
    return false;
}

template <typename TCONT>
inline bool collectFileNames(const fs::path& location, const td::String& strPattern, TCONT& fileNames, bool goToSubFolders)
{
    cnt::PushBackVector<td::String> patterns;
    auto nSeparators = strPattern.getCount(';');
    if (nSeparators == 0)
        patterns.push_back(strPattern);
    else
    {
        patterns.reserve(nSeparators+1);
        strPattern.split(';', patterns);
    }
    
    try
    {
        if (!fs::exists(location) || !fs::is_directory(location))
            return false;

        if (goToSubFolders)
        {
            for (const auto& entry : fs::recursive_directory_iterator(location))
            {
                if (!entry.is_regular_file())
                    continue;

                const fs::path& filePath = entry.path();
                std::string filename = filePath.filename().string();

                if (matchesPattern(filePath, patterns))
                    fileNames.push_back(filePath);
            }
        }
        else
        {
            for (const auto& entry : fs::directory_iterator(location))
            {
                if (!entry.is_regular_file())
                    continue;

                const fs::path& filePath = entry.path();
                std::string filename = filePath.filename().string();

                if (matchesPattern(filePath, patterns))
                    fileNames.push_back(filePath);
            }
        }

        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

inline void adjustFileName(const char* fileName)
{
#ifdef MU_WINDOWS
    if (!fileName)
        return;
    char* fn = const_cast<char*>(fileName);
    while (char ch = *fn)
    {
        if (ch == 0)
            return;
        if (ch == '/')
            *fn = '\\';
        ++fn;
    }
#endif
}

} //namespace fo
