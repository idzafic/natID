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
#include <td/String.h>
#include <fo/FileOperations.h>

namespace mu
{	
	class ILogger;

	class MAINUTILS_API IAppSettings
	{			

	public:
		
		virtual td::UINT4 getProgID() = 0;

		virtual const td::String& getPath(fo::FolderType folderType) const = 0;

		virtual const fo::fs::path& getFSPath(fo::FolderType folderType) const = 0;
		
		virtual void getTracerPath(td::String& outPathName) = 0;		

		virtual void getLocalDataAppPath(td::String& outPathName) const = 0;
		
		virtual bool createFolder(fo::FolderType parentFolderType, const td::String& folderName, td::String& outFolderName) const = 0;

		//should be called first in main (to delete all undeleted tmp folders after last restart)
		virtual void deleteAppFoldersInTmp() = 0;

		virtual td::String createAppFolderInTmp(bool useTime = true, bool addForDeletion = true) = 0;

		virtual td::String getUniqueFileNameInTmp(const char* filePrefix, const char* fileExtension) = 0;

		virtual fo::FileExist fileExists(fo::FolderType parentFolderType, const td::String& fileName, td::String& outFileName) const = 0;
				
		virtual void addFileForDeletion(const td::String& fileName) = 0;
		
		virtual void addFolderForDeletion(const td::String& folderName) = 0;		
		
		virtual td::String getFileNameInTmpFolder(const td::String& fileName) const = 0;
		
		virtual void getFileNameInAppFolder(const td::String& fileName, td::String& outFileName) const = 0;		

		virtual void getFileNameInHomeFolder(const td::String& fileName, td::String& outFileName) const = 0;
		
		//virtual void initApplicationRootFolder(const td::String& appNameWithPath, const td::String& applicationGroup) = 0;				

		virtual const td::String& getApplicationFolder() const = 0;
		
		//overides OS settings (tmpFolder will be used as new TMP locaton)
		virtual void setTmpFolder(const td::String& tmpFolder) = 0;		

		virtual const td::String& getTmpFolder() const = 0;		

		virtual const td::String& getOSFolder() const = 0;
		
		virtual const td::String& getCommonAppDataFolder() const = 0;		

		virtual void setAppName(const td::String& appName) = 0;
		
		virtual const td::String& getAppName() const = 0;		

		virtual const td::String& getHomeFolder() const = 0;
		
		virtual void setAppUserName(const td::String& userName) = 0;
		
		virtual const td::String& getAppUserName() const = 0;
		
		virtual const td::String& getAppNameWithExtension() const = 0;		

		virtual const td::String& getUserName() const = 0;
		
		virtual void setVersionNo(int versionNo, int subVersionNo) = 0;
		
		virtual int getVersionNo() const = 0;
		
		virtual int getSubVersionNo() const = 0;
		
		virtual const td::String& getHomeLocalDataFolder() const = 0;		

		virtual void getLogFolder(td::String& strLogPath) const = 0;		

		//NOVO
		virtual void setLogger(const mu::ILogger* pLogger) = 0;
		virtual const mu::ILogger* getLogger() const = 0;

		//these two are just for debuging (messages are not visible on UI)
		virtual void setDebugLogger(const mu::ILogger* pLogger) = 0;
		virtual const mu::ILogger* getDebugLogger() const = 0;

		virtual void setTranslationExtension(const td::String& ext) = 0;
		virtual const td::String& getTranslationExtension() const = 0;

		virtual void initRecordsetConfigPath(const char* strConfigFolderPath = ".") = 0;
		virtual const fo::fs::path& getDPConfigPath() const = 0;
		virtual const fo::fs::path& getReportConfigPath() const = 0;
		virtual const fo::fs::path& getResPath() const = 0;
		virtual const fo::fs::path& getGUIPath() const = 0;
		virtual const fo::fs::path& getTranslationPath() const = 0;
		virtual const fo::fs::path& getHelpPath() const = 0;
        virtual const fo::fs::path& getLogPath() const = 0;

		virtual void release() = 0;
	};
}
