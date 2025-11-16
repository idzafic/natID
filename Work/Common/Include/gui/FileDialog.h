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
#include <gui/Consumer.h>
#include <cnt/SafeFullVector.h>
#include <td/Variant.h>
#include <gui/Frame.h>

namespace gui
{
//class Window;
class Frame;
class FileDialogHelper;
class IMessageConsumer;

class NATGUI_API FileDialog : public Consumer
{
    friend class FileDialogHelper;
    
public:
    typedef struct _patern
    {
        td::String docDescription;
        const char* pattern; //*.txt, *.bin, etc...
    } Pattern;
    
    enum class Status : td::BYTE {Cancel=0, OK};
    enum class Type : td::BYTE {OpenFile, SaveFile, OpenFolder, CreateFolder};
    enum class EventHandlerType : td::BYTE { Consumer = 0, FunctionPtr, FunctionForOK };
    typedef std::function<void(FileDialog*)> CallBack;
protected:
    Frame* _pFrame;
    FileDialog::CallBack* _pCallBackFn = nullptr;
    FileDialog::CallBack _callBack;
    td::String _fileOrFolderName;
    td::String _defaultExtension;
    cnt::SafeFullVector<td::String> _filters; //pattern or extension
    td::Variant _attachedValue;
    td::UINT4 _wndID;
    EventHandlerType _eventHandlerType;
    Type _type;
    Status _status;
    td::BYTE _isOpen = 0;
private:
    void handleDlg();
    void destroyCPP();
protected:
    void destroy() override;
    FileDialog::CallBack* getCallBackFn();
    gui::FileDialog::EventHandlerType getEventHandlerType() const;
    const td::String& getDefaultExtension() const;
    const td::String& getFilter(int pos) const;
    
    //save with different extensions
    FileDialog(Frame* pFrame, const td::String& dlgTitle, const std::initializer_list<gui::FileDialog::Pattern>& allowedDocDescriptions, const char* defaultFileName = nullptr);
    //for all (when Type==OpenFile only one extension) - save also
    FileDialog(Frame* pFrame, Type type, const td::String& dlgTitle, const char* defaultExtension = nullptr, const td::String& strAcceptButtonLbl="", const char* defaultFileOrFolderName = nullptr);
    //for Type==OpenFilewith different extension types (*.
    FileDialog(const std::initializer_list<gui::FileDialog::Pattern>& allowedTypes, Frame* pFrame, const td::String& dlgTitle,  const td::String& strAcceptButtonLbl="");
    
    void open(FileDialog::CallBack* pCallBackFn);

    void open(std::function<void(gui::FileDialog*)> fnToCallOnOK); //modal mode with callback function
    
//    template <class TINT>
//    void open(TINT wndID, std::function<void(gui::FileDialog*)> fnToCallOnOK)
//    {
//        _openWithID((td::UINT4) wndID, fnToCallOnOK);
//    }
//    
//    template <class TINT>
//    void open(TINT wndID, gui::IMessageConsumer* pConsumer = nullptr)
//    {
//        _openWithID((td::UINT4) wndID, pConsumer);
//    }
    
public:
    ~FileDialog();
    
    gui::ObjType getObjType() const override { return ObjType::FileDialog;}

    void setFocus();
    
    void close(); //for async mode
    
    td::String getFileName() const;
    td::UINT4 getVisualID() const override;
    Status getStatus() const;
    Type getType() const;
    const Frame* getFrame() const;
    const char* getFileExtension() const;
    
    template <typename TVAL>
    void attachValue(const TVAL& val)
    {
        _attachedValue = val;
    }
    
    template <typename TVAL>
    void getAttachedValue(TVAL& val)
    {
        _attachedValue.getValue(val);
    }
    
    void _openWithID(td::UINT4 wndID, gui::IMessageConsumer* pConsumer);
    void _openWithID(td::UINT4 wndID, std::function<void(gui::FileDialog*)> fnToCallOnOK);
    void _openWithID(td::UINT4 wndID, FileDialog::CallBack* pCallBackFn);
    
