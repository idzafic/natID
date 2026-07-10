// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file FileOperations.h
    @brief Cross-platform file and folder utility functions for the fo (file operations) namespace. */
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
#include <filesystem>
#include <vector>
#include <string>

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

/// @brief Controls how an existing destination file is handled during a copy operation.
enum class CopyOption : td::BYTE {
    None = 0,          ///< No special handling; fail if the destination already exists
    SkipExisting,      ///< Leave the destination unchanged if it already exists
    OverwriteExisting, ///< Replace the destination file if it already exists
    CopySimLinks,      ///< Copy symbolic links as symbolic links
    CreateSimLinks     ///< Create a symbolic link at the destination instead of copying data
};

//for using with getLine (fo::getLine(f, buffer))

typedef td::BufferString<td::UTF8, 256, 256>  LineSmall;   ///< Small line buffer (256-byte capacity) for reading text lines
typedef td::BufferString<td::UTF8, 1024 * 4, 1024 * 4>  LineNormal; ///< Normal line buffer (4 KiB capacity) for reading text lines
typedef td::BufferString<td::UTF8, 1024 * 64, 1024 * 64>  LineLarge; ///< Large line buffer (64 KiB capacity) for reading text lines

typedef std::ifstream InFile;  ///< Alias for a standard input file stream
typedef std::ofstream OutFile; ///< Alias for a standard output file stream


/// @brief Indicates the result of a file-existence check.
typedef enum {
    FE_FILEISNOTVALID=0, ///< The path is invalid or inaccessible
    FE_FILEDOESNOTEXIST, ///< The path does not exist on the filesystem
    FE_FILEEXIST         ///< The path exists and refers to a regular file
} FileExist;

/// @brief Classifies a filesystem entry by its kind.
typedef enum _FT{
    FT_File,   ///< Regular file
    FT_Folder, ///< Directory
    FT_Other   ///< Any other filesystem entry (device, symlink, etc.)
} FileType;

/// @brief Enumerates well-known application folder categories.
typedef enum {
    FT_GLOBAL_PATH = 0,    ///< Absolute global path (no category)
    FT_APPFOLDER,          ///< Application installation folder
    FT_HOMEFOLDER,         ///< Current user's home folder
    FT_HOMELOCALDATAFOLDER,///< User's local application data folder
    FT_TMPFOLDER,          ///< System temporary folder
    FT_DP_FOLDER,          ///< Data processing folder
    FT_REP_FOLDER,         ///< Reports folder
    FT_TR_FOLDER,          ///< Translations folder
    FT_RES_FOLDER,         ///< Resources folder
    FT_GUI_FOLDER,         ///< GUI assets folder
    FT_HELP_FOLDER         ///< Help content folder
} FolderType;

/// @brief Describes a single filesystem entry with its path and type classification.
typedef struct _FileDesc
{
    fo::fs::path name; ///< Filesystem path of the entry
    FileType type;     ///< Classification of the entry (file, folder, or other)
}FileDesc;

/// @brief Populates a singly-linked list with all entries found directly inside a directory.
/// @param folderPath Path of the directory to enumerate.
/// @param outList List that receives a FileDesc entry for each item found.
/// @return Number of entries that could not be accessed due to errors.
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

/// @brief Constructs a canonical absolute path for a file located in the current working directory.
/// @param fileName Null-terminated relative file name.
/// @param strCurrPath String that receives the resulting absolute path.
inline void getFileNameInCurrentFolder(const char* fileName, td::String& strCurrPath)
{
    fs::path currPath(fs::current_path() / fileName);
    currPath.make_preferred();
    strCurrPath = currPath.string();
}


/// @brief Retrieves the current working directory as a platform-preferred path string.
/// @param strCurrPath String that receives the current working directory path.
inline void getCurrentPath(td::String& strCurrPath)
{
    fs::path currPath(fs::current_path());
    currPath.make_preferred();
    strCurrPath = currPath.string();
}

#ifdef MU_WINDOWS
/// @brief Prepends the Windows long-path prefix (\\?\) when the path exceeds 240 characters.
/// @param inFilePath Input file path as a UTF-8 string.
/// @param outLongFilePath Output string that receives the (possibly prefixed) UTF-16 path.
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

/// @brief Prepends the Windows long-path prefix (\\?\) when the UTF-16 path exceeds 240 code units.
/// @param inFilePath Input file path as a UTF-16 string.
/// @param outLongFilePath Output string that receives the (possibly prefixed) UTF-16 path.
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


/// @brief Creates and opens a new binary file, truncating any existing file at that path.
/// @tparam T File stream type (e.g. std::ofstream).
/// @param f File stream object to open.
/// @param fileName Filesystem path of the file to create.
/// @return true if the file was created and opened successfully.
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

/// @brief Creates and opens a new text file, truncating any existing file at that path.
/// @tparam T File stream type (e.g. std::ofstream).
/// @param f File stream object to open.
/// @param fileName Path of the file to create, as a td::String.
/// @param writeUTFBOM If true, writes a UTF-8 BOM (0xEF 0xBB 0xBF) at the start of the file.
/// @return true if the file was created and opened successfully.
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

/// @brief Creates and opens a new binary file from a td::String path, truncating any existing file.
/// @tparam T File stream type (e.g. std::ofstream).
/// @param f File stream object to open.
/// @param fileName Path of the file to create, as a td::String.
/// @return true if the file was created and opened successfully.
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

/// @brief Creates and opens a new binary file from a C-string path, truncating any existing file.
/// @tparam T File stream type (e.g. std::ofstream).
/// @param f File stream object to open.
/// @param fName Null-terminated path of the file to create.
/// @return true if the file was created and opened successfully.
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

/// @brief Opens a file for writing and obtains its OS-level file descriptor for shared-read access.
/// @tparam T File stream type.
/// @param f File stream object to open.
/// @param fileName Path of the file to open.
/// @param fid Integer that receives the OS-level file descriptor on success.
/// @return true if the file was opened successfully.
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


/// @brief Opens an existing binary file for reading.
/// @tparam T File stream type (e.g. std::ifstream).
/// @param f File stream object to open.
/// @param fileName Path of the existing binary file, as a td::String.
/// @return true if the file was opened successfully.
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

