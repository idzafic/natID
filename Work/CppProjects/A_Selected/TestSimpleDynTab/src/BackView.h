//
//  Created by Izudin Dzafic on 16/12/2023.
//  Copyright © 2023 IDz. All rights reserved.
//
#pragma once
#include <gui/View.h>
#include <gui/VerticalLayout.h>
#include <gui/Image.h>
#include <gui/ImageView.h>

class BackView : public gui::View
{
private:
protected:
	gui::Image _etf;
	gui::ImageView _imgView;
    gui::VerticalLayout _vl;
public:
    BackView()
    : _etf(":ETF")
    , _vl(3)
    {
        _vl.appendSpacer();
		_imgView.setImage(&_etf);
        _vl.append(_imgView);
        _vl.appendSpacer();
        setLayout(&_vl);
    }
};