    static FileDialog* getAttached(gui::Frame* pFrame, td::UINT4 wndID);
    
    bool isOpen() const;
    
};

class NATGUI_API OpenFileDialog : public FileDialog
{
protected:
    OpenFileDialog(Frame* pFrame, const td::String& dlgTitle, const char* defaultExtension, const td::String& strOpenButtonLbl="");
    
    OpenFileDialog(Frame* pFrame, const td::String& dlgTitle, const std::initializer_list<gui::FileDialog::Pattern>& allowedTypes, const td::String& strOpenButtonLbl="");
    
public:
    template <typename DLGID>
    static FileDialog* show(Frame* pFrame, const td::String& dlgTitle, const char* defaultExtension, DLGID wndID, gui::IMessageConsumer* pConsumer = nullptr, const td::String& strOpenButtonLbl="")
    {
        FileDialog* pFD = FileDialog::getAttached(pFrame, (td::UINT4) wndID);
        if (pFD)
        {
            pFD->setFocus();
            return pFD;
        }
        pFD = new OpenFileDialog(pFrame, dlgTitle, defaultExtension, strOpenButtonLbl);
        if (!pConsumer)
            pConsumer = pFrame->getMessageConsumer();
        pFD->_openWithID((td::UINT4) wndID, pConsumer);
        return pFD;
    }
    
    template <typename DLGID>
    static FileDialog* show(Frame* pFrame, const td::String& dlgTitle, const char* defaultExtension, DLGID wndID, std::function<void(gui::FileDialog*)> fnToCallOnOK, const td::String& strOpenButtonLbl="")
    {
        FileDialog* pFD = FileDialog::getAttached(pFrame, (td::UINT4) wndID);
        if (pFD)
        {
            pFD->setFocus();
            return pFD;
        }
        
        pFD = new OpenFileDialog(pFrame, dlgTitle, defaultExtension, strOpenButtonLbl);
        pFD->_openWithID((td::UINT4) wndID, fnToCallOnOK);
        return pFD;
    }
    
    template <typename DLGID>
    static FileDialog* show(Frame* pFrame, const td::String& dlgTitle, const char* defaultExtension, DLGID wndID, FileDialog::CallBack* pCallBackFn, const td::String& strOpenButtonLbl="")
    {
        FileDialog* pFD = FileDialog::getAttached(pFrame, (td::UINT4) wndID);
        if (pFD)
        {
            pFD->setFocus();
            return pFD;
        }
        
        pFD = new OpenFileDialog(pFrame, dlgTitle, defaultExtension, strOpenButtonLbl);
        pFD->_openWithID((td::UINT4) wndID, pCallBackFn);
        return pFD;
    }
    
    template <typename DLGID>
    static FileDialog* show(Frame* pFrame, const td::String& dlgTitle, const std::initializer_list<gui::FileDialog::Pattern>& allowedTypes, DLGID wndID, gui::IMessageConsumer* pConsumer = nullptr, const td::String& strOpenButtonLbl="")
    {
        FileDialog* pFD = FileDialog::getAttached(pFrame, (td::UINT4) wndID);
        if (pFD)
        {
            pFD->setFocus();
            return pFD;
        }
        pFD = new OpenFileDialog(pFrame, dlgTitle, allowedTypes, strOpenButtonLbl);
        if (!pConsumer)
            pConsumer = pFrame->getMessageConsumer();
        pFD->_openWithID((td::UINT4) wndID, pConsumer);
        return pFD;
    }
    
    template <typename DLGID>
    static FileDialog* show(Frame* pFrame, const td::String& dlgTitle, const std::initializer_list<gui::FileDialog::Pattern>& allowedTypes, DLGID wndID, std::function<void(gui::FileDialog*)> fnToCallOnOK, const td::String& strOpenButtonLbl="")
    {
        FileDialog* pFD = FileDialog::getAttached(pFrame, (td::UINT4) wndID);
        if (pFD)
        {
            pFD->setFocus();
            return pFD;
        }
        pFD = new OpenFileDialog(pFrame, dlgTitle, allowedTypes, strOpenButtonLbl);
        pFD->_openWithID((td::UINT4) wndID, fnToCallOnOK);
        return pFD;
    }
    
