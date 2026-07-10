// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file FileDialog.h
    @brief File and folder picker dialogs (open, save, select folder). */
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

/// @brief Base class for all native file and folder picker dialogs.
class NATGUI_API FileDialog : public Consumer
{
    friend class FileDialogHelper;

public:
    /// @brief Associates a human-readable description with a file-type pattern.
    typedef struct _patern
    {
        td::String docDescription; ///< Human-readable document type description (e.g. "Text files").
        const char* pattern;       ///< File pattern string (e.g. "*.txt", "*.bin").
    } Pattern;

    /// @brief Result of a dialog session.
    enum class Status : td::BYTE {
        Cancel=0, ///< User dismissed the dialog without confirming.
        OK        ///< User confirmed a selection.
    };

    /// @brief Describes the purpose of the dialog.
    enum class Type : td::BYTE {
        OpenFile,    ///< Open an existing file.
        SaveFile,    ///< Save to a file (may create a new one).
        OpenFolder,  ///< Open an existing folder.
        CreateFolder ///< Create a new folder.
    };

    /// @brief Internal classification of the completion handler strategy.
    enum class EventHandlerType : td::BYTE { Consumer = 0, FunctionPtr, FunctionForOK };

    typedef std::function<void(FileDialog*)> CallBack; ///< Callback type invoked on dialog completion.
protected:
    Frame* _pFrame; ///< Parent frame that owns the dialog.
    FileDialog::CallBack* _pCallBackFn = nullptr; ///< Pointer to an external callback function object.
    FileDialog::CallBack _callBack; ///< Owned callback function.
    td::String _fileOrFolderName;    ///< Selected file or folder path.
    td::String _defaultExtension;    ///< Default file extension presented to the user.
    cnt::SafeFullVector<td::String> _filters; ///< File-type filter patterns or extensions.
    td::Variant _attachedValue;      ///< Arbitrary value attached to the dialog for caller use.
    td::UINT4 _wndID;                ///< Visual identifier assigned to this dialog instance.
    td::INT2 _selectedFilterIndex = 0; ///< Index of the currently selected filter in the list.

    EventHandlerType _eventHandlerType; ///< Strategy used to notify the caller after closure.
    Type _type;                         ///< Dialog purpose (open, save, folder, etc.).
    Status _status;                     ///< Last result status of the dialog.
    td::BYTE _isOpen = 0;               ///< Non-zero while the dialog is currently displayed.
private:
    /// @brief Internal: processes the result and notifies the caller.
    void handleDlg();
    /// @brief Internal: releases native C++ resources.
    void destroyCPP();
protected:
    /// @brief Releases all resources held by the dialog.
    void destroy() override;

    /// @brief Returns a pointer to the registered callback function.
    /// @return Pointer to the callback, or nullptr if none is set.
    FileDialog::CallBack* getCallBackFn();

    /// @brief Returns the active event-handler classification.
    /// @return Event handler type enum value.
    gui::FileDialog::EventHandlerType getEventHandlerType() const;

    /// @brief Returns the default file extension string.
    /// @return Const reference to the default extension.
    const td::String& getDefaultExtension() const;

    /// @brief Returns the filter string at the given position.
    /// @param pos Zero-based index into the filter list.
    /// @return Const reference to the filter string.
    const td::String& getFilter(int pos) const;

    //save with different extensions
    /// @brief Constructs a save dialog that supports multiple document types.
    /// @param pFrame Owner frame.
    /// @param dlgTitle Title shown in the dialog title bar.
    /// @param allowedDocDescriptions List of document-type pattern descriptors.
    /// @param defaultFileName Optional pre-populated file name.
    FileDialog(Frame* pFrame, const td::String& dlgTitle, const std::initializer_list<gui::FileDialog::Pattern>& allowedDocDescriptions, const char* defaultFileName = nullptr);

    //for all (when Type==OpenFile only one extension) - save also
    /// @brief General-purpose constructor for open, save, and folder dialogs.
    /// @param pFrame Owner frame.
    /// @param type Dialog purpose.
    /// @param dlgTitle Title shown in the dialog title bar.
    /// @param defaultExtension Default file extension (may be nullptr).
    /// @param strAcceptButtonLbl Label for the accept button (empty string uses system default).
    /// @param defaultFileOrFolderName Optional pre-populated path.
    FileDialog(Frame* pFrame, Type type, const td::String& dlgTitle, const char* defaultExtension = nullptr, const td::String& strAcceptButtonLbl="", const char* defaultFileOrFolderName = nullptr);

