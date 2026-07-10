// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Dialog.h
    @brief Modal and non-modal dialog window with configurable buttons. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Window.h>
#include <gui/_aux/DialogView.h>
#include <gui/Button.h>
#include <gui/Application.h>

namespace gui
{

class WindowHelper;

/// @brief A window subclass providing standard dialog behaviour with a button row.
class NATGUI_API Dialog : public Window
{
    friend class DialogView;
    friend class WindowHelper;

    /// @brief Internal classification of the event handler attached to the dialog.
    enum class EventHandlerType : td::BYTE { Consumer=0, FunctionForSingleButtonWithoutBtnID, FunctionForAllButtons, FunctionForSingleButton, NA};
public:

    /// @brief Represents a single push-button inside the dialog.
    class Button
    {
    public:
        /// @brief Standard identifiers for dialog buttons.
        enum class ID : td::BYTE {
            Cancel=0, ///< Cancel button.
            OK,       ///< OK / confirm button.
            Apply,    ///< Apply button.
            Delete,   ///< Delete button.
            Close,    ///< Close button.
            User0,    ///< User-defined button 0.
            User1,    ///< User-defined button 1.
            User2,    ///< User-defined button 2.
            User3,    ///< User-defined button 3.
            User4,    ///< User-defined button 4.
            User5,    ///< User-defined button 5.
            User6,    ///< User-defined button 6.
            User7,    ///< User-defined button 7.
            User8,    ///< User-defined button 8.
            User9,    ///< User-defined button 9.
            NA        ///< Not assigned / sentinel value.
        };
        gui::Button button; ///< The underlying push-button widget.
        Dialog::Button::ID btnID; ///< Logical identifier assigned to this button.

        /// @brief Default constructor. Creates a button with no label and ID::NA.
        Button()
        : button("")
        , btnID(Button::ID::NA)
        {}
    };

    /// @brief Descriptor used to configure a dialog button before the dialog is created.
    class ButtonDesc
    {
    public:
        td::String title;                                         ///< Display label for the button.
        Dialog::Button::ID btnID;                                 ///< Logical identifier for the button.
        gui::Button::Type btnType = gui::Button::Type::Normal;    ///< Visual type (Normal, Default, etc.).

        /// @brief Default constructor. Produces a descriptor with no title and ID::NA.
        ButtonDesc()
        : btnID(Button::ID::NA)
        {
        }

        /// @brief Constructs a button descriptor with explicit attributes.
        /// @param buttonID Logical button identifier.
        /// @param buttonTitle Display label.
        /// @param buttonType Visual button type.
        ButtonDesc(Button::ID buttonID, const td::String& buttonTitle, gui::Button::Type buttonType = gui::Button::Type::Normal)
        : title(buttonTitle)
        , btnID(buttonID)
        , btnType(buttonType)
        {
        }
    };

    /// @brief Interface used by panels to veto dialog closure on a per-button basis.
    //used with Pangel to provide additional checks if button closes the dialog in the Panel's view
    class CloseChecker
    {
    public:
        /// @brief Called before the dialog closes to allow the consumer to veto the close.
        /// @param btnID Identifier of the button that was clicked.
        /// @param pButton Pointer to the button widget.
        /// @return True if the dialog should close; false to cancel closure.
        virtual bool shouldTriggerClose(Dialog::Button::ID, gui::Button*) = 0;
    };

    static const std::initializer_list<Dialog::ButtonDesc> OkCancel; ///< Convenience initialiser list for an OK/Cancel button row.
protected:
    CloseChecker* _pCloseChecker = nullptr; ///< Optional close-veto checker; may be nullptr.
    DialogView _mainView;                   ///< Internal composite view that holds the central content and the button row.
    VerticalLayout _vl;                     ///< Vertical layout stacking content above the button row.
    HorizontalLayout _hlButtons;            ///< Horizontal layout holding the dialog buttons.
    cnt::SafeFullVector<Dialog::Button> _buttons; ///< Allocated button widgets.

    std::function<void(gui::Dialog::Button::ID, gui::Dialog*)> _fnToCallForAllButtons; ///< Callback receiving button ID and dialog pointer; fired for any button.
    std::function<void(gui::Dialog*)> _fnToCallForSingleButtons; ///< Callback receiving only the dialog pointer; fired for a specific button.

    Dialog::Button::ID _clickedButtonID = Dialog::Button::ID::NA; ///< ID of the button that last closed the dialog.
    EventHandlerType _eventHandlerType = EventHandlerType::Consumer; ///< Currently active event-handler strategy.
    Dialog::Button::ID _buttonIDToHandle = Dialog::Button::ID::OK; ///< Button whose click is forwarded to the single-button handler.
    td::BYTE _modal = 0;   ///< Non-zero when the dialog is open in modal mode.
    td::BYTE _closing = 0; ///< Non-zero while the dialog is in the process of closing.
private:
    //this is for internal use
    bool shouldClose() override final;
    void onClose() override final;
    void onLostFocus() override final;

