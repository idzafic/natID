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
#include <mu/Logger.h>
#include <mu/IRelease.h>
#include <mu/Regionals.h>
#include <mu/IAppSettings.h>
#include <mu/TxtOutFile.h>
#include <mu/ColorNames.h>
#include <mu/IAppProperties.h>
#include <tuple>
#include <fo/Definitions.h>

//class QWidget;

namespace web
{
class IView;
}

namespace mu
{
class AppProperties;

//some forward declarations
template<typename T>
class TranslationMgrBase;
struct _NTR;
struct _DPTR;


class MAINUTILS_API IApplication
{
    friend class AppProperties;
public:
    enum class Init : td::BYTE { Win_InitNone = 0, Win_InitCom = 1, Win_InitSocket = 2, Win_InitComAndSocket = 3, Win_InitOLE = 4 };
    enum class GenInit : td::UINT4 { GI_None = 0, GI_UseCurrentFolderForAppResources = 1, GI_FT_DP=2, GI_FT_Res=4, GI_FT_Help=8, GI_FT_GUI=16, GI_FT_Tr=32, GI_FT_Rep=64, GI_FT_ALL=126, GI_Colors=512} ;
    enum class Environment : td::BYTE {Unknown=0, Development, Deplyoment};
    
protected:
    mu::IAppProperties* _pProperties = nullptr;
    td::String _appName;
    //td::String _userHomeFolder;
    fo::fs::path _appFolderPath;
    fo::fs::path _appResPath;
    
    mu::Regionals* _mainThreadRegionals = nullptr;
    mu::IAppSettings* _pSettings = nullptr;
    
    mu::TranslationMgrBase<struct _NTR>* _translationMgr = nullptr;
    mu::TranslationMgrBase<struct _NTR>* _translationMgrRep = nullptr;
    mu::TranslationMgrBase<struct _DPTR>* _translationMgrDP = nullptr;
    mu::TranslationMgrBase<struct _DPTR>* _translationMgrMenu = nullptr;
            
    mu::TxtOutFile<std::ofstream>* _tracer = nullptr;
    mu::ColorNames* _colors = nullptr;

    web::IView* _pHelpView = nullptr;
    const char* _mainWndHelpFile = nullptr;
    const char** _argv = nullptr;
    const char* _applicationGroup = nullptr;
    const char* _logFileLocation = nullptr;
    int _argc = 0;
    Environment _environment = Environment::Unknown;
    //GenInit _genInitFlag = GI_None;

    void createRegionals(const char* desc);
    void createRegionalsFromRes(const char* desc);
    
    void initWithTdCheck(const char* pAppGroup, const char* pAppName);
    virtual const char* getMainToolBarStrID() const;
    //void initEnvironment(const char* commandLine, const char* applicationGroup, const char* logFileLocation, bool colorsNeeded);
    
public:
    
    IApplication(int argc, const char** argv, const char* applicationGroup, const char* logFileLocation, bool createLogFile);
    ~IApplication();

    void initEnvironment(td::UINT4 genInit = (td::UINT4) GenInit::GI_FT_ALL);
    void initRegionals(const td::String& translationExtension);
    
    void init(const td::String& translationExtension, td::UINT4 genInit = (td::UINT4) GenInit::GI_FT_ALL);

    void setLanguage(const char* pStrLangName);
    void setLanguage(const td::String& langName);
    const td::String& getLanguage() const;

    void initGUI();

    td::UINT4 getID();

    mu::IAppSettings* getSettings();
    mu::IAppProperties* getProperties();
    const td::String& getName() const;
    //const td::String& getFolder() const;
    const fo::fs::path& getFolderPath() const;
//        const td::String& getNameWithExtension() const;
    
    Environment getEnvironment() const;
    
    Regionals* getMainThreadRegionals();
    const td::String& getHomeFolderName() const;
    const td::String& getPath(fo::FolderType folderType) const;
    const fo::fs::path& getFSPath(fo::FolderType folderType) const;
    bool createFolder(fo::FolderType parentFolderType, const td::String& folderName, td::String& outFolderName) const;

    mu::TxtOutFile<std::ofstream>* getTracer();

    mu::TranslationMgrBase<struct _NTR>* getTranslationMgr();
    mu::TranslationMgrBase<struct _NTR>* getTranslationMgrRep();
    mu::TranslationMgrBase<struct _DPTR>* getTranslationMgrDP();
    mu::TranslationMgrBase<struct _DPTR>* getTranslationMgrMenu();
    
    //virtual methods
    virtual mu::ILogger* getLogger() = 0;
    virtual mu::ILogger* getDbgLogger() = 0;
    virtual void addToGlobalReleaser(mu::IRelease* pObj);
    virtual void removeFromGlobalReleaser(mu::IRelease* pObj);

    virtual void* getMainWnd();
    virtual void setMainWnd(void* pWnd);

    virtual void setHelpWnd(web::IView*);
    virtual web::IView* getHelpWnd();
    void setMainWndHelpFile(const char* mainWndHelpFile);
    const char* getMainWndHelpFile() const;
    td::UINT4 getKnownColor(const td::String& colorName) const;
    td::String getKnownColorName(td::UINT4) const;
    virtual void closeFreeWnds();
    std::tuple<int, const char**> getMainArgs() const;
    td::String expandResFileName(const fo::fs::path& homePath, const td::String& resFileName) const;
    td::String expandResFileName(const fo::fs::path& homePath, const char* resFileName) const;
};

MAINUTILS_API mu::IApplication* pApp();
}
