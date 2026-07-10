// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IAppSettings.h
 *  @brief Abstract interface for application path management, folder operations, and runtime settings.
 */
#pragma once
#include <mu/mu.h>
#include <td/String.h>
#include <fo/FileOperations.h>

namespace mu
{
	class ILogger;

	/// @brief Abstract interface providing access to application directories, file utilities, logger references, and version information.
	class MAINUTILS_API IAppSettings
	{

	public:

		/// @brief Returns the numeric program identifier for this application instance.
		/// @return 32-bit unsigned program ID.
		virtual td::UINT4 getProgID() = 0;

		/// @brief Returns the string path for the specified folder type.
		/// @param folderType The logical folder type to query.
		/// @return Reference to the path string for the specified folder.
		virtual const td::String& getPath(fo::FolderType folderType) const = 0;

		/// @brief Returns the filesystem path for the specified folder type.
		/// @param folderType The logical folder type to query.
		/// @return Reference to the std::filesystem::path for the specified folder.
		virtual const fo::fs::path& getFSPath(fo::FolderType folderType) const = 0;

		/// @brief Fills outPathName with the path to the application tracer log file.
		/// @param outPathName Output string to receive the tracer path.
		virtual void getTracerPath(td::String& outPathName) = 0;

		/// @brief Fills outPathName with the application-specific local data path.
		/// @param outPathName Output string to receive the local data path.
		virtual void getLocalDataAppPath(td::String& outPathName) const = 0;

		/// @brief Returns the application-specific local data path as a string.
		/// @param bMakeNative If true, returns the path in native OS format.
		/// @return Local data application path string.
        virtual td::String getLocalDataAppPath(bool bMakeNative = false) const = 0;

		/// @brief Creates a subfolder under the specified parent folder type.
		/// @param parentFolderType The logical parent folder in which to create the subfolder.
		/// @param folderName Name of the subfolder to create.
		/// @param outFolderName Output string receiving the full path of the created folder.
		/// @return True if the folder was created successfully; false otherwise.
		virtual bool createFolder(fo::FolderType parentFolderType, const td::String& folderName, td::String& outFolderName) const = 0;

		//should be called first in main (to delete all undeleted tmp folders after last restart)
		/// @brief Removes all temporary application folders left over from previous sessions.
		virtual void deleteAppFoldersInTmp() = 0;

		/// @brief Creates a uniquely named temporary folder for the application.
		/// @param useTime If true, incorporates the current timestamp into the folder name.
		/// @param addForDeletion If true, registers the folder for automatic cleanup on exit.
		/// @return Full path of the newly created temporary folder.
		virtual td::String createAppFolderInTmp(bool useTime = true, bool addForDeletion = true) = 0;

		/// @brief Generates a unique file path in the temporary folder with the given prefix and extension.
		/// @param filePrefix Prefix for the temporary file name.
		/// @param fileExtension File extension (without dot) for the temporary file.
		/// @return Unique file path string in the temporary folder.
		virtual td::String getUniqueFileNameInTmp(const char* filePrefix, const char* fileExtension) = 0;

		/// @brief Checks whether a file exists inside the specified parent folder.
		/// @param parentFolderType The logical parent folder to search.
		/// @param fileName Name of the file to look for.
		/// @param outFileName Output string receiving the full path if the file is found.
		/// @return Enumerated file existence status.
		virtual fo::FileExist fileExists(fo::FolderType parentFolderType, const td::String& fileName, td::String& outFileName) const = 0;

		/// @brief Schedules a file for deletion when the application exits.
		/// @param fileName Full path of the file to delete on shutdown.
		virtual void addFileForDeletion(const td::String& fileName) = 0;

		/// @brief Schedules a folder for deletion when the application exits.
		/// @param folderName Full path of the folder to delete on shutdown.
		virtual void addFolderForDeletion(const td::String& folderName) = 0;

		/// @brief Returns the full path for a file located in the application's temporary folder.
		/// @param fileName Relative file name within the temporary folder.
		/// @return Full path string combining the temporary folder and the file name.
		virtual td::String getFileNameInTmpFolder(const td::String& fileName) const = 0;

		/// @brief Fills outFileName with the full path for a file in the application folder.
		/// @param fileName Relative file name within the application folder.
		/// @param outFileName Output string to receive the full path.
		virtual void getFileNameInAppFolder(const td::String& fileName, td::String& outFileName) const = 0;

		/// @brief Fills outFileName with the full path for a file in the user's home folder.
		/// @param fileName Relative file name within the home folder.
		/// @param outFileName Output string to receive the full path.
		virtual void getFileNameInHomeFolder(const td::String& fileName, td::String& outFileName) const = 0;

		//virtual void initApplicationRootFolder(const td::String& appNameWithPath, const td::String& applicationGroup) = 0;

		/// @brief Returns the root application installation folder path.
		/// @return Reference to the application folder path string.
		virtual const td::String& getApplicationFolder() const = 0;

		//overides OS settings (tmpFolder will be used as new TMP locaton)
		/// @brief Overrides the operating system temporary folder with a custom path.
		/// @param tmpFolder Full path to use as the new temporary folder location.
		virtual void setTmpFolder(const td::String& tmpFolder) = 0;