    template <typename DLGID>
    static FileDialog* show(Frame* pFrame, const td::String& dlgTitle, const std::initializer_list<gui::FileDialog::Pattern>& allowedTypes, DLGID wndID, FileDialog::CallBack* pCallBackFn, const td::String& strOpenButtonLbl="")
    {
        FileDialog* pFD = FileDialog::getAttached(pFrame, (td::UINT4) wndID);
        if (pFD)
        {
            pFD->setFocus();
            return pFD;
        }
        
        pFD = new OpenFileDialog(pFrame, dlgTitle, allowedTypes, strOpenButtonLbl);
        pFD->_openWithID((td::UINT4) wndID, pCallBackFn);
        return pFD;
    }
};

class NATGUI_API SaveFileDialog : public FileDialog
{
protected:
    SaveFileDialog(Frame* pFrame, const td::String& dlgTitle, const char* defaultExtension = nullptr, const td::String& strSaveButtonLbl="", const char* defaultFileOrFolderName = nullptr);
    
    SaveFileDialog(Frame* pFrame, const td::String& dlgTitle, const std::initializer_list<gui::FileDialog::Pattern>& allowedDocDescriptions, const char* defaultFileName = nullptr);
    
public:
    template <typename DLGID>
    static FileDialog* show(Frame* pFrame, const td::String& dlgTitle, const char* defaultExtension, DLGID wndID, gui::IMessageConsumer* pConsumer = nullptr, const td::String& strSaveButtonLbl="", const char* defaultFileOrFolderName = nullptr)
    {
        FileDialog* pFD = FileDialog::getAttached(pFrame, (td::UINT4) wndID);
        if (pFD)
        {
            pFD->setFocus();
            return pFD;
        }
        
        pFD = new SaveFileDialog(pFrame, dlgTitle, defaultExtension, strSaveButtonLbl, defaultFileOrFolderName);
        if (!pConsumer)
            pConsumer = pFrame->getMessageConsumer();
        pFD->_openWithID((td::UINT4) wndID, pConsumer);
        return pFD;
    }
    
    template <typename DLGID>
    static FileDialog* show(Frame* pFrame, const td::String& dlgTitle, const char* defaultExtension, DLGID wndID, std::function<void(gui::FileDialog*)> fnToCallOnOK, const td::String& strSaveButtonLbl="", const char* defaultFileOrFolderName = nullptr)
    {
        FileDialog* pFD = FileDialog::getAttached(pFrame, (td::UINT4) wndID);
        if (pFD)
        {
            pFD->setFocus();
            return pFD;
        }
        
        pFD = new SaveFileDialog(pFrame, dlgTitle, defaultExtension, strSaveButtonLbl, defaultFileOrFolderName);
        pFD->_openWithID((td::UINT4) wndID, fnToCallOnOK);
        return pFD;
    }
    
    template <typename DLGID, typename TATTACH>
    static FileDialog* showWithAttachment(Frame* pFrame, const td::String& dlgTitle, const char* defaultExtension, const TATTACH attachValue, DLGID wndID, std::function<void(gui::FileDialog*)> fnToCallOnOK, const td::String& strSaveButtonLbl="", const char* defaultFileOrFolderName = nullptr)
    {
        FileDialog* pFD = FileDialog::getAttached(pFrame, (td::UINT4) wndID);
        if (pFD)
        {
            pFD->setFocus();
            return pFD;
        }
        
        pFD = new SaveFileDialog(pFrame, dlgTitle, defaultExtension, strSaveButtonLbl, defaultFileOrFolderName);
        pFD->attachValue(attachValue);
        pFD->_openWithID((td::UINT4) wndID, fnToCallOnOK);
        return pFD;
    }
    