    /// @brief Internal handler called when any button is clicked.
    /// @param pButton Pointer to the button that was clicked.
    /// @return True if the click was handled and the dialog should close.
    bool handleClick(gui::Button* pButton);

    /// @brief Appends a button to the dialog if it is present in the descriptor list.
    /// @param btnID Button identifier to append.
    /// @param buttons Full list of button descriptors.
    /// @return Index of the appended button, or npos if not found.
    size_t appendButton(Dialog::Button::ID btnID, const std::initializer_list<Dialog::ButtonDesc>& buttons);

    /// @brief Creates and lays out all buttons described in the list.
    /// @param buttons List of button descriptors.
    /// @param hAlign Horizontal alignment of the button row.
    void initButtons(const std::initializer_list<Dialog::ButtonDesc>& buttons, td::HAlignment hAlign = td::HAlignment::Right);

    /// @brief Composes the complete dialog content hierarchy.
    void composeContent();

    /// @brief Sets the visual (window) identifier without triggering a duplicate-check error.
    /// @param wndID New visual identifier.
    void _setVisualID(td::UINT4 wndID);
protected:
    /// @brief Called when any dialog button is clicked; override to handle button presses.
    /// @param btnID Identifier of the clicked button.
    /// @param pButton Pointer to the button widget.
    /// @return True if handled; false to perform default close behaviour.
    virtual bool onClick(Dialog::Button::ID btnID, gui::Button* pButton);

    /// @brief Called the first time the dialog becomes visible.
    void onInitialAppearance() override;

    /// @brief Programmatically closes a modal dialog as if the given button was clicked.
    /// @param closingBtnID Button ID that represents the reason for closure.
    void closeModal(Dialog::Button::ID closingBtnID);

    /// @brief Transfers keyboard focus to the specified dialog button.
    /// @param btnID Identifier of the button to focus.
    void focusButton(Dialog::Button::ID btnID);
private:

    Dialog() = delete;
    Dialog(const Dialog&) = delete;
    Dialog& operator = (const Dialog&) = delete;
public:
    /// @brief Constructs a dialog with buttons aligned to the right.
    /// @param pParentFrame Parent frame that owns this dialog.
    /// @param buttons Ordered list of button descriptors.
    /// @param sz Initial dialog size.
    /// @param dlgID Optional visual identifier for the dialog.
    Dialog(Frame* pParentFrame, const std::initializer_list<Dialog::ButtonDesc>& buttons, const gui::Size& sz, td::UINT4 dlgID = 0);

    /// @brief Constructs a dialog with explicit button alignment.
    /// @param pParentFrame Parent frame that owns this dialog.
    /// @param buttons Ordered list of button descriptors.
    /// @param buttonAlignment Horizontal alignment of the button row.
    /// @param sz Initial dialog size.
    /// @param dlgID Optional visual identifier for the dialog.
    Dialog(Frame* pParentFrame, const std::initializer_list<Dialog::ButtonDesc>& buttons, td::HAlignment buttonAlignment, const gui::Size& sz, td::UINT4 dlgID = 0);
    // virtual ~Dialog();

    /// @brief Returns the composite view that contains the central content and button row.
    /// @return Const reference to the dialog view.
    const gui::DialogView& getCentralViewWithButtons() const;

    /// @brief Returns a const pointer to the central content control.
    /// @return Const pointer to the control, or nullptr if none is set.
    const Control* getCentralControl() const;

    /// @brief Returns a typed pointer to the dialog's central view.
    /// @tparam TVIEW Type of the expected central view.
    /// @return Pointer to the view cast to TVIEW, or nullptr if not set.
    template <class TVIEW>
    TVIEW* getView() const
    {
        const Control* pConstCtrl = getCentralControl();

        if (!pConstCtrl)
            return nullptr;
        Control* pCtrl = const_cast<Control*>(pConstCtrl);

        return reinterpret_cast<TVIEW*>(pCtrl);
    }

    //set visual ID (wndID) if it has not been already set in constructor, otherwise if will complain
    /// @brief Sets the visual identifier if it was not provided at construction time.
    /// @tparam TINT Integral type of the identifier.
    /// @param wndID Numeric visual identifier to assign.
    template <class TINT>
    void setVisualID(TINT wndID)
    {
        _setVisualID((td::UINT4)wndID);
    }

    //The consumer will be prompted after closing the dialog (with any button)
    /// @brief Opens the dialog in modal mode, notifying a message consumer after closure.
    /// @param pConsumer Consumer to notify when the dialog closes (any button).
    void openModal(gui::IMessageConsumer* pConsumer = nullptr);