    //for Type==OpenFilewith different extension types (*.
    /// @brief Constructs an open-file dialog that accepts multiple extension patterns.
    /// @param allowedTypes List of document-type pattern descriptors.
    /// @param pFrame Owner frame.
    /// @param dlgTitle Title shown in the dialog title bar.
    /// @param strAcceptButtonLbl Label for the accept button (empty string uses system default).
    FileDialog(const std::initializer_list<gui::FileDialog::Pattern>& allowedTypes, Frame* pFrame, const td::String& dlgTitle,  const td::String& strAcceptButtonLbl="");

    /// @brief Opens the dialog and notifies via an external callback pointer.
    /// @param pCallBackFn Pointer to the callback to invoke on completion.
    void open(FileDialog::CallBack* pCallBackFn);

    /// @brief Opens the dialog in modal mode with a callback lambda.
    /// @param fnToCallOnOK Function called only when the user confirms (OK).
    void open(std::function<void(gui::FileDialog*)> fnToCallOnOK); //modal mode with callback function

    FileDialog(const FileDialog&) = delete;
    FileDialog& operator =(const FileDialog&) = delete;
public:
    /// @brief Destructor. Closes the dialog if still open and releases resources.
    ~FileDialog();

    /// @brief Returns the GUI object type identifier.
    /// @return ObjType::FileDialog.
    gui::ObjType getObjType() const override { return ObjType::FileDialog;}

    /// @brief Moves keyboard focus to the dialog window.
    void setFocus();

    /// @brief Closes and destroys the dialog (for asynchronous/non-modal mode).
    void close(); //for async mode

    /// @brief Returns the full path of the selected file or folder.
    /// @return Path string (empty if the dialog was cancelled).
    td::String getFileName() const;

    /// @brief Returns the visual (window) identifier of the dialog.
    /// @return Visual identifier.
    td::UINT4 getVisualID() const override;

    /// @brief Returns the last result status.
    /// @return Status::OK if the user confirmed, Status::Cancel otherwise.
    Status getStatus() const;

    /// @brief Returns the dialog purpose type.
    /// @return Type enum value.
    Type getType() const;

    /// @brief Returns a const pointer to the parent frame.
    /// @return Const pointer to the frame.
    const Frame* getFrame() const;

    /// @brief Returns the file extension of the selected file.
    /// @return C-string extension (without the leading dot), or nullptr.
    const char* getFileExtension() const;

    /// @brief Attaches an arbitrary value to this dialog for retrieval by the caller.
    /// @tparam TVAL Type of the value to attach.
    /// @param val Value to store.
    template <typename TVAL>
    void attachValue(const TVAL& val)
    {
        _attachedValue = val;
    }

    /// @brief Retrieves the previously attached value.
    /// @tparam TVAL Expected type of the attached value.
    /// @param val Reference filled with the stored value.
    template <typename TVAL>
    void getAttachedValue(TVAL& val)
    {
        _attachedValue.getValue(val);
    }

    /// @brief Internal: opens the dialog with a given window ID and message consumer.
    /// @param wndID Visual identifier to assign.
    /// @param pConsumer Consumer to notify on closure.
    void _openWithID(td::UINT4 wndID, gui::IMessageConsumer* pConsumer);

    /// @brief Internal: opens the dialog with a given window ID and callback for OK.
    /// @param wndID Visual identifier to assign.
    /// @param fnToCallOnOK Callback invoked when the user confirms.
    void _openWithID(td::UINT4 wndID, std::function<void(gui::FileDialog*)> fnToCallOnOK);

    /// @brief Internal: opens the dialog with a given window ID and external callback pointer.
    /// @param wndID Visual identifier to assign.
    /// @param pCallBackFn Pointer to the callback to invoke on completion.
    void _openWithID(td::UINT4 wndID, FileDialog::CallBack* pCallBackFn);

    /// @brief Returns a pointer to an already-open file dialog with the given ID attached to a frame.
    /// @param pFrame Frame to search for the attached dialog.
    /// @param wndID Visual identifier to look up.
    /// @return Pointer to the dialog, or nullptr if not found.
    static FileDialog* getAttached(gui::Frame* pFrame, td::UINT4 wndID);