    template <typename DLGID>
    static FileDialog* show(Frame* pFrame, const td::String& dlgTitle, const char* defaultExtension, DLGID wndID, FileDialog::CallBack* pCallBackFn, const td::String& strSaveButtonLbl="", const char* defaultFileOrFolderName = nullptr)
    {
        FileDialog* pFD = FileDialog::getAttached(pFrame, (td::UINT4) wndID);
        if (pFD)
        {
            pFD->setFocus();
            return pFD;
        }
        
        pFD = new SaveFileDialog(pFrame, dlgTitle, defaultExtension, strSaveButtonLbl, defaultFileOrFolderName);
        pFD->_openWithID((td::UINT4) wndID, pCallBackFn);
        return pFD;
    }
    
    template <typename DLGID>
    static FileDialog* show(Frame* pFrame, const td::String& dlgTitle, DLGID wndID, gui::IMessageConsumer* pConsumer = nullptr, const char* defaultExtension = nullptr, const td::String& strSaveButtonLbl="", const char* defaultFileOrFolderName = nullptr)
    {
        FileDialog* pFD = FileDialog::getAttached(pFrame, (td::UINT4) wndID);
        if (pFD)
        {
            pFD->setFocus();
            return pFD;
        }
        
        pFD = new SaveFileDialog(pFrame, dlgTitle, defaultExtension, strSaveButtonLbl, defaultFileOrFolderName);
        if (!pConsumer)
            pConsumer = pFrame->getMessageConsumer();
        pFD->_openWithID((td::UINT4) wndID, pConsumer);
        return pFD;
    }
    
    template <typename DLGID>
    static FileDialog* show(Frame* pFrame, const td::String& dlgTitle, DLGID wndID, std::function<void(gui::FileDialog*)> fnToCallOnOK, const char* defaultExtension = nullptr, const td::String& strSaveButtonLbl="", const char* defaultFileOrFolderName = nullptr)
    {
        FileDialog* pFD = FileDialog::getAttached(pFrame, (td::UINT4) wndID);
        if (pFD)
        {
            pFD->setFocus();
            return pFD;
        }
        
        pFD = new SaveFileDialog(pFrame, dlgTitle, defaultExtension, strSaveButtonLbl, defaultFileOrFolderName);
        pFD->_openWithID((td::UINT4) wndID, fnToCallOnOK);
        return pFD;
    }
    
    template <typename DLGID>
    static FileDialog* show(Frame* pFrame, const td::String& dlgTitle, DLGID wndID, FileDialog::CallBack* pCallBackFn, const char* defaultExtension = nullptr, const td::String& strSaveButtonLbl="", const char* defaultFileOrFolderName = nullptr)
    {
        FileDialog* pFD = FileDialog::getAttached(pFrame, (td::UINT4) wndID);
        if (pFD)
        {
            pFD->setFocus();
            return pFD;
        }
        
        pFD = new SaveFileDialog(pFrame, dlgTitle, defaultExtension, strSaveButtonLbl, defaultFileOrFolderName);
        pFD->_openWithID((td::UINT4) wndID, pCallBackFn);
        return pFD;
    }
    
    template <typename DLGID>
    static FileDialog* show(Frame* pFrame, const td::String& dlgTitle, const std::initializer_list<gui::FileDialog::Pattern>& allowedDocDescriptions, DLGID wndID, gui::IMessageConsumer* pConsumer = nullptr, const char* defaultFileName = nullptr)
    {
        FileDialog* pFD = FileDialog::getAttached(pFrame, (td::UINT4) wndID);
        if (pFD)
        {
            pFD->setFocus();
            return pFD;
        }
        
        pFD = new SaveFileDialog(pFrame, dlgTitle, allowedDocDescriptions, defaultFileName);
        if (!pConsumer)
            pConsumer = pFrame->getMessageConsumer();
        pFD->_openWithID(wndID, pConsumer);
        return pFD;
    }
    
