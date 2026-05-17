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
#include <gui/Canvas.h>
#include <td/ColorID.h>
#include <gui/ILineNumbers.h>

namespace gui
{

class TextEdit;
class DataViewBody;

class NATGUI_API LineNumbers : public Canvas, public ILineNumbers
{
    enum class Type : td::BYTE {TextEdit=0, DataView};
public:
    enum class Style : td::BYTE {NotVisible, ZeroBased, OneBased};
protected:
    union
    {
        TextEdit* _pTE = nullptr;
        DataViewBody* _pDBV;
    };
    CoordType _dy = 0;
    CoordType _lineHeight = 0;
    CoordType _y = 0;
    td::UINT4 _totalLines = 0;
    td::UINT4 _firstVisibleLine;
    td::UINT4 _currentLine = 0;
    td::UINT4 _startingPos = 0;
    td::BYTE _nDigits = 3;

    Type _type = Type::TextEdit;
    Style _style = Style::OneBased;

protected:
    void measure(CellInfo&) override;
    void reMeasure(CellInfo&) override;
    
    //ILineNumbers
    
    td::UINT4 getCurrentLine() const override;
    //void setTotalLines(gui::CoordType lineHeight, td::UINT4 totalLines) override;
    
    void fontChanged() override;
    CoordType getOffsetOnRightSide() const override;
    td::ColorID getColor() const override;
    td::ColorID getColorForSelectedLine() const override;
    
    void onDraw(const gui::Rect& rect) override;
    LineNumbers() = delete;
public:
    explicit LineNumbers(TextEdit* pTE);
    explicit LineNumbers(DataViewBody* pDV);
    void setTotalLines(td::UINT4 totalNoOfLines, bool forceRedraw = false) override;
    td::UINT4 getTotalLines() const override;
    const TextEdit* getTextEdit() const;
    void setCurrentLine(td::UINT4 lineNo) override;
    void setScrollPos(CoordType dy) override;
    void setStyle(Style style);
};

}
