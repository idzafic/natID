// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Symbol.h
    @brief Scalable vector symbol loaded from a resource or XML file. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//

#pragma once
#include <gui/natGUI.h>
#include <gui/ISymbol.h>
//#include <gui/GraphicsSharedStore.h>

namespace gui
{

/// @brief Represents a scalable vector symbol that can be drawn onto a canvas.
class NATGUI_API Symbol
{
protected:
    ISymbol* _pHandle = nullptr; ///< Pointer to the native/platform symbol implementation.
    td::BYTE _fromRes = 0;       ///< Non-zero if the symbol was loaded from a compiled resource.

    Symbol(const Symbol&) = delete;
    Symbol& operator =(const Symbol&) = delete;
public:
    /// @brief Default constructor. Creates an uninitialised symbol.
    Symbol();

    /// @brief Constructs a symbol and immediately loads it from a resource ID or XML file path.
    /// @param resIDorXMLFileName Resource identifier string or path to an XML symbol file.
    Symbol(const char* resIDorXMLFileName);

    /// @brief Constructs a symbol and loads it from a resource ID or XML file path.
    /// @param resIDorXMLFileName Resource identifier or XML file path as a String.
    Symbol(const td::String& resIDorXMLFileName);

    /// @brief Destructor. Releases the native symbol resources.
    ~Symbol();

    /// @brief Loads (or reloads) the symbol from a resource ID or XML file path.
    /// @param resIDorXMLFileName Resource identifier or XML file path.
    /// @return True if the symbol was loaded successfully.
    bool load(const char* resIDorXMLFileName);

    /// @brief Loads (or reloads) the symbol from a resource ID or XML file path.
    /// @param resIDorXMLFileName Resource identifier or XML file path as a String.
    /// @return True if the symbol was loaded successfully.
    bool load(const td::String& resIDorXMLFileName);

    /// @brief Saves the symbol to an XML file.
    /// @param xmlFileName Destination file path.
    void save(const td::String& xmlFileName) const;

    /// @brief Returns the number of drawing groups contained in the symbol.
    /// @return Group count.
    size_t getNumberOfGroups() const;

    /// @brief Returns the number of connection points defined in the symbol.
    /// @return Connection count.
    size_t getNumberOfConnections() const;

    /// @brief Attaches a child symbol at a specified group and position within that group.
    /// @param groupNo Group index where the child symbol is attached.
    /// @param posInGroup Position index within the group.
    /// @param symbolToAttach Reference to the symbol to attach.
    /// @param scale Scale factor applied to the attached symbol.
    void attachSymbol(int groupNo, int posInGroup, Symbol& symbolToAttach, float scale = 1);

    /// @brief Removes a previously attached child symbol.
    /// @param groupNo Group index from which to remove the attachment.
    /// @param posInGroup Position index of the attached symbol to remove.
    void removeAttachedSymbol(int groupNo, int posInGroup);

    /// @brief Draws the symbol at the given offset with an optional rotation and style.
    /// @param dx Horizontal translation offset.
    /// @param dy Vertical translation offset.
    /// @param rotAngleRad Rotation angle in radians (default 0).
    /// @param drawingAttribs Optional drawing attributes; nullptr uses defaults.
    /// @param scale Scale factor applied when drawing.
    void draw(gui::CoordType dx, gui::CoordType dy, float rotAngleRad=0, const SymbolDrawingAttribs* drawingAttribs = nullptr, float scale = 1);

    /// @brief Draws the symbol using a rotation angle expressed in degrees.
    /// @param dx Horizontal translation offset.
    /// @param dy Vertical translation offset.
    /// @param rotAngleDeg Rotation angle in degrees (default 0).
    /// @param drawingAttribs Optional drawing attributes; nullptr uses defaults.
    /// @param scale Scale factor applied when drawing.
    void drawDeg(gui::CoordType dx, gui::CoordType dy, float rotAngleDeg = 0, const SymbolDrawingAttribs* drawingAttribs = nullptr, float scale = 1);

    /// @brief Draws the symbol centred on a point with an optional rotation.
    /// @param center Centre point in canvas coordinates.
    /// @param rotAngleRad Rotation angle in radians (default 0).
    /// @param drawingAttribs Optional drawing attributes; nullptr uses defaults.
    /// @param scale Scale factor applied when drawing.
    void draw(const gui::Point& center, float rotAngleRad = 0, const SymbolDrawingAttribs* drawingAttribs = nullptr, float scale = 1);

    /// @brief Draws the symbol centred on a point using a rotation angle in degrees.
    /// @param center Centre point in canvas coordinates.
    /// @param rotAngleDeg Rotation angle in degrees (default 0).
    /// @param drawingAttribs Optional drawing attributes; nullptr uses defaults.
    /// @param scale Scale factor applied when drawing.
    void drawDeg(const gui::Point& center, float rotAngleDeg = 0, const SymbolDrawingAttribs* drawingAttribs = nullptr, float scale = 1);

    /// @brief Draws the symbol scaled to fit within a bounding rectangle.
    /// @param boundRect Rectangle to fit the symbol into.
    /// @param rotationRad Rotation angle in radians (default 0).
    /// @param pGroupAttribs Optional per-group drawing attributes.
    void drawInRect(const gui::Rect& boundRect, float rotationRad=0, const gui::SymbolDrawingAttribs* pGroupAttribs = nullptr);

    /// @brief Returns the natural bounding rectangle of the symbol.
    /// @return Const reference to the bounds rectangle.
    const gui::Rect& getBounds() const;

    /// @brief Returns the height-to-width aspect ratio of the symbol.
    /// @return Aspect ratio (height / width).
    float getHWRatio() const;

    /// @brief Returns the default drawing attributes used when none are provided.
    /// @return Pointer to the default SymbolDrawingAttribs.
    static const SymbolDrawingAttribs* getDefaultSymbolAttribs();

    /// @brief Returns the drawing attributes used when a symbol is in a selected state.
    /// @return Pointer to the selected-state SymbolDrawingAttribs.
    static const SymbolDrawingAttribs* getDefaultSelectedSymbolAttribs();
};

} //namepace gui
