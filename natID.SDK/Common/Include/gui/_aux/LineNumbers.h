// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file LineNumbers.h
 * @brief Canvas sidebar that displays line numbers for a TextEdit or DataViewBody.
 */
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

/// @brief Renders a line-number gutter alongside either a TextEdit or DataViewBody widget.
class NATGUI_API LineNumbers : public Canvas, public ILineNumbers
{
    /// @brief Identifies which type of widget this line-numbers panel is attached to.
    enum class Type : td::BYTE {TextEdit=0, DataView};
public:
    /// @brief Controls the visibility and counting base of line numbers.
    enum class Style : td::BYTE {NotVisible, ZeroBased, OneBased};
protected:
    union
    {
        TextEdit* _pTE = nullptr;     ///< Pointer to an attached TextEdit (when Type == TextEdit).
        DataViewBody* _pDBV;          ///< Pointer to an attached DataViewBody (when Type == DataView).
    };
    CoordType _dy = 0;                ///< Current vertical scroll offset in pixels.
    CoordType _lineHeight = 0;        ///< Height of a single line in pixels.
    CoordType _y = 0;                 ///< Vertical origin of the first visible line.
    td::UINT4 _totalLines = 0;        ///< Total number of lines in the attached widget.
    td::UINT4 _firstVisibleLine;      ///< One-based index of the topmost visible line.
    td::UINT4 _currentLine = 0;       ///< One-based index of the currently active (cursor) line.
    td::UINT4 _startingPos = 0;       ///< Starting line number offset (for OneBased vs. ZeroBased).
    td::BYTE _nDigits = 3;            ///< Minimum number of digits used to format line numbers.

    Type _type = Type::TextEdit;      ///< Which widget type this panel is attached to.
    Style _style = Style::OneBased;   ///< Display style for line numbering.

protected:
    /// @brief Measures the preferred width of the line-number panel.
    /// @param cell Cell info structure to populate with size constraints.
    void measure(CellInfo&) override;
    /// @brief Re-measures the panel after font or line-count changes.
    /// @param cell Cell info structure to update.
    void reMeasure(CellInfo&) override;

    //ILineNumbers

    /// @brief Returns the one-based index of the currently active line.
    /// @return Current line number.
    td::UINT4 getCurrentLine() const override;
    //void setTotalLines(gui::CoordType lineHeight, td::UINT4 totalLines) override;

    /// @brief Called when the attached widget's font changes; triggers a re-measure.
    void fontChanged() override;
    /// @brief Returns the horizontal gap to leave on the right side of the gutter.
    /// @return Offset in pixels.
    CoordType getOffsetOnRightSide() const override;
    /// @brief Returns the normal text color for line numbers.
    /// @return Color identifier.
    td::ColorID getColor() const override;
    /// @brief Returns the text color used for the selected (current) line number.
    /// @return Color identifier.
    td::ColorID getColorForSelectedLine() const override;

    /// @brief Draws the visible line numbers within the given dirty rectangle.
    /// @param rect The rectangle region that needs to be redrawn.
    void onDraw(const gui::Rect& rect) override;
    /// @brief Deleted default constructor; a widget pointer is required.
    LineNumbers() = delete;
public:
    /// @brief Constructs a LineNumbers panel attached to a TextEdit widget.
    /// @param pTE Pointer to the TextEdit to track.
    explicit LineNumbers(TextEdit* pTE);
    /// @brief Constructs a LineNumbers panel attached to a DataViewBody widget.
    /// @param pDV Pointer to the DataViewBody to track.
    explicit LineNumbers(DataViewBody* pDV);
    /// @brief Updates the total line count and optionally forces a redraw.
    /// @param totalNoOfLines New total number of lines.
    /// @param forceRedraw True to redraw immediately even if the count has not changed.
    void setTotalLines(td::UINT4 totalNoOfLines, bool forceRedraw = false) override;
    /// @brief Returns the total number of lines tracked by this panel.
    /// @return Total line count.
    td::UINT4 getTotalLines() const override;
    /// @brief Returns a const pointer to the attached TextEdit, if any.
    /// @return Pointer to the TextEdit, or nullptr if attached to a DataViewBody.
    const TextEdit* getTextEdit() const;
    /// @brief Sets the currently highlighted line number.
    /// @param lineNo One-based line number to highlight.
    void setCurrentLine(td::UINT4 lineNo) override;
    /// @brief Updates the vertical scroll position to keep line numbers in sync.
    /// @param dy New vertical scroll offset in pixels.
    void setScrollPos(CoordType dy) override;
    /// @brief Sets the display style for line numbering.
    /// @param style The desired Style value.
    void setStyle(Style style);
};

}
