// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Columns.h
    @brief Defines Column, Header, and Columns helper classes for configuring table/grid column descriptors. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "Types.h"
#include <gui/_aux/Format.h>
#include <cnt/SafeFullVector.h>
#include <td/DateTime.h>

namespace gui
{

/// @brief Describes a single data column, including its header text, display formatting, and data binding position.
class Column
{
public:
    const cnt::SafeFullVector<td::String>* pEnumStrings = nullptr; ///< Optional pointer to enumeration strings for enum-typed columns.
    td::String header;    ///< Column header label text.
    td::String toolTip;   ///< Tooltip text shown when hovering over the column header.
    td::INT2 width = 130; ///< Column width in logical pixels.
    union
    {
        td::Date::Format dateFormat;  ///< Format used when the column contains date values.
        char nDecForReal;             ///< Number of decimal places when the column contains real (floating-point) values.
    };
    union
    {
        td::Time::Format timeFormat;       ///< Format used when the column contains time values.
        td::FormatFloat floatingNoFmt;     ///< Floating-point notation format (e.g. decimal, scientific).
    };
    td::UINT4 posInDS = 0;                    ///< Zero-based position of this column in the associated data set.
    td::HAlignment hAlign = td::HAlignment::Left; ///< Horizontal text alignment within the column cells.
    ThSep showThSep = ThSep::AppDefault;      ///< Thousands-separator display policy for numeric columns.

    /// @brief Default constructor; initialises decimal count to 2 and floating format to Decimal.
    Column()
    {
        nDecForReal = 2;
        floatingNoFmt = td::FormatFloat::Decimal;
    }
};

/// @brief Lightweight column descriptor used when constructing table headers with explicit positions.
class Header
{
public:
    td::String header;     ///< Column header label text.
    td::String toolTip;    ///< Tooltip text shown when hovering over the column header.
    td::INT2 width;        ///< Column width in logical pixels.
    td::BYTE posInDS;      ///< Zero-based position of this column in the data set.
    td::HAlignment hAlign; ///< Horizontal text alignment within the column cells.

    /// @brief Constructs a Header with a C-string header label.
    /// @param positionInDS  Zero-based index of the column in the data set.
    /// @param colHeader     Column header text (C-string).
    /// @param colToolTip    Optional tooltip text (C-string).
    /// @param colWidth      Column width in logical pixels (default 100).
    /// @param hAlignCol     Horizontal alignment of cell content (default Left).
    Header(td::BYTE positionInDS, const char* colHeader, const char* colToolTip = nullptr, td::INT2 colWidth = 100, td::HAlignment hAlignCol = td::HAlignment::Left)
    : header(colHeader)
    , toolTip(colToolTip)
    , width(colWidth)
    , posInDS(positionInDS)
    , hAlign(hAlignCol)
    {}

    /// @brief Constructs a Header with a td::String header label.
    /// @param positionInDS  Zero-based index of the column in the data set.
    /// @param colHeader     Column header text.
    /// @param colToolTip    Optional tooltip text.
    /// @param colWidth      Column width in logical pixels (default 100).
    /// @param hAlignCol     Horizontal alignment of cell content (default Left).
    Header(td::BYTE positionInDS, const td::String& colHeader, const td::String& colToolTip = "", td::INT2 colWidth = 100, td::HAlignment hAlignCol = td::HAlignment::Left)
    : header(colHeader)
    , toolTip(colToolTip)
    , width(colWidth)
    , posInDS(positionInDS)
    , hAlign(hAlignCol)
    {}
};

/// @brief Stream-style builder that populates a vector of Column descriptors using operator<<.
class NATGUI_API Columns
{
    cnt::SafeFullVector<gui::Column>& _columns; ///< Reference to the column vector being populated.
    int _iCol;                                  ///< Index of the column currently being configured.
public:
    /// @brief Constructs a Columns builder targeting the given column vector.
    /// @param columns Reference to the column vector to populate.
    Columns(cnt::SafeFullVector<gui::Column>& columns);

    /// @brief Sets the thousands-separator policy for the current column.
    /// @param showThSep  Thousands-separator display policy.
    /// @return Reference to this Columns builder for chaining.
    Columns& operator << (gui::ThSep showThSep);

    /// @brief Sets the numeric format for the current column.
    /// @param numericFormat  Numeric format descriptor.
    /// @return Reference to this Columns builder for chaining.
    Columns& operator << (const NumFormat& numericFormat);

    /// @brief Sets the date format for the current column.
    /// @param dateFormat  Date display format.
    /// @return Reference to this Columns builder for chaining.
    Columns& operator << (td::Date::Format dateFormat);

    /// @brief Sets the time format for the current column.
    /// @param timeFormat  Time display format.
    /// @return Reference to this Columns builder for chaining.
    Columns& operator << (td::Time::Format timeFormat);

    /// @brief Advances to the next column and applies the provided Header descriptor.
    /// @param header  Header descriptor containing label, tooltip, width, and alignment.
    /// @return Reference to this Columns builder for chaining.
    Columns& operator << (const Header& header);
};

} //namespace gui
