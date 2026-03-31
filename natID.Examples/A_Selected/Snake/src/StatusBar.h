//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/StatusBar.h>
#include <gui/Label.h>
#include <gui/Image.h>
#include <gui/ImageView.h>
#include <gui/HorizontalLayout.h>


class StatusBar : public gui::StatusBar
{
protected:
    gui::Label _lblMessage;
    
    gui::Image _imgBanana;
    gui::ImageView _imgvBanana;
    gui::Label _lblNoOfBananas;
    
    gui::Image _imgApple;
    gui::ImageView _imgvApple;
    gui::Label _lblNoOfApples;
    
    gui::Image _imgPear;
    gui::ImageView _imgvPear;
    gui::Label _lblNoOfPears;
    
    gui::Label _lblTotal;
    gui::Label _lblNoOfPoints;

    int _noBananas = 0;
    int _noApples = 0;
    int _noPears = 0;
    int _noPoints = 0;
protected:
    
public:
    StatusBar()
    : gui::StatusBar(13)
    , _lblMessage(tr("ClStart"))
    , _imgBanana(":banana")
    , _lblNoOfBananas(": 0")
    , _imgApple(":apple")
    , _lblNoOfApples(": 0")
    , _imgPear(":pear")
    , _lblNoOfPears(": 0")
    , _lblTotal(tr("Total:"))
    , _lblNoOfPoints(" 0")
    {
        _lblNoOfBananas.setFont(gui::Font::ID::SystemLargestBold);
        _lblNoOfPears.setFont(gui::Font::ID::SystemLargestBold);
        _lblNoOfApples.setFont(gui::Font::ID::SystemLargestBold);
        _lblTotal.setFont(gui::Font::ID::SystemLargestBold);
        _lblNoOfPoints.setFont(gui::Font::ID::SystemLargestBold);
        
        gui::Size szLbl;
        _lblNoOfPears.getMinSize(szLbl);
        
        setMargins(4, 0, 4, 4);
        _imgvBanana.setImage(&_imgBanana);
        _imgvApple.setImage(&_imgApple);
        _imgvPear.setImage(&_imgPear);
        
        _imgvBanana.setFixedHeight(szLbl.height);
        _imgvApple.setFixedHeight(szLbl.height);
        _imgvPear.setFixedHeight(szLbl.height);
        
        _layout.setSpaceBetweenCells(0);
        _layout << _lblMessage;
        _layout.appendSpacer();
        _layout << _imgvApple << _lblNoOfApples;
        _layout.appendSpace(5);
        _layout << _imgvPear << _lblNoOfPears;
        _layout.appendSpace(5);
        _layout << _imgvBanana << _lblNoOfBananas;
        _layout.appendSpace(5);
        
        _layout << _lblTotal << _lblNoOfPoints;
        
        setLayout(&_layout);
    }
    
    void resetPoints()
    {
        _lblNoOfApples.setTitle(": 0");
        _lblNoOfPears.setTitle(": 0");
        _lblNoOfBananas.setTitle(": 0");
        _lblNoOfPoints.setTitle("0");
        _lblMessage.setTitle(tr("clickArrK"));
    }
    
    void updatePoints(td::INT1 fruitType, int fruitPts, int totalScore)
    {
#define USE_BUFFER
#ifdef USE_BUFFER
        char tmp[16];
        SNPRINTF(tmp, 16, _TRUNCATE, ": %d", fruitPts);
        tmp[15] = 0;
        if (fruitType == 0)
        {
            _lblNoOfApples.setTitle(&tmp[0]);
            _lblMessage.setTitle(tr("+apple"));
        }
        else if (fruitType == 1)
        {
            _lblNoOfPears.setTitle(&tmp[0]);
            _lblMessage.setTitle(tr("+pear"));
        }
        else if (fruitType == 2)
        {
            _lblNoOfBananas.setTitle(&tmp[0]);
            _lblMessage.setTitle(tr("+banana"));
        }
        else
        {
            assert(false);
        }
        
        SNPRINTF(tmp, 16, _TRUNCATE, "%d", totalScore);
        tmp[15] = 0;
        _lblNoOfPoints.setTitle(&tmp[0]);
#else
        td::String pts;
        pts.format(": %d", fruitPts);
        if (fruitType == 0)
        {
            _lblNoOfApples.setTitle(pts);
            _lblMessage.setTitle(tr("+apple"));
        }
        else if (fruitType == 1)
        {
            _lblNoOfPears.setTitle(pts);
            _lblMessage.setTitle(tr("+pear"));
        }
        else if (fruitType == 2)
        {
            _lblNoOfBananas.setTitle(pts);
            _lblMessage.setTitle(tr("+banana"));
        }
        else
        {
            assert(false);
        }
        
        pts.format("%d", totalScore);
        _lblNoOfPoints.setTitle(pts);
#endif
        
    }
    
};