/// @brief Opens an existing binary file for reading from a C-string path.
/// @tparam T File stream type (e.g. std::ifstream).
/// @param f File stream object to open.
/// @param fileName Null-terminated path of the existing binary file.
/// @return true if the file was opened successfully.
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

/// @brief Opens a file using the default open mode from a filesystem path.
/// @tparam T File stream type.
/// @param f File stream object to open.
/// @param fileName Filesystem path of the file.
/// @return true if the file was opened successfully.
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

/// @brief Opens a file with an explicit open mode from a filesystem path.
/// @tparam T File stream type.
/// @param f File stream object to open.
/// @param fileName Filesystem path of the file.
/// @param mode Combination of std::ios_base::openmode flags.
/// @return true if the file was opened successfully.
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


/// @brief Opens a file using the default open mode from a UTF-8 C-string path.
/// @tparam T File stream type.
/// @param f File stream object to open.
/// @param fileName Null-terminated UTF-8 path of the file.
/// @return true if the file was opened successfully.
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

/// @brief Opens a file with an explicit open mode from a UTF-8 C-string path.
/// @tparam T File stream type.
/// @param f File stream object to open.
/// @param fileName Null-terminated UTF-8 path of the file.
/// @param mode Combination of std::ios_base::openmode flags.
/// @return true if the file was opened successfully.
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

/// @brief Opens a file using the default open mode from a td::String path.
/// @tparam T File stream type.
/// @param f File stream object to open.
/// @param fileName Path of the file as a td::String.
/// @return true if the file was opened successfully.
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

/// @brief Opens a file with an explicit open mode from a td::String path.
/// @tparam T File stream type.
/// @param f File stream object to open.
/// @param fileName Path of the file as a td::String.
/// @param mode Combination of std::ios_base::openmode flags.
/// @return true if the file was opened successfully.
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

/// @brief Opens a file using the default open mode from a UTF-16 C-string path.
/// @tparam T File stream type.
/// @param f File stream object to open.
/// @param fileName Null-terminated UTF-16 path of the file.
/// @return true if the file was opened successfully.
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

/// @brief Opens a file with an explicit open mode from a UTF-16 C-string path.
/// @tparam T File stream type.
/// @param f File stream object to open.
/// @param fileName Null-terminated UTF-16 path of the file.
/// @param mode Combination of std::ios_base::openmode flags.
/// @return true if the file was opened successfully.
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

/// @brief Opens a file using the default open mode from a UTF-32 C-string path.
/// @tparam T File stream type.
/// @param f File stream object to open.
/// @param fileName Null-terminated UTF-32 path of the file.
/// @return true if the file was opened successfully.
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

/// @brief Opens a file with an explicit open mode from a UTF-32 C-string path.
/// @tparam T File stream type.
/// @param f File stream object to open.
/// @param fileName Null-terminated UTF-32 path of the file.
/// @param mode Combination of std::ios_base::openmode flags.
/// @return true if the file was opened successfully.
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


/// @brief Writes a null-terminated UTF-8 string to the given output stream.
/// @tparam T Output stream type.
/// @param f The output stream to write to.
/// @param txt Pointer to the null-terminated UTF-8 string; does nothing if null.
template <typename T>
void writeString(T& f, const td::UTF8* txt)
{
    if (txt)
        f << txt;
        //f.write(txt, strlen(txt));
}

/// @brief Converts a UTF-16 string to UTF-8 and writes it to the output stream.
/// @tparam T Output stream type.
/// @param f The output stream to write to.
/// @param txt Pointer to the null-terminated UTF-16 string; does nothing if null.
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

/// @brief Converts a UTF-32 string to UTF-8 and writes it to the output stream.
/// @tparam T Output stream type.
/// @param f The output stream to write to.
/// @param txt Pointer to the null-terminated UTF-32 string; does nothing if null.
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

/// @brief Writes a td::String to the output stream, optionally excluding the null terminator byte.
/// @tparam T Output stream type.
/// @param f The output stream to write to.
/// @param txt The string to write.
/// @param isBinary If true, all bytes including any trailing null are written; if false, the last byte is skipped.
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

/// @brief Converts a UTF-16 string to UTF-8 and writes it to the output stream.
/// @tparam T Output stream type.
/// @param f The output stream to write to.
/// @param txt The UTF-16 string to convert and write.
template <typename T>
void writeString(T& f, const td::StringUTF16& txt)
{
    td::String str(txt);
    int nLen = str.length();
    if (nLen > 0)
        f.write(str.c_str(), nLen);
}

/// @brief Converts a UTF-32 string to UTF-8 and writes it to the output stream.
/// @tparam T Output stream type.
/// @param f The output stream to write to.
/// @param txt The UTF-32 string to convert and write.
template <typename T>
void writeString(T& f, const td::StringUTF32& txt)
{
        td::String str(txt);
    int nLen = str.length();
    if (nLen > 0)
        f.write(str.c_str(), nLen);
}

/// @brief Writes a MidStringUTF8 substring to the output stream.
/// @tparam T Output stream type.
/// @param f The output stream to write to.
/// @param txt The UTF-8 substring to write.
template <typename T>
void writeString(T& f, const td::MidStringUTF8& txt)
{
    int nLen = txt.length();
    if (nLen > 0)
        f.write(txt.c_str(), nLen);
}

/// @brief Converts a MidStringUTF16 substring to UTF-8 and writes it to the output stream.
/// @tparam T Output stream type.
/// @param f The output stream to write to.
/// @param txt The UTF-16 substring to convert and write.
template <typename T>
void writeString(T& f, const td::MidStringUTF16& txt)
{
        td::MidStringUTF8 str(txt);
    int nLen = str.length();
    if (nLen > 0)
        f.write(str.c_str(), nLen);
}

/// @brief Converts a MidStringUTF32 substring to UTF-8 and writes it to the output stream.
/// @tparam T Output stream type.
/// @param f The output stream to write to.
/// @param txt The UTF-32 substring to convert and write.
template <typename T>
void writeString(T& f, const td::MidStringUTF32& txt)
{
        td::MidStringUTF8 str(txt);
    int nLen = str.length();
    if (nLen > 0)
        f.write(str.c_str(), nLen);
}

