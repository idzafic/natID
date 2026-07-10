// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IApplication.h
 *  @brief Central application interface providing access to settings, translations, GUI, and global services.
 */
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


/// @brief Core application singleton that owns all global subsystems including settings, translations, and logging.
class MAINUTILS_API IApplication
{
    friend class AppProperties;
public:
    /// @brief Platform-specific COM/socket initialisation flags (Windows only).
    enum class Init : td::BYTE { Win_InitNone = 0,        ///< No special Windows initialisation.
                                 Win_InitCom = 1,          ///< Initialise COM.
                                 Win_InitSocket = 2,       ///< Initialise Winsock.
                                 Win_InitComAndSocket = 3, ///< Initialise both COM and Winsock.
                                 Win_InitOLE = 4           ///< Initialise OLE.
                               };

    /// @brief Bitmask flags controlling which resource sub-directories are set up during environment initialisation.
    enum class GenInit : td::UINT4 { GI_None = 0,                           ///< No resource initialisation.
                                     GI_UseCurrentFolderForAppResources = 1, ///< Use current working directory for resources.
                                     GI_FT_DP=2,                             ///< Set up data-provider config path.
                                     GI_FT_Res=4,                            ///< Set up resources path.
                                     GI_FT_Help=8,                           ///< Set up help path.
                                     GI_FT_GUI=16,                           ///< Set up GUI resources path.
                                     GI_FT_Tr=32,                            ///< Set up translation path.
                                     GI_FT_Rep=64,                           ///< Set up report config path.
                                     GI_FT_ALL=126,                          ///< Set up all standard resource paths.
                                     GI_Colors=512                           ///< Load colour name definitions.
                                   };

    /// @brief Indicates whether the application is running in a development or deployed environment.
    enum class Environment : td::BYTE {Unknown=0,     ///< Environment has not been determined.
                                       Development,   ///< Running from a development build tree.
                                       Deplyoment     ///< Running from an installed deployment.
                                      };

protected:
    mu::IAppProperties* _pProperties = nullptr;     ///< Pointer to the persistent application properties store.
    td::String _appName;                            ///< Display name of the application.
    //td::String _userHomeFolder;
    fo::fs::path _appFolderPath;                    ///< Filesystem path to the application installation folder.
    fo::fs::path _appResPath;                       ///< Filesystem path to the application resources folder.

    mu::Regionals* _mainThreadRegionals = nullptr;  ///< Locale/regional formatting object for the main thread.
    mu::IAppSettings* _pSettings = nullptr;         ///< Pointer to the application settings interface.

    mu::TranslationMgrBase<struct _NTR>* _translationMgr = nullptr;       ///< Translation manager for UI strings.
    mu::TranslationMgrBase<struct _NTR>* _translationMgrRep = nullptr;    ///< Translation manager for report strings.
    mu::TranslationMgrBase<struct _DPTR>* _translationMgrDP = nullptr;    ///< Translation manager for data-provider strings.
    mu::TranslationMgrBase<struct _DPTR>* _translationMgrMenu = nullptr;  ///< Translation manager for menu strings.

    mu::TxtOutFile<std::ofstream>* _tracer = nullptr; ///< File-based trace/debug output stream.
    mu::ColorNames* _colors = nullptr;                ///< Named colour registry.

    web::IView* _pHelpView = nullptr;          ///< Embedded web view used to display help content.
    const char* _mainWndHelpFile = nullptr;    ///< Path to the help file associated with the main window.
    const char** _argv = nullptr;              ///< Command-line argument vector passed at startup.
    const char* _applicationGroup = nullptr;   ///< Application group identifier string.
    const char* _logFileLocation = nullptr;    ///< Path or name of the application log file.
    int _argc = 0;                             ///< Number of command-line arguments.
    Environment _environment = Environment::Unknown; ///< Detected runtime environment.
    //GenInit _genInitFlag = GI_None;

    /// @brief Creates regional formatting objects using the default locale.
    /// @param desc Description label attached to the Regionals object.
    void createRegionals(const char* desc);

    /// @brief Creates regional formatting objects from embedded resource locale data.
    /// @param desc Description label attached to the Regionals object.
    void createRegionalsFromRes(const char* desc);

    /// @brief Initialises the application with a type-definition (td) library version check.
    /// @param pAppGroup Application group identifier.
    /// @param pAppName Application name.
    void initWithTdCheck(const char* pAppGroup, const char* pAppName);