    /// @brief Returns whether the dialog is currently displayed.
    /// @return True if the dialog is open.
    bool isOpen() const;

};

/// @brief Specialisation of FileDialog for opening existing files.
class NATGUI_API OpenFileDialog : public FileDialog
{
protected:
    /// @brief Constructs an open-file dialog for a single extension type.
    /// @param pFrame Owner frame.
    /// @param dlgTitle Title bar text.
    /// @param defaultExtension File extension to filter (e.g. "txt").
    /// @param strOpenButtonLbl Label for the open button.
    OpenFileDialog(Frame* pFrame, const td::String& dlgTitle, const char* defaultExtension, const td::String& strOpenButtonLbl="");

    /// @brief Constructs an open-file dialog for multiple extension types.
    /// @param pFrame Owner frame.
    /// @param dlgTitle Title bar text.
    /// @param allowedTypes List of permitted document patterns.
    /// @param strOpenButtonLbl Label for the open button.
    OpenFileDialog(Frame* pFrame, const td::String& dlgTitle, const std::initializer_list<gui::FileDialog::Pattern>& allowedTypes, const td::String& strOpenButtonLbl="");

public:
    /// @brief Shows (or focuses) an open-file dialog for a single extension with a consumer.
    /// @tparam DLGID Integral identifier type.
    /// @param pFrame Owner frame.
    /// @param dlgTitle Title bar text.
    /// @param defaultExtension Extension filter.
    /// @param wndID Visual identifier (reuses existing dialog if already open).
    /// @param pConsumer Consumer notified on closure.
    /// @param strOpenButtonLbl Label for the open button.
    /// @return Pointer to the dialog.
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

    /// @brief Shows (or focuses) an open-file dialog for a single extension with a lambda callback.
    /// @tparam DLGID Integral identifier type.
    /// @param pFrame Owner frame.
    /// @param dlgTitle Title bar text.
    /// @param defaultExtension Extension filter.
    /// @param wndID Visual identifier.
    /// @param fnToCallOnOK Callback invoked when the user confirms.
    /// @param strOpenButtonLbl Label for the open button.
    /// @return Pointer to the dialog.
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

    /// @brief Shows (or focuses) an open-file dialog with a callback pointer.
    /// @tparam DLGID Integral identifier type.
    /// @param pFrame Owner frame.
    /// @param dlgTitle Title bar text.
    /// @param defaultExtension Extension filter.
    /// @param wndID Visual identifier.
    /// @param pCallBackFn Pointer to the callback function.
    /// @param strOpenButtonLbl Label for the open button.
    /// @return Pointer to the dialog.
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

    /// @brief Shows (or focuses) an open-file dialog for multiple types with a consumer.
    /// @tparam DLGID Integral identifier type.
    /// @param pFrame Owner frame.
    /// @param dlgTitle Title bar text.
    /// @param allowedTypes List of document-type patterns.
    /// @param wndID Visual identifier.
    /// @param pConsumer Consumer notified on closure.
    /// @param strOpenButtonLbl Label for the open button.
    /// @return Pointer to the dialog.
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

    /// @brief Shows (or focuses) an open-file dialog for multiple types with a lambda.
    /// @tparam DLGID Integral identifier type.
    /// @param pFrame Owner frame.
    /// @param dlgTitle Title bar text.
    /// @param allowedTypes List of document-type patterns.
    /// @param wndID Visual identifier.
    /// @param fnToCallOnOK Callback invoked when the user confirms.
    /// @param strOpenButtonLbl Label for the open button.
    /// @return Pointer to the dialog.
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

