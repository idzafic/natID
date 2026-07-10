// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Legend.h
    @brief Plot legend widget that renders labeled line and band entries in a floating box. */
#pragma once
#include <deque>
#include <gui/DrawableString.h>
#include <vector>
#include <gui/Shape.h>
#include <gui/Font.h>
#include <gui/plot/Plot.h>
#include <td/LineParams.h>

namespace gui
{
namespace plot
{

/// @brief Renders a configurable legend box listing the plots, bands, and helper lines
///        present in a chart view.
class NATPLOT_API Legend
{
    /// @brief Internal descriptor for a single legend entry (line or band label).
    using Desc = struct _Desc
    {
        gui::DrawableString lbl; ///< The rendered label string for this entry.
        td::LineParams lineParams; ///< Line style parameters (color, pattern, width) for the entry symbol.
        td::BYTE isLine = 1;     ///< Non-zero if the entry represents a line; zero for a filled band.
    };

    cnt::PushBackVector<Desc> _labels;   ///< Ordered list of legend entries.
    gui::Size _parentSize = {0,0};       ///< Size of the parent view used to position the legend.
    gui::Rect _window;                   ///< Bounding rectangle of the legend box in view coordinates.

    std::vector<double> _lengths;        ///< Pre-computed label text lengths used for column layout.
    gui::Font* _font;                    ///< Font used to render legend label text.
    //const gui::Point& offset;

    td::UINT2 _nColumns = 1;          ///< Number of columns used to arrange legend entries.
    td::UINT2 _nBands = 0;            ///< Count of band entries in the legend.
    td::UINT2 _nAritraryLines = 0;    ///< Count of arbitrary helper-line entries in the legend.

    td::ColorID _textColor;           ///< Color used to draw the legend label text.
    td::BYTE _initialized = 0;        ///< Non-zero when the legend geometry has been computed.
    td::BYTE _drawBackgroundRect = 1; ///< Non-zero to draw a filled background rectangle behind the legend.

public:

    //void changeLocation(const gui::Point &location);
    /// @brief Constructs a Legend with the given text color and font.
    /// @param textColor Color ID used for all legend label text.
    /// @param font Pointer to the font used to render label text.
    Legend(td::ColorID textColor, gui::Font *font);

    /// @brief Returns the number of legend entries displayed per column.
    /// @return Entry count per column.
    int getNoOfEntriesPerColumn();

    /// @brief Reserves storage capacity for the given number of plot entries.
    /// @param nPlots Expected number of plots (and legend entries) to accommodate.
    void reserve(size_t nPlots)
    {
        _labels.reserve(nPlots);
        _lengths.reserve(nPlots);
    }

    /// @brief Draws the legend onto the current canvas.
    /// @param backColor Background color of the parent view (used for contrast).
    /// @param bDarkMode True if the application is in dark mode.
    void draw(td::ColorID backColor, bool bDarkMode);

    /// @brief Updates the display name of the legend entry at the specified position.
    /// @param name New label text.
    /// @param poz Zero-based index of the entry to update.
    void changeName(const td::String& name, size_t poz);

    /// @brief Updates the color of the legend entry at the specified position.
    /// @param color New color ID for the entry symbol.
    /// @param poz Zero-based index of the entry to update.
    void changeColor(td::ColorID color, size_t poz);

    /// @brief Sets the number of columns used to lay out the legend entries.
    /// @param columnCnt Desired number of columns.
    void changeColumnCnt(size_t columnCnt);

    /// @brief Returns the current number of legend columns.
    /// @return Column count.
    size_t getColumnCnt() const;

    /// @brief Adds a plot's legend entry (line symbol and name) to the legend.
    /// @param f The plot whose style and name are used for the new entry.
    void addPlot(const Plot& f);

    /// @brief Adds a band entry with separate light and dark mode colors.
    /// @param bandName Display name of the band.
    /// @param bandColorL Band fill color for light mode.
    /// @param bandColorD Band fill color for dark mode.
    void addBand(const td::String& bandName, td::ColorID bandColorL, td::ColorID bandColorD);

    /// @brief Adds a standalone helper-line entry with custom line parameters.
    /// @param name Display name of the helper line.
    /// @param lineParams Line style parameters (color, pattern, width) for the symbol.
    void addHelperLine(const td::String& name, const td::LineParams& lineParams);

    /// @brief Returns the current bounding rectangle of the legend box.
    /// @return Const reference to the legend window rectangle.
    const gui::Rect& getWindow();

    /// @brief Moves the origin (top-left anchor) of the legend to the given location.
    /// @param location New origin point in view coordinates.
    void setOrigin(const gui::Point &location);

    /// @brief Updates the parent view size and optionally repositions the legend.
    /// @param parentSize New size of the parent view.
    /// @param updateOrigin True to recompute the legend origin based on the new parent size.
    void setParentSize(const gui::Size& parentSize, bool updateOrigin);

    /// @brief Changes the text color used for all legend labels.
    /// @param color The new text color ID.
    void setTextColor(const td::ColorID &color);

    /// @brief Removes all legend entries and resets internal state.
    void reset();
};

} //namepsace plot
} //namespace gui