		/// @brief Returns the current temporary folder path.
		/// @return Reference to the temporary folder path string.
		virtual const td::String& getTmpFolder() const = 0;

		/// @brief Returns the operating system folder path (e.g., Windows system directory).
		/// @return Reference to the OS folder path string.
		virtual const td::String& getOSFolder() const = 0;

		/// @brief Returns the shared application data folder path (e.g., ProgramData on Windows).
		/// @return Reference to the common application data folder path string.
		virtual const td::String& getCommonAppDataFolder() const = 0;

		/// @brief Sets the application display name.
		/// @param appName The name to assign to the application.
		virtual void setAppName(const td::String& appName) = 0;

		/// @brief Returns the application display name.
		/// @return Reference to the application name string.
		virtual const td::String& getAppName() const = 0;

		/// @brief Returns the path to the current user's home directory.
		/// @return Reference to the home folder path string.
		virtual const td::String& getHomeFolder() const = 0;

		/// @brief Sets the application-level username used for logging and identification.
		/// @param userName The username to assign.
		virtual void setAppUserName(const td::String& userName) = 0;

		/// @brief Returns the application-level username.
		/// @return Reference to the application username string.
		virtual const td::String& getAppUserName() const = 0;

		/// @brief Returns the application executable name including its file extension.
		/// @return Reference to the application name-with-extension string.
		virtual const td::String& getAppNameWithExtension() const = 0;

		/// @brief Returns the OS-level username of the currently logged-in user.
		/// @return Reference to the OS username string.
		virtual const td::String& getUserName() const = 0;

		/// @brief Sets the application version numbers.
		/// @param versionNo Major version number.
		/// @param subVersionNo Minor version number.
		virtual void setVersionNo(int versionNo, int subVersionNo) = 0;

		/// @brief Returns the major version number.
		/// @return Major version number.
		virtual int getVersionNo() const = 0;

		/// @brief Returns the minor version number.
		/// @return Minor version number.
		virtual int getSubVersionNo() const = 0;

		/// @brief Returns the path to the local data folder within the user's home directory.
		/// @return Reference to the home local data folder path string.
		virtual const td::String& getHomeLocalDataFolder() const = 0;

		/// @brief Fills strLogPath with the path to the application log folder.
		/// @param strLogPath Output string to receive the log folder path.
		virtual void getLogFolder(td::String& strLogPath) const = 0;

		//NOVO
		/// @brief Associates a logger instance for regular application messages.
		/// @param pLogger Pointer to the ILogger to use for normal output.
		virtual void setLogger(const mu::ILogger* pLogger) = 0;

		/// @brief Returns the current application logger.
		/// @return Pointer to the currently assigned ILogger.
		virtual const mu::ILogger* getLogger() const = 0;

		//these two are just for debuging (messages are not visible on UI)
		/// @brief Associates a logger for debug-only messages (output is not shown in the UI).
		/// @param pLogger Pointer to the ILogger to use for debug output.
		virtual void setDebugLogger(const mu::ILogger* pLogger) = 0;

		/// @brief Returns the debug logger instance.
		/// @return Pointer to the currently assigned debug ILogger.
		virtual const mu::ILogger* getDebugLogger() const = 0;

		/// @brief Sets the file extension used when loading translation files.
		/// @param ext Extension string (e.g., "tr" or "lng").
		virtual void setTranslationExtension(const td::String& ext) = 0;

		/// @brief Returns the file extension used for translation files.
		/// @return Reference to the translation extension string.
		virtual const td::String& getTranslationExtension() const = 0;

		/// @brief Initialises the recordset configuration path from the given folder path.
		/// @param strConfigFolderPath Path to the configuration folder; defaults to current directory.
		virtual void initRecordsetConfigPath(const char* strConfigFolderPath = ".") = 0;

		/// @brief Returns the filesystem path to the data provider (DP) configuration directory.
		/// @return Reference to the DP configuration path.
		virtual const fo::fs::path& getDPConfigPath() const = 0;

		/// @brief Returns the filesystem path to the report configuration directory.
		/// @return Reference to the report configuration path.
		virtual const fo::fs::path& getReportConfigPath() const = 0;

		/// @brief Returns the filesystem path to the application resources directory.
		/// @return Reference to the resources path.
		virtual const fo::fs::path& getResPath() const = 0;

		/// @brief Returns the filesystem path to the GUI resources directory.
		/// @return Reference to the GUI resources path.
		virtual const fo::fs::path& getGUIPath() const = 0;

		/// @brief Returns the filesystem path to the translation files directory.
		/// @return Reference to the translation files path.
		virtual const fo::fs::path& getTranslationPath() const = 0;

		/// @brief Returns the filesystem path to the application help files directory.
		/// @return Reference to the help files path.
		virtual const fo::fs::path& getHelpPath() const = 0;

		/// @brief Returns the filesystem path to the application log files directory.
		/// @return Reference to the log files path.
        virtual const fo::fs::path& getLogPath() const = 0;

		/// @brief Releases this settings object and frees associated resources.
		virtual void release() = 0;
	};
}
