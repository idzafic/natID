// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ISymbol.h
    @brief Interface for scalable vector symbol objects that can be loaded, drawn, and attached. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//

#pragma once
#include <gui/Types.h>
#include <td/ColorID.h>
#include <td/String.h>

namespace gui
{


/// @brief Abstract interface representing a scalable, rotatable symbol that can be composed from sub-symbols.
class ISymbol
{
public:
    /// @brief Classifies the connection topology of the symbol.
    enum class Type : unsigned char
    {
        NoConnections = 0,        ///< Symbol has no connection points.
        OneConnectionOneGroup,    ///< Symbol has one connection point and one group.
        NConnectionsOneGroup      ///< Symbol has multiple connection points in a single group.
    };

    //static ISymbol* create(Symbol::Type symbType = gui::Symbol::Type::NoConnections);

    /// @brief Loads the symbol definition from an XML file.
    /// @param xmlFileName Path to the XML file containing the symbol definition.
    /// @return True if the symbol was loaded successfully.
    virtual bool load(const td::String& xmlFileName) = 0;

    /// @brief Saves the symbol definition to an XML file.
    /// @param xmlFileName Path to the XML file to write.
    virtual void save(const td::String& xmlFileName) const = 0;

    /// @brief Attaches a child symbol to a specific group and position within this symbol.
    /// @param groupNo Group index to attach the child symbol to.
    /// @param posInGroup Position within the group.
    /// @param pSymbolToAttach Pointer to the symbol to attach.
    /// @param scale Scale factor to apply to the attached symbol.
    virtual void attachSymbol(int groupNo, int posInGroup, ISymbol* pSymbolToAttach, float scale = 1) = 0;

    /// @brief Returns the number of groups in this symbol.
    /// @return Number of groups.
    virtual size_t getNumberOfGroups() const = 0;

    /// @brief Returns the number of connection points in this symbol.
    /// @return Number of connections.
    virtual size_t getNumberOfConnections() const = 0;

    /// @brief Draws the symbol at the specified offset with optional rotation and attributes.
    /// @param dx Horizontal drawing offset.
    /// @param dy Vertical drawing offset.
    /// @param rotAngleRad Rotation angle in radians.
    /// @param drawingAttribs Optional pointer to drawing attributes. Pass nullptr for defaults.
    /// @param scale Scale factor to apply when drawing.
    virtual void draw(gui::CoordType dx, gui::CoordType dy, float rotAngleRad=0, const SymbolDrawingAttribs* drawingAttribs = nullptr, float scale = 1) = 0;

    /// @brief Returns the axis-aligned bounding rectangle of the symbol in local coordinates.
    /// @return Const reference to the bounding rectangle.
    virtual const gui::Rect& getBounds() const = 0;

//    virtual void scaleAndRotate(double scaleX, double scaleY, double rotate) = 0; //should be called only once after loading

    /// @brief Releases all resources held by the symbol.
    virtual void release() = 0;

    /// @brief Outputs debug information about the symbol to the console or log.
    virtual void showDebugInfo() const = 0;
};

} //namepace gui
