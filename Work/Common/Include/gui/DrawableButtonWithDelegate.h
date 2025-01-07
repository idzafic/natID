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
#include <gui/DrawableButton.h>
#include <gui/IDrawDelegate.h>

namespace gui
{
class NATGUI_API DrawableButtonWithDelegate : public DrawableButton
{
public:
    enum class Type : unsigned char {Apply, Restore, DelegateDraw};
private:
    gui::IDrawDelegate* _pDelegate = nullptr;
    //td::UINT4 _tag = 0;
//    Type _type;
    td::BYTE _checked = 0;
    
protected:
//    void getMinSize(gui::Size& sz) const override;
    void measure(CellInfo&) override;
    void onDraw(const gui::Rect& rect) override;
    DrawableButtonWithDelegate();
    //td::UINT4 getTagID() const override;
    td::UINT2 getPreferableWidth(td::UINT2 forHeight, td::UINT2 minWidth) const override;
public:
//    void setTag(td::UINT4 tag);
//    td::UINT4 getTag() const;
    void setChecked(bool checked);
    bool isChecked() const;
    
    DrawableButtonWithDelegate(gui::IDrawDelegate* pDrawDelegatem, td::UINT4 visualID);
    ~DrawableButtonWithDelegate();
//    Type getType() const;
    gui::ObjType getObjType() const override { return ObjType::DrawableButtonWithDelegate;}
    
};

}//namespace gui

