#pragma once
#include <gui/Dialog.h>
#include "ViewDialog.h"


class DialogFrequency : public gui::Dialog
{
protected:
    ViewDialog _viewDialog;

    // Handles button clicks, validates frequency input on OK press
    bool onClick(Dialog::Button::ID btnID, gui::Button* pButton) override
    {
        if (btnID == Dialog::Button::ID::OK)
        {
            td::String freqStr = _viewDialog.getFrequency();
            if (freqStr.length() == 0) 
            {
                gui::Alert::show(tr("Input Error"), tr("Frequency field cannot be empty."));
                return false; // Prevents the dialog from closing
            }

            double freqVal = freqStr.toDouble();
            if (freqVal <= 0 || freqVal>=25)
            {
                gui::Alert::show(tr("Input Error"), tr("Please enter a valid positive number for frequency."));
                return false; // Prevents the dialog from closing
            }
        }
        return true; // Closes the dialog for OK (if valid) and Cancel
    }

public:
    // Constructor: initializes dialog with OK and Cancel buttons, sets size and central view
    DialogFrequency(gui::Control* pParent, td::UINT4 wndID = 0)
        : gui::Dialog(pParent, { {gui::Dialog::Button::ID::OK, tr("Ok"), gui::Button::Type::Default},
                                {gui::Dialog::Button::ID::Cancel, tr("Cancel")} }, gui::Size(300, 100), wndID)
    {
        setTitle(tr("Set Frequency"));
        setCentralView(&_viewDialog);
    }

    // Returns the frequency string entered in the dialog
    td::String getFrequency() const
    {
        return _viewDialog.getFrequency();
    }
};