#pragma once
#include <gui/DrawableButtonWithDelegate.h>

class Button : public gui::DrawableButtonWithDelegate
{
protected:
public:
    Button(gui::IDrawDelegate* pDrawDelegate, td::UINT4 tagID)
    : gui::DrawableButtonWithDelegate(pDrawDelegate, tagID)
    {
        switch (tagID)
        {
            case 10: setToolTip(tr("Nomal")); break;
            case 20: setToolTip(tr("Bold")); break;
            case 30: setToolTip(tr("Italic")); break;
            case 40: setToolTip(tr("BoldItalic")); break;
            case 50: setToolTip(tr("Underline")); break;
            case 51: setToolTip(tr("ResetUnderline")); break;
            case 60: setToolTip(tr("Subscript")); break;
            case 70: setToolTip(tr("Superscript")); break;
            case 71: setToolTip(tr("ResetSupSuperscript")); break;
            case 80: setToolTip(tr("ResetColor")); break;
                
            case 100: setToolTip(tr("IncreaseFont")); break;
            case 110: setToolTip(tr("DecreaseFont")); break;
        }
    }
};