    /// @brief Returns the string ID of the main toolbar definition resource.
    /// @return Null-terminated string ID for the main toolbar.
    virtual const char* getMainToolBarStrID() const;
    //void initEnvironment(const char* commandLine, const char* applicationGroup, const char* logFileLocation, bool colorsNeeded);

public:

    /// @brief Constructs the application object with command-line arguments and basic configuration.
    /// @param argc Number of command-line arguments.
    /// @param argv Array of command-line argument strings.
    /// @param applicationGroup Application group identifier used for settings scoping.
    /// @param logFileLocation Path to the log file; may be nullptr to disable file logging.
    /// @param createLogFile True to create a log file on startup.
    IApplication(int argc, const char** argv, const char* applicationGroup, const char* logFileLocation, bool createLogFile);

    /// @brief Destructor that releases all owned subsystems.
    ~IApplication();

    /// @brief Sets up resource paths and environment detection.
    /// @param genInit Bitmask of GenInit flags controlling which resource paths are initialised.
    void initEnvironment(td::UINT4 genInit = (td::UINT4) GenInit::GI_FT_ALL);

    /// @brief Initialises the regional/locale subsystem for the specified translation extension.
    /// @param translationExtension File extension identifying the translation files to load.
    void initRegionals(const td::String& translationExtension);

    /// @brief Convenience method combining initEnvironment and initRegionals.
    /// @param translationExtension File extension for translation files.
    /// @param genInit Bitmask of GenInit flags controlling resource path setup.
    void init(const td::String& translationExtension, td::UINT4 genInit = (td::UINT4) GenInit::GI_FT_ALL);

    /// @brief Switches the active UI language by name.
    /// @param pStrLangName Null-terminated language name string (e.g., "en", "de").
    void setLanguage(const char* pStrLangName);

    /// @brief Switches the active UI language by name.
    /// @param langName Language name string.
    void setLanguage(const td::String& langName);

    /// @brief Returns the currently active UI language name.
    /// @return Reference to the current language name string.
    const td::String& getLanguage() const;

    /// @brief Performs any GUI-specific initialisation required after the application is set up.
    void initGUI();

    /// @brief Returns the unique numeric identifier for this application instance.
    /// @return 32-bit application ID.
    td::UINT4 getID();

    /// @brief Returns the application settings interface.
    /// @return Pointer to the IAppSettings instance.
    mu::IAppSettings* getSettings();

    /// @brief Returns the application properties interface.
    /// @return Pointer to the IAppProperties instance.
    mu::IAppProperties* getProperties();

    /// @brief Returns the display name of the application.
    /// @return Reference to the application name string.
    const td::String& getName() const;

    //const td::String& getFolder() const;
    /// @brief Returns the filesystem path to the application folder.
    /// @return Reference to the application folder path.
    const fo::fs::path& getFolderPath() const;
//        const td::String& getNameWithExtension() const;

    /// @brief Returns the detected runtime environment.
    /// @return Environment value indicating development or deployment context.
    Environment getEnvironment() const;

    /// @brief Returns the Regionals object associated with the main thread.
    /// @return Pointer to the main-thread Regionals instance.
    Regionals* getMainThreadRegionals();

    //const td::String& getHomeFolderName() const;
    /// @brief Returns the string path for the given logical folder type.
    /// @param folderType The folder type to query.
    /// @return Reference to the path string for that folder.
    const td::String& getPath(fo::FolderType folderType) const;

    /// @brief Returns the filesystem path for the given logical folder type.
    /// @param folderType The folder type to query.
    /// @return Reference to the std::filesystem::path for that folder.
    const fo::fs::path& getFSPath(fo::FolderType folderType) const;

    /// @brief Creates a named subfolder under the specified parent folder type.
    /// @param parentFolderType Logical parent folder in which to create the subfolder.
    /// @param folderName Name of the subfolder to create.
    /// @param outFolderName Output string receiving the full path of the created folder.
    /// @return True on success; false on failure.
    bool createFolder(fo::FolderType parentFolderType, const td::String& folderName, td::String& outFolderName) const;

    /// @brief Returns the file-based trace output stream.
    /// @return Pointer to the TxtOutFile tracer; may be nullptr if tracing is disabled.
    mu::TxtOutFile<std::ofstream>* getTracer();

    /// @brief Returns the translation manager for main UI strings.
    /// @return Pointer to the NTR translation manager.
    mu::TranslationMgrBase<struct _NTR>* getTranslationMgr();