    /// @brief Shows (or focuses) an open-file dialog for multiple types with a callback pointer.
    /// @tparam DLGID Integral identifier type.
    /// @param pFrame Owner frame.
    /// @param dlgTitle Title bar text.
    /// @param allowedTypes List of document-type patterns.
    /// @param wndID Visual identifier.
    /// @param pCallBackFn Pointer to the callback function.
    /// @param strOpenButtonLbl Label for the open button.
    /// @return Pointer to the dialog.
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

/// @brief Specialisation of FileDialog for saving files.
class NATGUI_API SaveFileDialog : public FileDialog
{
protected:
    /// @brief Constructs a save-file dialog for a single extension.
    /// @param pFrame Owner frame.
    /// @param dlgTitle Title bar text.
    /// @param defaultExtension Default extension (may be nullptr).
    /// @param strSaveButtonLbl Label for the save button.
    /// @param defaultFileOrFolderName Optional pre-populated file name.
    SaveFileDialog(Frame* pFrame, const td::String& dlgTitle, const char* defaultExtension = nullptr, const td::String& strSaveButtonLbl="", const char* defaultFileOrFolderName = nullptr);

    /// @brief Constructs a save-file dialog supporting multiple document types.
    /// @param pFrame Owner frame.
    /// @param dlgTitle Title bar text.
    /// @param allowedDocDescriptions List of allowed document-type patterns.
    /// @param defaultFileName Optional pre-populated file name.
    SaveFileDialog(Frame* pFrame, const td::String& dlgTitle, const std::initializer_list<gui::FileDialog::Pattern>& allowedDocDescriptions, const char* defaultFileName = nullptr);

public:
    /// @brief Shows (or focuses) a save-file dialog with a consumer.
    /// @tparam DLGID Integral identifier type.
    /// @param pFrame Owner frame.
    /// @param dlgTitle Title bar text.
    /// @param defaultExtension Default extension filter.
    /// @param wndID Visual identifier.
    /// @param pConsumer Consumer notified on closure.
    /// @param strSaveButtonLbl Label for the save button.
    /// @param defaultFileOrFolderName Optional pre-populated file name.
    /// @return Pointer to the dialog.
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

    /// @brief Shows (or focuses) a save-file dialog with a lambda callback.
    /// @tparam DLGID Integral identifier type.
    /// @param pFrame Owner frame.
    /// @param dlgTitle Title bar text.
    /// @param defaultExtension Default extension filter.
    /// @param wndID Visual identifier.
    /// @param fnToCallOnOK Callback invoked when the user confirms.
    /// @param strSaveButtonLbl Label for the save button.
    /// @param defaultFileOrFolderName Optional pre-populated file name.
    /// @return Pointer to the dialog.
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

    /// @brief Shows (or focuses) a save-file dialog with an attached value and lambda.
    /// @tparam DLGID Integral identifier type.
    /// @tparam TATTACH Type of the value to attach.
    /// @param pFrame Owner frame.
    /// @param dlgTitle Title bar text.
    /// @param defaultExtension Default extension filter.
    /// @param attachValue Value to attach for later retrieval by the caller.
    /// @param wndID Visual identifier.
    /// @param fnToCallOnOK Callback invoked when the user confirms.
    /// @param strSaveButtonLbl Label for the save button.
    /// @param defaultFileOrFolderName Optional pre-populated file name.
    /// @return Pointer to the dialog.
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

    /// @brief Shows (or focuses) a save-file dialog with a callback pointer.
    /// @tparam DLGID Integral identifier type.
    /// @param pFrame Owner frame.
    /// @param dlgTitle Title bar text.
    /// @param defaultExtension Default extension filter.
    /// @param wndID Visual identifier.
    /// @param pCallBackFn Pointer to the callback function.
    /// @param strSaveButtonLbl Label for the save button.
    /// @param defaultFileOrFolderName Optional pre-populated file name.
    /// @return Pointer to the dialog.
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

    /// @brief Shows (or focuses) a save-file dialog without a mandatory extension, using a consumer.
    /// @tparam DLGID Integral identifier type.
    /// @param pFrame Owner frame.
    /// @param dlgTitle Title bar text.
    /// @param wndID Visual identifier.
    /// @param pConsumer Consumer notified on closure.
    /// @param defaultExtension Optional extension filter.
    /// @param strSaveButtonLbl Label for the save button.
    /// @param defaultFileOrFolderName Optional pre-populated file name.
    /// @return Pointer to the dialog.
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

    /// @brief Shows (or focuses) a save-file dialog with a lambda and optional extension.
    /// @tparam DLGID Integral identifier type.
    /// @param pFrame Owner frame.
    /// @param dlgTitle Title bar text.
    /// @param wndID Visual identifier.
    /// @param fnToCallOnOK Callback invoked when the user confirms.
    /// @param defaultExtension Optional extension filter.
    /// @param strSaveButtonLbl Label for the save button.
    /// @param defaultFileOrFolderName Optional pre-populated file name.
    /// @return Pointer to the dialog.
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