/// @brief Serialises the contents of a string builder to the output stream.
/// @tparam TFILE Output stream type.
/// @tparam TBUILDER String builder type that exposes a serialize(stream) method.
/// @param f The output stream to write to.
/// @param builder The string builder whose content is serialised.
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


/// @brief Serialises the contents of a binary string builder to the output stream.
/// @tparam TFILE Output stream type.
/// @tparam TBUILDER Binary builder type that exposes a serialize(stream) method.
/// @param f The output stream to write to.
/// @param builder The binary builder whose content is serialised.
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

/// @brief Writes each segment of a string builder to the output stream, separated by chSep.
/// @tparam TFILE Output stream type.
/// @tparam TBUILDER String builder type whose iterator yields writable string segments.
/// @param f The output stream to write to.
/// @param builder The string builder to iterate over.
/// @param chSep Null-terminated separator inserted between segments; ignored before the first segment.
/// @param putEndLine If true, appends std::endl after the last segment.
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

/// @brief Returns the current read position in the stream, recovering gracefully if tellg() returns -1.
/// @tparam T Input stream type.
/// @param f The input stream to query.
/// @return Current read position in bytes, or 0 if the position cannot be determined.
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

/// @brief Returns the current write position in the stream, returning 0 if tellp() reports an error.
/// @tparam T Output stream type.
/// @param f The output stream to query.
/// @return Current write position in bytes, or 0 on error.
template <typename T>
std::streamoff tellWritePos(T& f)
{
    std::streamoff toRet = f.tellp();
    if (toRet < 0)
        return 0;
    return toRet;
}

/// @brief Seeks the read cursor of an input stream to the given absolute byte position.
/// @tparam T Input stream type.
/// @param f The input stream to seek.
/// @param pos Absolute byte position to seek to.
template <typename T>
void seekReadPos(T& f, size_t pos)
{
    f.seekg(pos);
}

/// @brief Seeks the write cursor of an output stream to the given absolute byte position.
/// @tparam T Output stream type.
/// @param f The output stream to seek.
/// @param pos Absolute byte position to seek to.
template <typename T>
void seekWritePos(T& f, size_t pos)
{
    f.seekp(pos);
}


/// @brief Seeks an input stream to the given absolute byte position.
/// @param f The input stream to seek.
/// @param pos Absolute byte position.
inline void seek(std::istream& f, size_t pos)
{
    f.seekg(pos);
}

/// @brief Seeks an output stream to the given absolute byte position.
/// @param f The output stream to seek.
/// @param pos Absolute byte position.
inline void seek(std::ostream& f, size_t pos)
{
    f.seekp(pos);
}

/// @brief Returns the current read position of an input stream, handling negative tellg() results.
/// @param f The input stream to query.
/// @return Current read position in bytes, or 0 if the position cannot be determined.
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

/// @brief Returns the current write position of an output stream, returning 0 on error.
/// @param f The output stream to query.
/// @return Current write position in bytes, or 0 on error.
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

/// @brief Reads up to buffSize bytes from the stream into buffer.
/// @tparam T Input stream type.
/// @param f The input stream to read from.
/// @param buffer Pointer to the destination buffer.
/// @param buffSize Maximum number of bytes to read.
/// @return Number of bytes actually read, 0 at end of file, or -1 on error.
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

/// @brief Writes buffSize bytes from buffer to the output stream.
/// @tparam OSTREAM Output stream type.
/// @param f The output stream to write to.
/// @param buffer Pointer to the source data.
/// @param buffSize Number of bytes to write.
/// @return buffSize if the write succeeded, or -1 on error.
template <class OSTREAM>
std::streamsize write(OSTREAM& f, const char* buffer, std::streamsize buffSize)
{
    f.write(buffer, buffSize);

    if (f.good())
        return buffSize;
    return -1;
}

/// @brief Writes all buffSize bytes from buffer to the output stream.
/// @tparam T Output stream type.
/// @param f The output stream to write to.
/// @param buffer Pointer to the source data.
/// @param buffSize Number of bytes to write.
/// @return true if all bytes were written successfully, false on error.
template <typename T>
bool writeAll(T& f, const char* buffer, size_t buffSize)
{
    f.write(buffer, buffSize);

    if (f.good())
        return true;
    return false;
}

/// @brief Reads one line of text from the stream into a BufferString, handling CR, LF, and CR+LF line endings.
/// @tparam T_STREAM Input stream type.
/// @tparam T_CHAR Character type of the BufferString.
/// @tparam NINITLEN Initial capacity of the BufferString.
/// @tparam INCREMENT Growth increment of the BufferString.
/// @param f The input stream to read from.
/// @param s BufferString that receives the line content (without the line ending).
/// @return true if a line was read (possibly empty), false at end of file or on overflow.
template <class T_STREAM, class T_CHAR, int NINITLEN, int INCREMENT>
bool getLine(T_STREAM& f, td::BufferString<T_CHAR, NINITLEN, INCREMENT>& s)
{
    s.erase();

    if (!f.good())
        return false;

    typedef typename T_STREAM::int_type   int_type;
    typedef typename T_STREAM::traits_type traits_type;

    const int_type EOF_VAL  = traits_type::eof();
    const int_type LF_VAL   = traits_type::to_int_type(static_cast<T_CHAR>(TD_TO_NEWLINE));  // \n
    const int_type CR_VAL   = traits_type::to_int_type(static_cast<T_CHAR>(TD_TO_LINEBEG));  // \r

    bool hasContent = false;

    for (;;)
    {
        int_type raw = f.rdbuf()->sbumpc();  // read and advance in one step

        if (traits_type::eq_int_type(raw, EOF_VAL))
        {
            f.setstate(std::ios_base::eofbit);
            return hasContent;  // true if we accumulated something, false if truly empty
        }

        if (traits_type::eq_int_type(raw, LF_VAL))
        {
            // End of line — return whatever we have (even empty lines)
            return true;
        }

        if (traits_type::eq_int_type(raw, CR_VAL))
        {
            // Peek ahead: consume LF if it follows (handles \r\n)
            int_type next = f.rdbuf()->sgetc();
            if (!traits_type::eq_int_type(next, EOF_VAL) &&
                 traits_type::eq_int_type(next, LF_VAL))
            {
                f.rdbuf()->sbumpc();  // consume the LF
            }
            return true;
        }

        if (s.size() >= s.max_size())
        {
            f.setstate(std::ios_base::failbit);
            return false;
        }

        s += traits_type::to_char_type(raw);
        hasContent = true;
    }
}

