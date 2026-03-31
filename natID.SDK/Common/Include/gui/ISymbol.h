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
#include <gui/Types.h>
#include <td/ColorID.h>
#include <td/String.h>

namespace gui
{


class ISymbol
{
public:
    enum class Type : unsigned char { NoConnections = 0, OneConnectionOneGroup, NConnectionsOneGroup };
    
    //static ISymbol* create(Symbol::Type symbType = gui::Symbol::Type::NoConnections);
    
    virtual bool load(const td::String& xmlFileName) = 0;
    
    virtual void save(const td::String& xmlFileName) const = 0;
    
    virtual void attachSymbol(int groupNo, int posInGroup, ISymbol* pSymbolToAttach, float scale = 1) = 0;
    
    virtual size_t getNumberOfGroups() const = 0;
    
    virtual size_t getNumberOfConnections() const = 0;
    
    virtual void draw(gui::CoordType dx, gui::CoordType dy, float rotAngleRad=0, const SymbolDrawingAttribs* drawingAttribs = nullptr, float scale = 1) = 0;
    virtual const gui::Rect& getBounds() const = 0;
    
//    virtual void scaleAndRotate(double scaleX, double scaleY, double rotate) = 0; //should be called only once after loading
    
    virtual void release() = 0;

    virtual void showDebugInfo() const = 0;
};

} //namepace gui

