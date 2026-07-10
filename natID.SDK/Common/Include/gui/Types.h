// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Types.h
    @brief Core GUI type aliases, enumerations, and utility functions shared across the gui namespace. */
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

/// @brief Identifies every concrete GUI object type in the framework.
enum class ObjType : unsigned char {AppWindow=0, ///< Application window.
    Window,                  ///< Standard window.
    Dialog,                  ///< Modal or modeless dialog.
    FileDialog,              ///< Generic file dialog.
    OpenFileDialog,          ///< File-open dialog.
    SaveFileDialog,          ///< File-save dialog.
    MenuBar,                 ///< Application menu bar.
    MenuItem,                ///< Single menu item.
    ToolBar,                 ///< Toolbar container.
    ToolBarView,             ///< View that renders toolbar controls.
    ToolBarItem,             ///< Descriptor for a single toolbar item.
    ToolBarButton,           ///< Button control placed in a toolbar.
    StatusBar,               ///< Application status bar.
    ToolBarPopoverItem,      ///< Popover item attached to a toolbar.
    Image,                   ///< Image object.
    View,                    ///< Generic container view.
    ViewScroller,            ///< Scrollable view wrapper.
    ViewSwitcher,            ///< View that shows one child at a time.
    StandardTabView,         ///< Tab view using the platform's standard style.
    TabView,                 ///< Custom tab view.
    Navigator,               ///< Side-navigation control.
    Canvas,                  ///< Custom-drawn canvas view.
    GLView,                  ///< OpenGL rendering view.
    GLXView,                 ///< Extended OpenGL rendering view.
    Label,                   ///< Read-only text label.
    LineEdit,                ///< Single-line text input.
    PasswordEdit,            ///< Password (masked) text input.
    NumericEdit,             ///< Numeric value input.
    Stepper,                 ///< Value increaser/decreaser for NumericEdit
    DateEdit,                ///< Date value input.
    TimeEdit,                ///< Time value input.
    TextEdit,                ///< Multi-line text editor.
    CodeView,                ///< Syntax-highlighting code editor.
    ComboBox,                ///< Drop-down combo box.
    DBComboBox,              ///< Database-backed combo box.
    CheckBox,                ///< Boolean check-box control.
    Slider,                  ///< Slider / track-bar control.
    ProgressIndicator,       ///< Progress bar or spinner.
    TableEdit,               ///< Editable table control.
    PopoverCanvas,           ///< Canvas shown inside a popover.
    PopoverButton,           ///< Button that presents a popover.
    ContextMenuNat,          ///< Native context (right-click) menu.
    ImageView,               ///< View that displays an image.
    Button,                  ///< Generic button.
    PushButton,              ///< Standard push button.
    DrawableButton,          ///< Button with custom drawing.
    DrawableButtonIntern,    ///< Internal drawable button variant.
    DrawableButtonWithDelegate, ///< Drawable button that delegates drawing.
    MenuButton,              ///< Button that opens a drop-down menu.
    HorizontalLayout,        ///< Horizontal stacked layout.
    VerticalLayout,          ///< Vertical stacked layout.
    GridLayout,              ///< Grid-based layout.
    PropertyLayout,          ///< Property-sheet style layout.
    SplitterLayout,          ///< Resizable splitter layout.
    SplitterCell,            ///< Individual cell within a splitter layout.
    Transformation,          ///< 2D affine transformation object.
    Timer,                   ///< Timer event source.
    NA                       ///< Not applicable / unknown object type.
};

//enum class Artwork : unsigned char {EPS=0, PDF, SVG};

/// @brief Bitmask flags for keyboard modifier keys.
enum class KeyModifier : td::UINT4 {None=0,  ///< No modifier.
    Shift=1,  ///< Shift key.
    Caps=2,   ///< Caps Lock key.
    Ctrl=4,   ///< Control key.
    Alt=8,    ///< Alt / Option key.
    Cmd=16,   ///< Command / Windows key.
    Fn=32     ///< Function (Fn) key.
};