//fromPosition is input/output argument
/// @brief Reads one line from the stream into a MutableString, tracking the byte position within the stream.
/// @tparam ISTREAM Input stream type.
/// @param f The input stream to read from.
/// @param str MutableString that receives the line content (without the line ending).
/// @param pos Input/output parameter tracking the cumulative byte position in the stream.
/// @return true if a line was read (possibly empty), false at end of file.
template <class ISTREAM>
inline bool getLine(ISTREAM& f, td::MutableString& str, td::UINT4& pos)
{
    if (!f.good())
        return false;

    if (str.capacity() == 0)
        str.reserve(4096);
    str.reset();

    typedef typename ISTREAM::int_type    int_type;
    typedef typename ISTREAM::traits_type traits_type;

    const int_type EOF_VAL = traits_type::eof();
    const int_type LF_VAL  = traits_type::to_int_type(static_cast<char>(TD_TO_NEWLINE));
    const int_type CR_VAL  = traits_type::to_int_type(static_cast<char>(TD_TO_LINEBEG));

    bool hasContent = false;

    for (;;)
    {
        int_type raw = f.rdbuf()->sbumpc();  // read and advance in one step

        if (traits_type::eq_int_type(raw, EOF_VAL))
        {
            f.setstate(std::ios_base::eofbit);
            return hasContent;
        }

        ++pos;

        if (traits_type::eq_int_type(raw, LF_VAL))
        {
            // end of line — return whatever we have (even empty lines)
            return true;
        }

        if (traits_type::eq_int_type(raw, CR_VAL))
        {
            // peek ahead: consume LF if it follows (handles \r\n)
            int_type next = f.rdbuf()->sgetc();
            if (!traits_type::eq_int_type(next, EOF_VAL) &&
                 traits_type::eq_int_type(next, LF_VAL))
            {
                f.rdbuf()->sbumpc();
                ++pos;
            }
            return true;
        }

        str.append(traits_type::to_char_type(raw));
        hasContent = true;
    }
}

/// @brief Seeks an input stream to the given absolute position, optionally clearing error flags first.
/// @tparam ISTREAM Input stream type.
/// @tparam TPOS Integral type of the position value.
/// @param f The input stream to seek.
/// @param pos Absolute byte position to seek to.
/// @param clearState If true, clears all stream error flags before seeking.
template <class ISTREAM, typename TPOS>
inline void goToPos(ISTREAM& f, TPOS pos, bool clearState)
{
    if (clearState)
        f.clear();

#ifdef MU_LINUX
    // seekg is unreliable on some Linux systems for certain stream types
    // (e.g. streams over virtual filesystems, pipes, or proc files).
    // ignore() skips bytes sequentially which always works.
    if (pos > 0)
        f.ignore(pos);
#else
    f.seekg(pos, std::ios::beg);
#endif
}

/// @brief Checks whether a file exists and is a regular file (td::String overload).
/// @param fileName Path of the file to check.
/// @return true if the path exists and is a regular file.
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

/// @brief Checks whether a file exists and is a regular file (C-string overload).
/// @param fileName Null-terminated path of the file to check.
/// @return true if the path exists and is a regular file.
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

/// @brief Checks whether a file exists and is a regular file (fs::path overload).
/// @param filePath Filesystem path of the file to check.
/// @return true if the path exists and is a regular file.
[[nodiscard]]
inline bool fileExists(const fo::fs::path& filePath)
{
    if (fo::fs::exists(filePath) && fo::fs::is_regular_file(filePath))
        return true;
    return false;
}

/// @brief Checks whether a directory exists (td::String overload).
/// @param folderName Path of the directory to check.
/// @return true if the path exists and is a directory.
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

/// @brief Checks whether a directory exists (C-string overload).
/// @param folderNameCStr Null-terminated path of the directory to check.
/// @return true if the path exists and is a directory.
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

/// @brief Checks whether a directory exists (fs::path overload).
/// @param folderPath Filesystem path of the directory to check.
/// @return true if the path exists and is a directory.
[[nodiscard]]
inline bool folderExists(const fo::fs::path& folderPath)
{
    if (fo::fs::exists(folderPath) && fo::fs::is_directory(folderPath))
        return true;
    return false;
}

/// @brief Deletes a file by its td::String path.
/// @param fileName Path of the file to delete.
/// @return true if the file was successfully deleted, false on error.
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

/// @brief Deletes a file identified by an fs::path.
/// @param fileNamePath Filesystem path of the file to delete.
/// @return true if the file was successfully deleted, false on error.
inline bool deleteFile(const fs::path& fileNamePath)
{
    td::String str;
    str.fromKnownString(fileNamePath.string().c_str(), fileNamePath.string().length());
    return deleteFile(str);
}

/// @brief Creates a single directory at the given td::String path if it does not already exist.
/// @param folderName Path of the directory to create.
/// @return true if the directory was created or already existed, false on error.
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

/// @brief Creates a single directory at the given C-string path if it does not already exist.
/// @param folderNameCStr Null-terminated path of the directory to create.
/// @return true if the directory was created or already existed, false on error.
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

/// @brief Creates a single directory at the given fs::path if it does not already exist.
/// @param path Filesystem path of the directory to create.
/// @return true if the directory was created or already existed, false on error.
inline bool createFolder(const fs::path& path)
{
    td::String str;
    str.fromKnownString(path.string().c_str(), path.string().length());
    return createFolder(str);
}

/// @brief Creates a nested directory path relative to an existing root directory.
/// @param pathRoot The existing root directory under which the new path is created.
/// @param foldersToCreate Null-terminated relative sub-path to create.
/// @return true if the directories were created successfully, false if pathRoot does not exist.
inline bool createFolders(const fo::fs::path& pathRoot, const char* foldersToCreate)
{
    if (!fo::fs::exists(pathRoot))
        return false;
    fo::fs::path toCreate(pathRoot / foldersToCreate);
    return fo::fs::create_directories(toCreate);
}

/// @brief Creates all directories in the given path, including any missing intermediate directories.
/// @param folders Filesystem path of the full directory tree to create.
/// @return true if the directories exist or were created successfully.
inline bool createFolders(const fo::fs::path& folders)
{
    if (fo::fs::exists(folders))
        return true;
    return fo::fs::create_directories(folders);
}