    /// @brief Shows (or focuses) a save-file dialog with a callback pointer and optional extension.
    /// @tparam DLGID Integral identifier type.
    /// @param pFrame Owner frame.
    /// @param dlgTitle Title bar text.
    /// @param wndID Visual identifier.
    /// @param pCallBackFn Pointer to the callback function.
    /// @param defaultExtension Optional extension filter.
    /// @param strSaveButtonLbl Label for the save button.
    /// @param defaultFileOrFolderName Optional pre-populated file name.
    /// @return Pointer to the dialog.
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

    /// @brief Shows (or focuses) a save-file dialog for multiple document types with a consumer.
    /// @tparam DLGID Integral identifier type.
    /// @param pFrame Owner frame.
    /// @param dlgTitle Title bar text.
    /// @param allowedDocDescriptions List of document-type patterns.
    /// @param wndID Visual identifier.
    /// @param pConsumer Consumer notified on closure.
    /// @param defaultFileName Optional pre-populated file name.
    /// @return Pointer to the dialog.
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

    /// @brief Shows (or focuses) a save-file dialog for multiple document types with a lambda.
    /// @tparam DLGID Integral identifier type.
    /// @param pFrame Owner frame.
    /// @param dlgTitle Title bar text.
    /// @param allowedDocDescriptions List of document-type patterns.
    /// @param wndID Visual identifier.
    /// @param fnToCallOnOK Callback invoked when the user confirms.
    /// @param defaultFileName Optional pre-populated file name.
    /// @return Pointer to the dialog.
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

    /// @brief Shows (or focuses) a save-file dialog for multiple document types with a callback pointer.
    /// @tparam DLGID Integral identifier type.
    /// @param pFrame Owner frame.
    /// @param dlgTitle Title bar text.
    /// @param allowedDocDescriptions List of document-type patterns.
    /// @param wndID Visual identifier.
    /// @param pCallBackFn Pointer to the callback function.
    /// @param defaultFileName Optional pre-populated file name.
    /// @return Pointer to the dialog.
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

/// @brief Specialisation of FileDialog for selecting an existing folder.
class NATGUI_API SelectFolderDialog : public FileDialog
{
protected:
    /// @brief Constructs a folder-selection dialog.
    /// @param pFrame Owner frame.
    /// @param dlgTitle Title bar text.
    /// @param strSelectButtonLbl Label for the select button.
    /// @param defaultFileOrFolderName Optional pre-populated folder path.
    SelectFolderDialog(Frame* pFrame, const td::String& dlgTitle, const td::String& strSelectButtonLbl="", const char* defaultFileOrFolderName = nullptr);
public:

    /// @brief Shows (or focuses) a folder-selection dialog with a consumer.
    /// @tparam DLGID Integral identifier type.
    /// @param pFrame Owner frame.
    /// @param dlgTitle Title bar text.
    /// @param wndID Visual identifier.
    /// @param pConsumer Consumer notified on closure.
    /// @param strSelectButtonLbl Label for the select button.
    /// @param defaultFileOrFolderName Optional pre-populated folder path.
    /// @return Pointer to the dialog.
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

    /// @brief Shows (or focuses) a folder-selection dialog with a lambda callback.
    /// @tparam DLGID Integral identifier type.
    /// @param pFrame Owner frame.
    /// @param dlgTitle Title bar text.
    /// @param wndID Visual identifier.
    /// @param fnToCallOnOK Callback invoked when the user confirms.
    /// @param strSelectButtonLbl Label for the select button.
    /// @param defaultFileOrFolderName Optional pre-populated folder path.
    /// @return Pointer to the dialog.
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

    /// @brief Shows (or focuses) a folder-selection dialog with a callback pointer.
    /// @tparam DLGID Integral identifier type.
    /// @param pFrame Owner frame.
    /// @param dlgTitle Title bar text.
    /// @param wndID Visual identifier.
    /// @param pCallBackFn Pointer to the callback function.
    /// @param strSelectButtonLbl Label for the select button.
    /// @param defaultFileOrFolderName Optional pre-populated folder path.
    /// @return Pointer to the dialog.
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