/// @brief Returns whether the Shift modifier is active.
/// @param modifiers Combined modifier bitmask.
/// @return True if Shift is pressed.
inline bool isShiftPressed(td::UINT4 modifiers) {return (modifiers & (td::UINT4)KeyModifier::Shift);}
/// @brief Returns whether the Caps Lock modifier is active.
/// @param modifiers Combined modifier bitmask.
/// @return True if Caps Lock is active.
inline bool isCapsPressed(td::UINT4 modifiers) {return (modifiers & (td::UINT4)KeyModifier::Caps);}
/// @brief Returns whether the Ctrl modifier is active.
/// @param modifiers Combined modifier bitmask.
/// @return True if Ctrl is pressed.
inline bool isCtrlPressed(td::UINT4 modifiers) {return (modifiers & (td::UINT4)KeyModifier::Ctrl);}
/// @brief Returns whether the Alt modifier is active.
/// @param modifiers Combined modifier bitmask.
/// @return True if Alt is pressed.
inline bool isAltPressed(td::UINT4 modifiers) {return (modifiers & (td::UINT4)KeyModifier::Alt);}
/// @brief Returns whether the Cmd (Command/Windows) modifier is active.
/// @param modifiers Combined modifier bitmask.
/// @return True if Cmd is pressed.
inline bool isCmdPressed(td::UINT4 modifiers) {return (modifiers & (td::UINT4)KeyModifier::Cmd);}
/// @brief Returns whether the Fn (Function) modifier is active.
/// @param modifiers Combined modifier bitmask.
/// @return True if Fn is pressed.
inline bool isFnPressed(td::UINT4 modifiers) {return (modifiers & (td::UINT4)KeyModifier::Fn);}

/// @brief Classifies a file-system entry kind used in file dialogs.
enum class FileType : unsigned char {File=0,        ///< Regular file.
    Folder,     ///< Directory / folder.
    FileOrFolder ///< Either a file or a folder.
};
/// @brief Specifies whether a UI element is laid out horizontally or vertically.
enum class Orientation : unsigned char {Horizontal=0, ///< Horizontal orientation.
    Vertical          ///< Vertical orientation.
};
//typedef void* NatShape;
/// @brief Controls how a boolean value is rendered in a table cell or label.
enum class BoolStyle : td::BYTE {Text=0,             ///< Render as plain text ("true"/"false").
    CheckCrossGreenRed,  ///< Render as a green check or red cross icon.
    CircleGreenRed       ///< Render as a green or red circle icon.
};

/// @brief Bitmask that specifies which edges of a rectangle should be clipped.
enum class ClipMode : td::BYTE {None=0,   ///< No clipping applied.
    Left=1,    ///< Clip the left edge.
    Right=2,   ///< Clip the right edge.
    Top=4,     ///< Clip the top edge.
    Bottom=8,  ///< Clip the bottom edge.
    All=15     ///< Clip all four edges.
};

/// @brief Bitwise AND operator for ClipMode flags.
/// @param a First ClipMode operand.
/// @param b Second ClipMode operand.
/// @return True if any bit is set in both operands.
inline bool operator& (ClipMode a, ClipMode b) {
    return static_cast<bool>(
        static_cast<td::BYTE>(a) & static_cast<td::BYTE>(b)
    );
}

#ifdef MU_MACOS
typedef void* Handle;           ///< Platform-specific native handle.
typedef double CoordType;       ///< Scalar type used for 2D coordinates.
typedef void* NatString;        ///< Platform-native string handle.
typedef void* AttributedString; ///< Platform-native attributed string handle.
#elif defined(MU_WINDOWS)
typedef void* Handle;           ///< Platform-specific native handle (Windows).
typedef double CoordType;       ///< Scalar type used for 2D coordinates (Windows).
typedef void* NatString;        ///< Platform-native string handle (Windows).
typedef void* AttributedString; ///< Platform-native attributed string handle (Windows).
#else
typedef void* Handle;              ///< Platform-specific native handle (other).
typedef double CoordType;          ///< Scalar type used for 2D coordinates (other).
typedef td::String* NatString;     ///< Platform-native string handle (other).
typedef td::String* AttributedString; ///< Platform-native attributed string handle (other).
#endif