/// @brief Deletes a directory and optionally all of its contents recursively.
/// @param folderPath Filesystem path of the directory to delete.
/// @param deleteContent If true, deletes all files and sub-directories before removing the directory itself.
/// @return true if the directory was successfully deleted, false on error.
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

/// @brief Deletes a directory and optionally all of its contents (td::String overload).
/// @param folderName Path of the directory to delete.
/// @param deleteContent If true, deletes all files and sub-directories before removing the directory itself.
/// @return true if the directory was successfully deleted, false on error.
inline bool deleteFolder(const td::String& folderName, bool deleteContent=true)
{
    fs::path folderPath(folderName.c_str());
    return deleteFolder(folderPath, deleteContent);
}

/// @brief Deletes all files in a folder whose names start with a given prefix.
/// @param folderPath Path of the folder to clean.
/// @param fileNamesThatStartWith Null-terminated prefix; only files whose names start with this string are deleted.
MAINUTILS_API void deleteFilesInFolder(const std::filesystem::path& folderPath, const char* fileNamesThatStartWith);

/// @brief Joins a folder name and a file name into a single platform-preferred absolute path.
/// @param folderName Null-terminated folder path.
/// @param fileName Null-terminated file name.
/// @param outFileName String that receives the combined path.
inline void buildFileName(const char* folderName, const char* fileName, td::String& outFileName)
{
    fs::path path1(folderName);
    fs::path path(path1 / fileName);
    path.make_preferred();
    outFileName = path.string();
}

/// @brief Joins a td::String folder path and a td::String file name into a combined path.
/// @param folderName The folder path.
/// @param fileName The file name to append.
/// @param outFileName String that receives the combined path.
inline void buildFileName(const td::String& folderName, const td::String& fileName, td::String& outFileName)
{
    fs::path path1(folderName.c_str());
    fs::path path(path1 / fileName.c_str());
    outFileName = path.string();
}

/// @brief Generates a unique temporary file name and writes it into a caller-supplied buffer.
/// @param buff Pointer to the character buffer that receives the temporary file name.
/// @param buffLen Size of the buffer in bytes.
/// @return Pointer to buff on success, or nullptr if the name could not be generated or the buffer is too small.
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

/// @brief Generates a unique temporary file name and stores it in a td::String.
/// @param outFileName String that receives the generated temporary file name.
inline void getTmpFileName(td::String& outFileName)
{
    char tmp[1024];
    outFileName = getTmpFileName(tmp, 1024);
}


/// @brief Extracts the directory component from a full file path.
/// @param fullPathName The full file path to parse.
/// @param outDirectory String that receives the directory portion of the path.
inline void getDirectoryFromFullPath(const td::String& fullPathName, td::String& outDirectory)
{
    fs::path filePath(fullPathName.c_str());
    outDirectory =filePath.parent_path().string();
}

/// @brief Extracts the file base name (stem) from a full file path, without the extension.
/// @param fullPathName The full file path to parse.
/// @param outBaseName String that receives the file name without its directory or extension.
inline void getBaseNameFromFullPath(const td::String& fullPathName, td::String& outBaseName)
{
    fs::path filePath(fullPathName.c_str());
#ifdef USE_BOOST_FILESYSTEM
    outBaseName = fs::basename(filePath);
#else
    outBaseName = filePath.stem().string();
#endif
}

/// @brief Extracts the file name (including extension) from a full file path.
/// @param fullPathName The full file path to parse.
/// @param outFileName String that receives the file name including its extension.
inline void getFileNameFromFullPath(const td::String& fullPathName, td::String& outFileName)
{
    fs::path filePath(fullPathName.c_str());
    outFileName = filePath.filename().string();
}


/// @brief Reads the entire contents of a binary file into a BufferString in chunks.
/// @tparam TBYTES Character type of the buffer.
/// @tparam INITSIZE Initial capacity of the BufferString.
/// @tparam INCSIZE Growth increment of the BufferString.
/// @param fileName Path of the binary file to read.
/// @param buffer BufferString that receives the file content.
/// @return true if the file was read successfully, false if it could not be opened.
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

/// @brief Returns the size of a file given its C-string path.
/// @param fileName Null-terminated path of the file.
/// @return File size in bytes, or 0 if the file does not exist.
inline td::LUINT8 getFileSize(const char* fileName)
{
    fs::path filePath(fileName);
    if (fo::fileExists(filePath))
        return (td::LUINT8) fs::file_size(filePath);
    return 0;
}

/// @brief Returns the size of the file associated with an input stream.
/// @param f The input stream to measure; the seek cursor is reset to the beginning on return.
/// @return Total size of the file in bytes.
inline td::LUINT8 getFileSize(std::istream& f)
{
    f.seekg(0, std::ios_base::end);//seek to end
    //now get current position as length of file
    td::LUINT8 toRet = f.tellg();
    //move cursor to the first position
    f.seekg(0, std::ios_base::beg);//seek to end
    return toRet;
}

/// @brief Returns the size of the file associated with an output stream.
/// @param f The output stream to measure; the seek cursor is reset to the beginning on return.
/// @return Total size of the file in bytes.
inline td::LUINT8 getFileSize(std::ostream& f)
{
    f.seekp(0, std::ios_base::end);//seek to end
    td::LUINT8 toRet = f.tellp();
    //now get current position as length of file
    f.seekp(0, std::ios_base::beg);//seek to end
    return toRet;
}

/// @brief Collects all file paths in a folder whose extension matches the given string.
/// @tparam TCONTAINER Container type that supports push_back with fs::path elements.
/// @param outFileNames Container that receives matching file paths.
/// @param folderPath Path of the folder to search.
/// @param pExtension Optional file extension filter (e.g. ".txt"); pass null to collect all files.
template <class TCONTAINER>
void getFileNamesInFolder(TCONTAINER& outFileNames, const fo::fs::path& folderPath, const char* pExtension = 0)
{
    if (!folderExists(folderPath))
        return;

//    fs::path dir_path(folderName.c_str());
    fs::directory_iterator end_it;

    if (pExtension)
    {
        if (*pExtension == '*')
            ++pExtension;
    }

    // loop through each file in the directory
    for(fs::directory_iterator it(folderPath); it != end_it; ++it)
    {
        // if it's not a directory and its extension is as given [2]
        if(fs::is_directory(it->status()))
            continue;

        if (pExtension != 0)
        {
            auto strExtension = it->path().extension();
            if (strExtension == pExtension)
            {
                // store filename for later use
                //std::string strTmp = it->path().string();
                td::String str(it->path().c_str());
                outFileNames.push_back(str);
            }
        }
        else
        {
            outFileNames.push_back( it->path().c_str() );
        }
    }
}

