// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "natGUI.h"
#include <td/String.h>
#include "Types.h"
#include <tuple>
#include <td/Color.h>
#include <td/ColorID.h>
#include <gui/Font.h>
#include <functional>
#include "Lang.h"
#include <cnt/SafeFullVector.h>
#include <mu/IApplication.h>

namespace gui
{
class NatObjHelper;
class Application;
class Window;

typedef std::function<void()> AsyncFn;
typedef void (*AsyncFn1)(void*);

class NATGUI_API NatObject
{
    friend class NatObjHelper;
    
protected:
    gui::Handle _handle = nullptr;
protected:
    NatObject();
    virtual ~NatObject();
    void createObject(const gui::Geometry* pg); //Window
    void createObject(const char* pStrID); //Image
    void createObject(const gui::Size& sz);
    void createObject(const td::String& str);
    void createObject(const td::String& str, gui::Font::ID fntID);
    void release();
    virtual void destroy();
    void resetHandle(); //just for window when is closed from outside
    mu::IAppProperties* getAppProperties();
    const mu::IAppProperties* getAppProperties() const;
    void copyToClipboard(const char* pStr) const;
    void copyToClipboard(const td::String& str) const;
public:
    gui::Handle getHandle() const;
    std::tuple<gui::ObjType, gui::Handle> getNatInfo() const;
    void showAlert(const td::String& msgText, const td::String& informativeText) const;
    virtual gui::ObjType getObjType() const { return ObjType::NA;}
    //virtual gui::CtrlType getCtrlType() const { return CtrlType::NA;}
    bool isControl() const;
    //call function from another thread and execute it in main (GUI) thread
    static void asyncCall(AsyncFn* pFn, bool bDeleteFnPtrAfterCall = false);
    static void asyncCall1(AsyncFn1 pFn, void* userData);
    ///translation
    td::String tr(const char* strKey) const;
    const char* trCStr(const char* strKey) const;
    
    td::String getResText(const char* strKey) const;
    td::String getResFileName(const char* strKey) const;
    td::String getMenuResFileName(const char* strKey) const;
    
    const cnt::SafeFullVector<gui::Lang>& getSupportedLanguages() const;
    int getTranslationLanguageIndex() const;
    
    const gui::Lang& getTranslationLang() const;
    
    virtual td::UINT4 getVisualID() const;
    
    gui::Application* getApplication();
    const gui::Application* getApplication() const;
    
    gui::Window* getInitialWindow();
    const gui::Window* getInitialWindow() const;
};

} //namespace gui