using Geometry = td::PointAndSize<gui::CoordType>; ///< Rectangle defined by an origin point and a size.
using Size = td::Size<gui::CoordType>;              ///< 2D size (width, height).
using Point = td::Point<gui::CoordType>;            ///< 2D point (x, y).
using Rect = td::RectNormalized<gui::CoordType>;    ///< Axis-aligned rectangle with normalised coordinates.
using Circle = td::Circle<gui::CoordType>;          ///< Circle defined by centre and radius.
using Range = td::Range<td::UINT4>;                 ///< Integer range [start, end).

/// @brief Bundles a platform attributed string with its measured pixel dimensions.
class AttributedStringBlock
{
public:
    gui::AttributedString natString; ///< Platform-native attributed string handle.
    CoordType width;                 ///< Measured pixel width of the rendered string.
    CoordType height;                ///< Measured pixel height of the rendered string.
};

/// @brief Holds drawing attributes used when rendering a symbol (line width, colours).
class SymbolDrawingAttribs
{
public:
    float lineWidth = 1;                             ///< Stroke width in points.
    td::ColorID lineColor = td::ColorID::SysText;   ///< Stroke colour identifier.
    td::ColorID fillColor = td::ColorID::SysCtrlBack; ///< Fill colour identifier.
    //td::LinePattern linePattern = td::LinePattern::Solid;
    td::BYTE displayGroup : 1;       ///< Non-zero to include this symbol in the display group.
    td::BYTE overideLineWidth : 1;   ///< Non-zero to use lineWidth instead of the default.
    //td::BYTE overideLinePattern : 1;
    td::BYTE overideLineColor : 1;   ///< Non-zero to use lineColor instead of the default.
    td::BYTE overideFillColor : 1;   ///< Non-zero to use fillColor instead of the default.
    /// @brief Default constructor; enables display group and disables all attribute overrides.
    SymbolDrawingAttribs()
    : displayGroup(1)
    , overideLineWidth(0)
    //, overideLinePattern(0)
    , overideLineColor(0)
    , overideFillColor(0)
    {
    }

    /// @brief Sets the stroke line width.
    /// @param width New line width in points.
    void setLineWidth(float width)
    {
        lineWidth = width;
    }

    /// @brief Returns the line width truncated to an integer byte value.
    /// @return Line width as a byte.
    td::BYTE getLineWithInt() const
    {
        return td::BYTE(lineWidth);
    }

    /// @brief Returns the line width as a floating-point value.
    /// @return Line width in points.
    float getLineWithFloat() const
    {
        return lineWidth;
    }
};

/// @brief Looks up a localised string for the given key.
/// @param strKey Resource key identifying the string.
/// @return Localised string value.
NATGUI_API td::String tr(const char* strKey);
/// @brief Looks up a localised C-string for the given key.
/// @param strKey Resource key identifying the string.
/// @return Pointer to the localised C-string.
NATGUI_API const char* trCStr(const char* strKey);
/// @brief Retrieves a raw resource text string by key.
/// @param strKey Resource key.
/// @return Resource text as a td::String.
NATGUI_API td::String getResText(const char* strKey);
/// @brief Retrieves a resource file name by key.
/// @param strKey Resource key.
/// @return File name as a td::String.
NATGUI_API td::String getResFileName(const char* strKey);
/// @brief Removes a specific modifier flag from a combined modifier bitmask.
/// @param modifier Combined modifier bitmask.
/// @param toRemove The KeyModifier flag to clear.
/// @return Updated modifier bitmask with the specified flag removed.
NATGUI_API td::UINT4 removeKeyModifier(td::UINT4 modifier, KeyModifier toRemove);

} //namespace gui
