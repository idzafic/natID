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
#include <gui/ISymbol.h>
//#include <gui/GraphicsSharedStore.h>

namespace gui
{


class NATGUI_API Symbol
{
protected:
    ISymbol* _pHandle = nullptr;
    td::BYTE _fromRes = 0;
    
public:
    Symbol();
    
    Symbol(const char* resIDorXMLFileName);
    Symbol(const td::String& resIDorXMLFileName);
    
    ~Symbol();
    
    bool load(const char* resIDorXMLFileName);
    bool load(const td::String& resIDorXMLFileName);
    
    void save(const td::String& xmlFileName) const;
    
    size_t getNumberOfGroups() const;
    
    size_t getNumberOfConnections() const;
    
    void attachSymbol(int groupNo, int posInGroup, Symbol& symbolToAttach, float scale = 1);
    
    void removeAttachedSymbol(int groupNo, int posInGroup);
    
    void draw(gui::CoordType dx, gui::CoordType dy, float rotAngleRad=0, const SymbolDrawingAttribs* drawingAttribs = nullptr, float scale = 1);
    
    void drawDeg(gui::CoordType dx, gui::CoordType dy, float rotAngleDeg = 0, const SymbolDrawingAttribs* drawingAttribs = nullptr, float scale = 1);
    
    void draw(const gui::Point& center, float rotAngleRad = 0, const SymbolDrawingAttribs* drawingAttribs = nullptr, float scale = 1);
    
    void drawDeg(const gui::Point& center, float rotAngleDeg = 0, const SymbolDrawingAttribs* drawingAttribs = nullptr, float scale = 1);
    
    void drawInRect(const gui::Rect& boundRect, float rotationRad=0, const gui::SymbolDrawingAttribs* pGroupAttribs = nullptr);
    
    const gui::Rect& getBounds() const;
    
    float getHWRatio() const;
    
    static const SymbolDrawingAttribs* getDefaultSymbolAttribs();
    static const SymbolDrawingAttribs* getDefaultSelectedSymbolAttribs();
};

} //namepace gui

