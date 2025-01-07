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
#include "Canvas.h"
#include <gui/Image.h>
#include <gui/Symbol.h>

namespace gui
{
	class NATGUI_API ImageView : public Canvas
	{
    public:
        enum class Type : unsigned char {Image=0, Symbol, NA};
    private:
        union
        {
            const gui::Image* _pImage = nullptr;
            gui::Symbol* _pSymbol;
        };
        Size _imageSize;
        unsigned char _keepRatio = 1;
        Type _type = Type::NA;
        td::HAlignment _hAlign = td::HAlignment::Center;
        td::VAlignment _vAlign = td::VAlignment::Center;
    protected:
        void measure(CellInfo&) override;
        void reMeasure(CellInfo&) override;
        void onDraw(const gui::Rect& rDraw) override;
    public:
        ImageView(bool keepRatio = true);
        ImageView(const Image* image, bool keepRatio = true);
        ImageView(const Symbol* image, bool keepRatio = true);
        ~ImageView();
        gui::ObjType getObjType() const override { return ObjType::ImageView;}
        void setImage(const Image* img);
        void setImage(const Symbol* pSymb);
        ImageView::Type getType() const;
        const gui::Size&  getImageSize() const;
	};
}
