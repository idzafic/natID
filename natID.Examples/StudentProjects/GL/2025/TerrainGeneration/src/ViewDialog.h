#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/LineEdit.h>
#include <gui/GridLayout.h>
#include <gui/Button.h>
#include <gui/Alert.h>

// View for frequency input with label and text edit field
class ViewDialog : public gui::View
{
private:
protected:
    gui::Label _lblFR;
    gui::LineEdit _editFR;
    gui::GridLayout _gl;

public:
    ViewDialog()
        : _lblFR(tr("Enter frequency:"))
        , _gl(1, 2)
    {
        // Place label and line edit into a grid layout
        _gl.insert(0, 0, _lblFR);
        _gl.insert(0, 1, _editFR);

        setLayout(&_gl);

        // Set focus to the edit field when the dialog opens
        setFocus(&_editFR);
    }

    // Returns the text from the frequency edit field
    td::String getFrequency() const
    {
        return _editFR.getText();
    }
};