/// @brief Collects all file paths in a folder (td::String overload) whose extension matches the given string.
/// @tparam TCONTAINER Container type that supports push_back with td::String elements.
/// @param outFileNames Container that receives matching file paths.
/// @param folderName Path of the folder to search, as a td::String.
/// @param pExtension Optional file extension filter; pass null to collect all files.
template <class TCONTAINER>
void getFileNamesInFolder(TCONTAINER& outFileNames, const td::String& folderName, const char* pExtension = 0)
{
    fs::path folderPath(folderName.c_str());
    getFileNamesInFolder(outFileNames, folderPath, pExtension);
}

/// @brief Converts a DOS wildcard pattern (e.g. "*.txt") to an equivalent regular expression string.
/// @param dosWildcard The DOS wildcard pattern to convert.
/// @return A td::String containing the equivalent regex pattern.
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

/// @brief Collects all file paths whose 4-character extension matches strExtension from a folder.
/// @tparam TCONTAINER Container type that supports push_back with fs::path elements.
/// @param vOut Container that receives matching file paths.
/// @param folderPath Path of the folder to search.
/// @param strExtension Null-terminated 4-character extension to match (e.g. ".txt").
/// @param resetContent If true, resets vOut before adding results.
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

/// @brief Collects file names in a folder that match a DOS wildcard pattern.
/// @tparam TCONTAINER Container type that supports push_back with td::String elements.
/// @param outFileNames Container that receives matching file names (basename only, not full path).
/// @param folderName Path of the folder to search.
/// @param pWC Null-terminated DOS wildcard pattern (e.g. "*.log").
/// @return true if the search completed without error, false if an exception occurred.
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

/// @brief Reads an entire binary file into a td::String in a single operation.
/// @tparam TSTR String type for the file name (td::String or compatible).
/// @param fileName Path of the binary file to load.
/// @param content String that receives the complete file content.
/// @return true if the file was read successfully, false if it could not be opened.
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

/// @brief Reads an entire binary file into a string builder by appending 4 KiB chunks.
/// @tparam TSTRBUILDER String builder type that exposes appendString(buf, len).
/// @param fileName Path of the binary file to load.
/// @param builder String builder that receives the file content.
/// @return true if the file was read successfully, false if it could not be opened.
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


/// @brief Reads the entire content of a binary file into a td::String.
/// @param fileName Path of the binary file to load.
/// @param content String that receives the complete file content.
/// @return true if the file was read successfully, false if it could not be opened.
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

/// @brief Reads the entire content of a binary file and returns it as a td::String.
/// @param fileName Path of the binary file to load.
/// @return td::String containing the file content, or an empty string on failure.
inline td::String loadFileContent(const td::String& fileName)
{
    td::String content;
    loadFileContent(fileName, content);
    return content;
}

/// @brief Reads the entire content of a binary file into a td::MutableString.
/// @param fileName Path of the binary file to load.
/// @param content MutableString that receives the complete file content.
/// @return true if the file was read successfully, false if it could not be opened.
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


/// @brief Reads the entire content of a binary file into a td::String (alias for loadFileContent).
/// @param fileName Path of the binary file to load.
/// @param content String that receives the complete file content.
/// @return true if the file was read successfully, false if it could not be opened.
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

/// @brief Reads the entire content of a binary file identified by an fs::path into a td::String.
/// @param filePath Filesystem path of the binary file to load.
/// @param content String that receives the complete file content.
/// @return true if the file was read successfully, false if it could not be opened.
inline bool loadBinaryFile(const fo::fs::path& filePath, td::String& content)
{
    std::ifstream f;
    if (!openExistingBinaryFile(f, filePath))
        return false;
    cnt::StringBuilder<td::String> builder;

    char buf[1024 * 4];
    while (f.read(buf, sizeof(buf)).gcount() > 0)
        builder.appendString(buf, (int)f.gcount());

    builder.getString(content);
    return true;
}



/// @brief Reads the entire content of a binary file identified by a C-string path into a td::String.
/// @param fileName Null-terminated path of the binary file to load.
/// @param content String that receives the complete file content.
/// @return true if the file was read successfully, false if it could not be opened.
inline bool loadBinaryFile(const char* fileName, td::String& content)
{
    td::String strFileName(fileName);
    return loadBinaryFile(strFileName, content);
}

/// @brief Reads the entire content of a binary file (std::string path) into a std::vector<char>.
/// @param filename Path of the binary file to load.
/// @param content Vector that receives the complete file content.
/// @return true if the file was read successfully, false if it could not be opened.
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

/// @brief Reads all data from an input stream into a td::String using an internal string builder.
/// @tparam ISTREAM Input stream type.
/// @param f The input stream to read from.
/// @param content String that receives the complete stream content.
template <class ISTREAM>
inline void loadFromStream(ISTREAM& f, td::String& content)
{
    cnt::StringBuilder<td::String> builder;
    char buf[1024*4];
    while (f.read(buf, sizeof(buf)).gcount() > 0)
        builder.appendString(buf, (int) f.gcount());

    builder.getString(content);
}



/// @brief Creates a text file using the OS-level file API, allowing shared read access during writing.
/// @param fileName Path of the file to create.
/// @return OS-level file descriptor on success, or a negative value on error.
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

/// @brief Flushes all pending writes for an OS-level file descriptor to disk.
/// @param fd The OS-level file descriptor to sync.
/// @return 0 on success, or a non-zero error code on failure.
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

/// @brief Writes len bytes from buffer to the OS-level file descriptor.
/// @param fd The OS-level file descriptor to write to.
/// @param buffer Pointer to the data to write.
/// @param len Number of bytes to write.
/// @return Number of bytes written, or 0 if fd is invalid.
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

