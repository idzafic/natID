// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ImageView.h
    @brief Canvas-based view that displays a raster image or a vector symbol. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "Canvas.h"
#include <gui/Image.h>
#include <gui/Symbol.h>

namespace gui
{
	/// @brief A view that renders either a gui::Image or a gui::Symbol, with optional aspect-ratio preservation.
	class NATGUI_API ImageView : public Canvas
	{
    public:
        /// @brief Specifies the kind of content currently displayed by the view.
        enum class Type : unsigned char
        {
            Image=0, ///< View is displaying a raster image.
            Symbol,  ///< View is displaying a vector symbol.
            NA       ///< No content has been assigned.
        };
    private:
        union
        {
            const gui::Image* _pImage = nullptr; ///< Pointer to the displayed raster image.
            gui::Symbol* _pSymbol; ///< Pointer to the displayed vector symbol.
        };
        Size _imageSize; ///< Cached size of the image or symbol for layout purposes.
        unsigned char _keepRatio = 1; ///< Non-zero if the aspect ratio should be preserved when drawing.
        Type _type = Type::NA; ///< Indicates which union member is currently active.
        td::HAlignment _hAlign = td::HAlignment::Center; ///< Horizontal alignment of the content within the view.
        td::VAlignment _vAlign = td::VAlignment::Center; ///< Vertical alignment of the content within the view.
    protected:
        /// @brief Measures the preferred size of the view during an initial layout pass.
        /// @param cellInfo Cell information structure to fill with size requirements.
        void measure(CellInfo&) override;

        /// @brief Re-measures the preferred size of the view after a content change.
        /// @param cellInfo Cell information structure to update with new size requirements.
        void reMeasure(CellInfo&) override;

        /// @brief Draws the image or symbol within the dirty rectangle.
        /// @param rDraw The rectangle that needs to be redrawn.
        void onDraw(const gui::Rect& rDraw) override;

        ImageView(const ImageView&) = delete;
        ImageView& operator =(const ImageView&) = delete;
    public:
        /// @brief Constructs an empty ImageView.
        /// @param keepRatio If true, the aspect ratio is preserved when drawing.
        ImageView(bool keepRatio = true);

        /// @brief Constructs an ImageView displaying the given raster image.
        /// @param image Pointer to the image to display.
        /// @param keepRatio If true, the aspect ratio is preserved when drawing.
        ImageView(const Image* image, bool keepRatio = true);

        /// @brief Constructs an ImageView displaying the given vector symbol.
        /// @param image Pointer to the symbol to display.
        /// @param keepRatio If true, the aspect ratio is preserved when drawing.
        ImageView(const Symbol* image, bool keepRatio = true);

        /// @brief Destructor; releases any held resources.
        ~ImageView();

        /// @brief Returns the object type identifier for this view.
        /// @return ObjType::ImageView.
        gui::ObjType getObjType() const override { return ObjType::ImageView;}

        /// @brief Sets the raster image to display.
        /// @param img Pointer to the image to display. Pass nullptr to clear the content.
        void setImage(const Image* img);

        /// @brief Sets the vector symbol to display.
        /// @param pSymb Pointer to the symbol to display. Pass nullptr to clear the content.
        void setImage(const Symbol* pSymb);

        /// @brief Returns the type of content currently displayed.
        /// @return Current content type (Image, Symbol, or NA).
        ImageView::Type getType() const;

        /// @brief Returns the cached size of the current image or symbol.
        /// @return Const reference to the image size.
        const gui::Size&  getImageSize() const;
	};
}
