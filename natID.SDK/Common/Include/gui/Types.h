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
#include <gui/natGUI.h>
#include <td/PointAndSize.h>
#include <td/RectNormalized.h>
#include <td/Circle.h>
#include <td/ColorID.h>
#include <td/LinePattern.h>
#include <td/String.h>
#include <td/Range.h>

namespace gui
{

enum class ObjType : unsigned char {AppWindow=0, Window, Dialog, FileDialog, OpenFileDialog, SaveFileDialog, MenuBar, MenuItem, ToolBar, ToolBarView, ToolBarItem, ToolBarButton, StatusBar, ToolBarPopoverItem, Image, View, ViewScroller, ViewSwitcher, StandardTabView, TabView, Navigator, Canvas, GLView, GLXView, Label, LineEdit, PasswordEdit, NumericEdit, DateEdit, TimeEdit, TextEdit, CodeView, ComboBox, DBComboBox, CheckBox, Slider, ProgressIndicator, TableEdit, PopoverCanvas, PopoverButton, ContextMenuNat, ImageView, Button, PushButton, DrawableButton, DrawableButtonIntern, DrawableButtonWithDelegate,
    MenuButton, HorizontalLayout, VerticalLayout, GridLayout, PropertyLayout, SplitterLayout, SplitterCell, Transformation, Timer, NA};

//enum class Artwork : unsigned char {EPS=0, PDF, SVG};

enum class KeyModifier : td::UINT4 {None=0, Shift=1, Caps=2, Ctrl=4, Alt=8, Cmd=16, Fn=32};

inline bool isShiftPressed(td::UINT4 modifiers) {return (modifiers & (td::UINT4)KeyModifier::Shift);}
inline bool isCapsPressed(td::UINT4 modifiers) {return (modifiers & (td::UINT4)KeyModifier::Caps);}
inline bool isCtrlPressed(td::UINT4 modifiers) {return (modifiers & (td::UINT4)KeyModifier::Ctrl);}
inline bool isAltPressed(td::UINT4 modifiers) {return (modifiers & (td::UINT4)KeyModifier::Alt);}
inline bool isCmdPressed(td::UINT4 modifiers) {return (modifiers & (td::UINT4)KeyModifier::Cmd);}
inline bool isFnPressed(td::UINT4 modifiers) {return (modifiers & (td::UINT4)KeyModifier::Fn);}

enum class FileType : unsigned char {File=0, Folder, FileOrFolder};
enum class Orientation : unsigned char {Horizontal=0 , Vertical};
//typedef void* NatShape;



#ifdef MU_MACOS
typedef void* Handle;
typedef double CoordType;
typedef void* NatString;
typedef void* AttributedString;
#elif defined(MU_WINDOWS)
typedef void* Handle;
typedef double CoordType;
typedef void* NatString;
typedef void* AttributedString;
#else
typedef void* Handle;
typedef double CoordType;
typedef td::String* NatString;
typedef td::String* AttributedString;
#endif

using Geometry = td::PointAndSize<gui::CoordType>;
using Size = td::Size<gui::CoordType>;
using Point = td::Point<gui::CoordType>;
using Rect = td::RectNormalized<gui::CoordType>;
using Circle =  td::Circle<gui::CoordType>;
using Range = td::Range<td::UINT4>;

class AttributedStringBlock
{
public:
    gui::AttributedString natString;
    CoordType width;
    CoordType height;
};

class SymbolDrawingAttribs
{
public:
    float lineWidth = 1;
    td::ColorID lineColor = td::ColorID::SysText;
    td::ColorID fillColor = td::ColorID::SysCtrlBack;
    //td::LinePattern linePattern = td::LinePattern::Solid;
    td::BYTE displayGroup : 1;
    td::BYTE overideLineWidth : 1;
    //td::BYTE overideLinePattern : 1;
    td::BYTE overideLineColor : 1;
    td::BYTE overideFillColor : 1;
    SymbolDrawingAttribs()
    : displayGroup(1)
    , overideLineWidth(0)
    //, overideLinePattern(0)
    , overideLineColor(0)
    , overideFillColor(0)
    {
    }
    
    void setLineWidth(float width)
    {
        lineWidth = width;
    }
    
    td::BYTE getLineWithInt() const
    {
        return td::BYTE(lineWidth);
    }
    
    float getLineWithFloat() const
    {
        return lineWidth;
    }
};

NATGUI_API td::String tr(const char* strKey);
NATGUI_API const char* trCStr(const char* strKey);
NATGUI_API td::String getResText(const char* strKey);
NATGUI_API td::String getResFileName(const char* strKey);
NATGUI_API td::UINT4 removeKeyModifier(td::UINT4 modifier, KeyModifier toRemove);

} //namespace gui