/// @brief Writes a compile-time fixed-length C-string literal to the OS-level file descriptor.
/// @tparam size Size of the character array (including null terminator).
/// @param fd The OS-level file descriptor to write to.
/// @param ctStrIn Reference to the character array to write; the null terminator is not written.
/// @return Number of bytes written, or 0 if fd is invalid or size is zero.
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

/// @brief Writes the contents of a string object to the OS-level file descriptor.
/// @tparam TSTR String type that exposes length() and c_str().
/// @param fd The OS-level file descriptor to write to.
/// @param str The string whose content is written.
/// @return Number of bytes written, or 0 if fd is invalid or the string is empty.
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

/// @brief Closes an OS-level file descriptor.
/// @param fd The OS-level file descriptor to close.
/// @return 0 on success, or 0 if fd was already invalid.
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

/// @brief Resolves a relative path beginning with "./" or "../" against the current working directory.
/// @param strPotentialRelativePath Input/output path string; replaced with the resolved absolute path if relative.
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

/// @brief Resolves a relative path beginning with "./" or "../" against a given root directory.
/// @param rootPath The root directory used as the base for relative resolution.
/// @param strPotentialRelativePath Input/output path string; replaced with the resolved absolute path if relative.
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

/// @brief Copies a file from srcFile to destFileOrFolder using the specified copy option.
/// @tparam TSTR String type that exposes c_str().
/// @param srcFile Path of the source file.
/// @param destFileOrFolder Path of the destination file or folder.
/// @param co The CopyOption controlling how an existing destination is handled.
/// @return true if the copy succeeded, false on error.
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
/// @brief Recursively copies the entire content of inFolder (including subdirectories) into outFolder.
/// @param inFolder Source directory path.
/// @param outFolder Destination directory path; created if it does not exist.
/// @param copyOption Filesystem copy option applied to each file (default: overwrite_existing).
/// @return true if the copy completed successfully, false on error.
inline bool copyFolder(const td::String& inFolder, const td::String& outFolder, fo::fs::copy_options copyOption = fo::fs::copy_options::overwrite_existing)
{
    try
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
    catch(...)
    {
        return false;
    }

    return false;
}

/// @brief Recursively copies the entire content of inFolder (including subdirectories) into outFolder (fs::path overload).
/// @param inFolder Source directory path.
/// @param outFolder Destination directory path; created if it does not exist.
/// @param copyOption Filesystem copy option applied to each file (default: overwrite_existing).
/// @return true if the copy completed successfully, false on error.
inline bool copyFolder(const fo::fs::path& inFolder, const fo::fs::path& outFolder, fo::fs::copy_options copyOption = fo::fs::copy_options::overwrite_existing)
{
    try
    {
        if (!fo::fs::exists(inFolder))
            return false;

        if (!fo::fs::exists(outFolder))
            if (!fo::fs::create_directories(outFolder))
                return false;


        for (const auto& dir : fo::fs::recursive_directory_iterator(inFolder))
        {
            fo::fs::path destPath = outFolder / dir.path().lexically_relative(inFolder);

            auto dirPath = dir.path();

            if (fo::fs::is_regular_file(dirPath))
            {
                if (!fo::fs::copy_file(dirPath, destPath, copyOption))
                    return false;
            }
            else if (fo:: fs::is_directory(dirPath))
            {
                if (!fo::fs::exists(destPath))
                    if (!fo::fs::create_directories(destPath))
                        return false;
            }

        }
        return true;
    }
    catch(...)
    {
        return false;
    }

    return false;
}

/// @brief Recursively copies inFolder into outFolder, skipping any entries whose names appear in excludeNames.
/// @param inFolder Source directory path.
/// @param outFolder Destination directory path; created if it does not exist.
/// @param excludeNames List of file or directory names (not full paths) to exclude from the copy.
/// @param copyOption Filesystem copy option applied to each copied file (default: overwrite_existing).
/// @return true if the copy completed without error, false on any failure.
inline bool copyFolderExclude(
    const fo::fs::path& inFolder,
    const fo::fs::path& outFolder,
    const std::vector<td::String>& excludeNames,
    fo::fs::copy_options copyOption = fo::fs::copy_options::overwrite_existing)
{
    try
    {
        if (!fo::fs::exists(inFolder))
            return false;

        if (!fo::fs::exists(outFolder))
            if (!fo::fs::create_directories(outFolder))
                return false;

        for (const auto& entry : fo::fs::directory_iterator(inFolder))
        {
            fo::fs::path srcPath = entry.path();
            td::String name = srcPath.filename().string().c_str();

            // Skip if name is in exclude list
            if (std::find(excludeNames.begin(), excludeNames.end(), name) != excludeNames.end())
            {
                continue; // skip this file or folder completely
            }

            fo::fs::path destPath = outFolder / srcPath.filename();

            if (fo::fs::is_directory(srcPath))
            {
                // create destination folder
                if (!fo::fs::exists(destPath))
                    if (!fo::fs::create_directories(destPath))
                        return false;

                // recurse into this folder
                if (!copyFolderExclude(srcPath, destPath, excludeNames, copyOption))
                    return false;
            }
            else if (fo::fs::is_regular_file(srcPath))
            {
                if (!fo::fs::copy_file(srcPath, destPath, copyOption))
                    return false;
            }
        }

        return true;
    }
    catch (...)
    {
        return false;
    }
}

/// @brief Extracts the file name without its extension from a full file path.
/// @tparam TSTRING String type that exposes c_str().
/// @param fullFileName The full file path including extension.
/// @return A td::String containing only the file name stem (no directory, no extension).
template <class TSTRING>
td::String getFileNameWithoutExtension(const TSTRING& fullFileName)
{
    fo::fs::path fullPath(fullFileName.c_str());

    // Get the filename without extension
    td::String filenameWithoutExtension = fullPath.stem().string();
    return filenameWithoutExtension;
}

/// @brief Returns a copy of inputPath with its file extension replaced by newExtension.
/// @tparam CheckIfOrigFilesExists If true, returns an empty path when the original file does not exist.
/// @tparam CheckIfNewFileExists If true, returns an empty path when the new path does not exist.
/// @param inputPath The original filesystem path.
/// @param newExtension Null-terminated new extension (e.g. ".bak").
/// @return The new path, or an empty path if an existence check fails.
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