    //Function (fnToCal) will be called after closing the dialog (for any button)
    /// @brief Opens the dialog in modal mode, invoking a callback after closure.
    /// @param fnToCall Function called with a pointer to this dialog after any button is clicked.
    void openModal(std::function<void(gui::Dialog*)> fnToCall);

    //Function (fnToCal) will be called after closing the dialog (for any button). Called function has to have two parameters
    /// @brief Opens the dialog in modal mode with a two-parameter callback.
    /// @param fnToCall Function called with the button ID and dialog pointer after any button is clicked.
    void openModal(std::function<void(gui::Dialog::Button::ID, gui::Dialog*)> fnToCall);

    //Function (fnToCal) will be called after closing if the dialog was closed with the specified button ID
    /// @brief Opens the dialog in modal mode, invoking a callback only when a specific button closes it.
    /// @param btnID Button identifier that triggers the callback.
    /// @param fnToCall Function called when the dialog is closed via the specified button.
    void openModal(gui::Dialog::Button::ID, std::function<void(gui::Dialog*)> fnToCall);

    /// @brief Opens the dialog as non-modal, invoking a callback when it is closed.
    /// @param fnToCall Function called with a pointer to this dialog on closure.
    void openNonModal(std::function<void(gui::Dialog*)> fnToCall);

    /// @brief Opens the dialog as non-modal without any closure callback.
    void openNonModal();

    //get butto id that closed the dialog
    /// @brief Returns the identifier of the button that last closed the dialog.
    /// @return Button ID, or Button::ID::NA if the dialog has not been closed.
    Dialog::Button::ID getClickedButtonID() const;

    /// @brief Static helper that allocates, configures, and opens a dialog of the given type.
    /// @tparam Dlg Dialog subclass to instantiate.
    /// @param parent Parent frame.
    /// @param resizible True to make the dialog resizable.
    /// @param keepOnParent True to keep the dialog on top of the parent window.
    template <class Dlg>
    static void show(gui::Frame* parent, bool resizible=true, bool keepOnParent = true)
    {
        auto pDlg = new Dlg(parent);
        if (!resizible)
            pDlg->setResizable(false);
        if (keepOnParent)
            pDlg->keepOnTopOfParent();
        pDlg->open();
    }

    /// @brief Static helper that opens or activates a dialog identified by dlgID.
    /// @tparam Dlg Dialog subclass to instantiate.
    /// @tparam DLGID Integral type of the dialog identifier.
    /// @param parent Parent frame.
    /// @param dlgID Visual identifier; reuses an existing dialog if one is already open.
    /// @param resizible True to make the dialog resizable.
    /// @param keepOnParent True to keep the dialog on top of the parent window.
    template <class Dlg, typename DLGID>
    static void show(gui::Frame* parent, DLGID dlgID, bool resizible=true, bool keepOnParent = true)
    {
        auto pApp = gui::getApplication();

        gui::Window* pWnd = parent->getParentWindow();
        if (!pWnd)
            pWnd = pApp->getInitialWindow();

        assert(pWnd);
        auto dlg = pWnd->getAttachedWindow(dlgID);
        if (dlg)
        {
            dlg->setFocus();
            return;
        }

        auto pDlg = new Dlg(parent, (td::UINT4) dlgID);
        if (!resizible)
            pDlg->setResizable(false);
        if (keepOnParent)
            pDlg->keepOnTopOfParent();
        pDlg->open();
    }

    /// @brief Static helper that opens a titled dialog identified by dlgID.
    /// @tparam Dlg Dialog subclass to instantiate.
    /// @tparam DLGID Integral type of the dialog identifier.
    /// @param parent Parent frame.
    /// @param title Title string to display in the dialog title bar.
    /// @param dlgID Visual identifier; reuses an existing dialog if one is already open.
    /// @param resizible True to make the dialog resizable.
    /// @param keepOnParent True to keep the dialog on top of the parent window.
    template <class Dlg, typename DLGID>
    static void show(gui::Frame* parent, const td::String& title, DLGID dlgID,  bool resizible=true, bool keepOnParent = true)
    {
        auto pApp = gui::getApplication();

        gui::Window* pWnd = parent->getParentWindow();
        if (!pWnd)
            pWnd = pApp->getInitialWindow();

        assert(pWnd);
        auto dlg = pWnd->getAttachedWindow(dlgID);
        if (dlg)
        {
            dlg->setFocus();
            return;
        }

        auto pDlg = new Dlg(parent, (td::UINT4) dlgID);
        pDlg->setTitle(title);
        if (!resizible)
            pDlg->setResizable(false);
        if (keepOnParent)
            pDlg->keepOnTopOfParent();
        pDlg->open();
    }

    /// @brief Attaches a close-checker that can veto button-triggered closure.
    /// @param pCloseChecker Pointer to the CloseChecker implementation.
    void setButtonCloseChecker(CloseChecker* pCloseChecker)
    {
        _pCloseChecker = pCloseChecker;
    }
};

} //namespace gui