    /// @brief Returns the translation manager for report strings.
    /// @return Pointer to the NTR report translation manager.
    mu::TranslationMgrBase<struct _NTR>* getTranslationMgrRep();

    /// @brief Returns the translation manager for data-provider strings.
    /// @return Pointer to the DPTR data-provider translation manager.
    mu::TranslationMgrBase<struct _DPTR>* getTranslationMgrDP();

    /// @brief Returns the translation manager for menu strings.
    /// @return Pointer to the DPTR menu translation manager.
    mu::TranslationMgrBase<struct _DPTR>* getTranslationMgrMenu();

    //virtual methods
    /// @brief Returns the primary application logger.
    /// @return Pointer to the active ILogger; must be overridden by derived classes.
    virtual mu::ILogger* getLogger() = 0;

    /// @brief Returns the debug logger used for internal diagnostics.
    /// @return Pointer to the debug ILogger; must be overridden by derived classes.
    virtual mu::ILogger* getDbgLogger() = 0;

    /// @brief Registers an object with the global releaser so it is released on shutdown.
    /// @param pObj Pointer to the IRelease object to register.
    virtual void addToGlobalReleaser(mu::IRelease* pObj);

    /// @brief Unregisters an object from the global releaser.
    /// @param pObj Pointer to the IRelease object to remove.
    virtual void removeFromGlobalReleaser(mu::IRelease* pObj);

    /// @brief Returns a pointer to the main application window (platform-specific type).
    /// @return Opaque pointer to the main window handle.
    virtual void* getMainWnd();

    /// @brief Sets the main application window handle.
    /// @param pWnd Opaque pointer to the main window handle.
    virtual void setMainWnd(void* pWnd);

    //virtual void onInputParam(int iParam, const char* param);

    /// @brief Returns the count of command-line arguments passed at startup.
    /// @return Number of input parameters.
    int getNumberOfInputParams() const;

    /// @brief Returns the raw command-line argument vector.
    /// @return Pointer to the array of argument strings.
    const char** getInpurParams() const;

    /// @brief Sets the web view used to display the application help.
    /// @param pView Pointer to the IView to use as the help window.
    virtual void setHelpWnd(web::IView*);

    /// @brief Returns the web view currently used to display the application help.
    /// @return Pointer to the current help IView; may be nullptr.
    virtual web::IView* getHelpWnd();

    /// @brief Sets the help file path associated with the main application window.
    /// @param mainWndHelpFile Null-terminated path to the main window help file.
    void setMainWndHelpFile(const char* mainWndHelpFile);

    /// @brief Returns the help file path for the main application window.
    /// @return Null-terminated path to the main window help file.
    const char* getMainWndHelpFile() const;

    /// @brief Looks up the 32-bit ARGB colour value for a named colour.
    /// @param colorName Name of the colour to look up.
    /// @return 32-bit ARGB colour value, or 0 if not found.
    td::UINT4 getKnownColor(const td::String& colorName) const;

    /// @brief Returns the name of the colour corresponding to the given ARGB value.
    /// @param colorVal 32-bit ARGB colour value.
    /// @return Name string for the colour.
    td::String getKnownColorName(td::UINT4) const;

    /// @brief Closes all floating/non-modal windows owned by the application.
    virtual void closeFreeWnds();

    /// @brief Returns both the argc and argv from the original main() invocation.
    /// @return Tuple containing (argc, argv).
    std::tuple<int, const char**> getMainArgs() const;

    /// @brief Resolves a resource file name relative to the given path and returns the full path.
    /// @param relativePath Base path relative to which the resource is located.
    /// @param resFileName Resource file name to expand.
    /// @return Full expanded path as a td::String.
    td::String expandResFileName(const fo::fs::path& relativePath, const td::String& resFileName) const;

    /// @brief Resolves a resource file name relative to the given path and returns the full path.
    /// @param relativePath Base path relative to which the resource is located.
    /// @param resFileName Null-terminated resource file name to expand.
    /// @return Full expanded path as a td::String.
    td::String expandResFileName(const fo::fs::path& relativePath, const char* resFileName) const;

    /// @brief Returns the filesystem path to the application resources directory.
    /// @return Reference to the resources directory path.
    const fo::fs::path& getResPath() const;

    /// @brief Returns a pointer to the global IApplication singleton.
    /// @return Pointer to the application instance; nullptr if not yet created.
    static IApplication* getApplication();
};

//MAINUTILS_API mu::IApplication* pApp();
}