/// @brief Returns a copy of inputFileName (td::String) with its extension replaced by newExtension.
/// @tparam CheckIfOrigFilesExists If true, returns an empty string when the original file does not exist.
/// @tparam CheckIfNewFileExists If true, returns an empty string when the new path does not exist.
/// @param inputFileName The original file path string.
/// @param newExtension Null-terminated new extension (e.g. ".bak").
/// @return The new path as a td::String, or an empty string if an existence check fails.
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

/// @brief Returns a copy of inputPath with its extension replaced by newExtension (td::String extension overload).
/// @tparam CheckIfOrigFilesExists If true, returns an empty path when the original file does not exist.
/// @tparam CheckIfNewFileExists If true, returns an empty path when the new path does not exist.
/// @param inputPath The original filesystem path.
/// @param newExtension The new extension as a td::String.
/// @return The new path, or an empty path if an existence check fails.
template <bool CheckIfOrigFilesExists, bool CheckIfNewFileExists = CheckIfOrigFilesExists>
inline fs::path replaceFileExtension(const fs::path& inputPath, const td::String& newExtension)
{
    return replaceFileExtension<CheckIfOrigFilesExists, CheckIfNewFileExists>(inputPath, newExtension.c_str());
}

/// @brief Returns a copy of inputFileName with its extension replaced (both td::String overloads).
/// @tparam CheckIfOrigFilesExists If true, returns an empty string when the original file does not exist.
/// @tparam CheckIfNewFileExists If true, returns an empty string when the new path does not exist.
/// @param inputFileName The original file path string.
/// @param newExtension The new extension as a td::String.
/// @return The new path as a td::String, or an empty string if an existence check fails.
template <bool CheckIfOrigFilesExists, bool CheckIfNewFileExists = CheckIfOrigFilesExists>
inline td::String replaceFileExtension(const td::String& inputFileName, const td::String& newExtension)
{
    return replaceFileExtension<CheckIfOrigFilesExists, CheckIfNewFileExists>(inputFileName, newExtension.c_str());
}


/// @brief Creates a copy of filePath with the extension changed to newExtension, optionally in a different folder.
/// @param filePath Path of the source file.
/// @param newExtension Null-terminated new extension to apply (e.g. ".bak").
/// @param newFolder Destination folder; if empty, uses the same folder as filePath.
/// @return true if the copy was created successfully, false if the source is missing or a copy error occurred.
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


/// @brief Tests whether a file path matches any pattern in the provided list of extension patterns.
/// @tparam TSTRCONT Container of td::String elements holding patterns of the form "*.ext".
/// @param filePath The file path whose name is tested.
/// @param patterns Container of wildcard patterns (currently only "*.ext" suffix patterns are supported).
/// @return true if the file name matches at least one pattern in the list.
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

/// @brief Collects file paths in a directory that match a semicolon-separated list of extension patterns.
/// @tparam TCONT Container type that supports push_back with fs::path elements.
/// @param location Directory to search.
/// @param strPattern Semicolon-separated list of patterns (e.g. "*.cpp;*.h").
/// @param fileNames Container that receives all matching file paths.
/// @param goToSubFolders If true, the search recurses into sub-directories.
/// @return true if the search completed without error, false if location is invalid or an exception occurred.
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

/// @brief Replaces all forward slashes in a file name with backslashes (Windows-only no-op on other platforms).
/// @param fileName Pointer to the null-terminated file name string to adjust in place.
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


/// @brief Computes the total size in bytes of all regular files under a directory tree.
/// @param root Root directory path to traverse.
/// @return Total byte count of all regular files found, or 0 if root does not exist.
inline std::uintmax_t getFolderSize(const std::filesystem::path& root)
{
    namespace fs = std::filesystem;

    std::uintmax_t totalSize = 0;

    if (!fs::exists(root))
        return 0;

    for (auto const& entry : fs::recursive_directory_iterator(root, fs::directory_options::skip_permission_denied))
    {
        if (entry.is_regular_file())
        {
            std::error_code ec;
            auto size = entry.file_size(ec);
            if (!ec)
                totalSize += size;
        }
    }

    return totalSize;
}

/// @brief Returns the parent directory path of a full file path (C-string overload).
/// @param fullPath Null-terminated full file path.
/// @return fs::path representing the parent directory.
inline fs::path getFolderPath(const char* fullPath)
{
    fs::path p(fullPath);
    fs::path parent = p.parent_path();
    return parent;
}

/// @brief Returns the parent directory path of a full file path (td::String overload).
/// @param fullPath The full file path as a td::String.
/// @return fs::path representing the parent directory.
inline fs::path getFolderPath(const td::String& fullPath)
{
    return getFolderPath(fullPath.c_str());
}

/// @brief Returns the parent directory path of a full file path (fs::path overload).
/// @param fullPath The full file path as an fs::path.
/// @return fs::path representing the parent directory.
inline fs::path getFolderPath(const fs::path& fullPath)
{
    fs::path parent = fullPath.parent_path();
    return parent;
}

/// @brief Extracts just the file name (including extension) from a full path (C-string overload).
/// @param fullFath Null-terminated full file path.
/// @return A td::String containing only the file name component.
inline td::String getFilename(const char* fullFath)
{
    fs::path p(fullFath);
    return p.filename().c_str();
}

/// @brief Extracts just the file name (including extension) from a full path (td::String overload).
/// @param fullFath The full file path as a td::String.
/// @return A td::String containing only the file name component.
inline td::String getFilename(const td::String& fullFath)
{
    fs::path p(fullFath.c_str());
    return p.filename().c_str();
}

/// @brief Extracts just the file name (including extension) from a full path (fs::path overload).
/// @param fullFath The full file path as an fs::path.
/// @return A td::String containing only the file name component.
inline td::String getFilename(const fs::path& fullFath)
{
    return fullFath.filename().c_str();
}

/// @brief Returns the default shared-library file extension for the current platform.
/// @return ".dll" on Windows, ".dylib" on macOS, or ".so" on other platforms.
constexpr const char* getSharedLibDefaultExtension()
{
#ifdef MU_WINDOWS
    return ".dll";
#else
#ifdef MU_MACOS
    return ".dylib";
#else
    return ".so";
#endif
#endif
}

} //namespace fo