    template <typename DLGID>
    static FileDialog* show(Frame* pFrame, const td::String& dlgTitle, const std::initializer_list<gui::FileDialog::Pattern>& allowedDocDescriptions, DLGID wndID, std::function<void(gui::FileDialog*)> fnToCallOnOK, const char* defaultFileName = nullptr)
    {
        FileDialog* pFD = FileDialog::getAttached(pFrame, (td::UINT4) wndID);
        if (pFD)
        {
            pFD->setFocus();
            return pFD;
        }
        
        pFD = new SaveFileDialog(pFrame, dlgTitle, allowedDocDescriptions, defaultFileName);
        try
        {
            pFD->_openWithID(wndID, fnToCallOnOK);
        }
        catch (...)
        {
            mu::dbgLog("Error! Cannot create SaveFileDialog");
            delete pFD;
            pFD = nullptr;
        }
        
        return pFD;
    }
    
    template <typename DLGID>
    static FileDialog* show(Frame* pFrame, const td::String& dlgTitle, const std::initializer_list<gui::FileDialog::Pattern>& allowedDocDescriptions, DLGID wndID, FileDialog::CallBack* pCallBackFn, const char* defaultFileName = nullptr)
    {
        FileDialog* pFD = FileDialog::getAttached(pFrame, (td::UINT4) wndID);
        if (pFD)
        {
            pFD->setFocus();
            return pFD;
        }
        
        pFD = new SaveFileDialog(pFrame, dlgTitle, allowedDocDescriptions, defaultFileName);
        pFD->_openWithID(wndID, pCallBackFn);
        return pFD;
    }
    
//    int getSelectedDocumentDescriptionIndex() const;
};

class NATGUI_API SelectFolderDialog : public FileDialog
{
protected:
    SelectFolderDialog(Frame* pFrame, const td::String& dlgTitle, const td::String& strSelectButtonLbl="", const char* defaultFileOrFolderName = nullptr);
public:
    
    template <typename DLGID>
    static FileDialog* show(Frame* pFrame, const td::String& dlgTitle, DLGID wndID, gui::IMessageConsumer* pConsumer = nullptr, const td::String& strSelectButtonLbl="", const char* defaultFileOrFolderName = nullptr)
    {
        FileDialog* pFD = FileDialog::getAttached(pFrame, (td::UINT4) wndID);
        if (pFD)
        {
            pFD->setFocus();
            return pFD;
        }
        
        pFD = new SelectFolderDialog(pFrame, dlgTitle, strSelectButtonLbl, defaultFileOrFolderName);
        if (!pConsumer)
            pConsumer = pFrame->getMessageConsumer();
        pFD->_openWithID(wndID, pConsumer);
        return pFD;
    }
    
    template <typename DLGID>
    static FileDialog* show(Frame* pFrame, const td::String& dlgTitle, DLGID wndID, std::function<void(gui::FileDialog*)> fnToCallOnOK, const td::String& strSelectButtonLbl="", const char* defaultFileOrFolderName = nullptr)
    {
        FileDialog* pFD = FileDialog::getAttached(pFrame, (td::UINT4) wndID);
        if (pFD)
        {
            pFD->setFocus();
            return pFD;
        }
        
        pFD = new SelectFolderDialog(pFrame, dlgTitle, strSelectButtonLbl, defaultFileOrFolderName);
        pFD->_openWithID(wndID, fnToCallOnOK);
        return pFD;
    }
    
    template <typename DLGID>
    static FileDialog* show(Frame* pFrame, const td::String& dlgTitle, DLGID wndID, FileDialog::CallBack* pCallBackFn, const td::String& strSelectButtonLbl="", const char* defaultFileOrFolderName = nullptr)
    {
        FileDialog* pFD = FileDialog::getAttached(pFrame, (td::UINT4) wndID);
        if (pFD)
        {
            pFD->setFocus();
            return pFD;
        }
        
        pFD = new SelectFolderDialog(pFrame, dlgTitle, strSelectButtonLbl, defaultFileOrFolderName);
        pFD->_openWithID(wndID, pCallBackFn);
        return pFD;
    }
    
};

} //namespace gui
