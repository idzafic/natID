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
#include "DrawableView.h"
#include "Cursor.h"

namespace gui
{

class Image;
class ViewScroller;
class CanvasHelper;

class NATGUI_API Canvas : public DrawableView
{
    friend class CanvasHelper;
public:
    enum class Backend : unsigned char {Display=0, Bitmap, EPS, PDF, SVG, Printer};

protected:
    ViewScroller* _pScroller = nullptr;
    td::UINT2 _fixedWidth = 0;
    td::UINT2 _fixedHeight = 0;
    Backend _backend = Backend::Display;
protected:
    Canvas(int);
    void measure(CellInfo& cell) override;
    void reMeasure(CellInfo& cell) override;
    void setScale(double newScale) override;
//    
    bool onKeyPressed(const gui::Key& key) override;

    void enableResizeEvent(bool bEnable); //false if not called

    void setScroller(ViewScroller* pScroller) override;
    ViewScroller* getScroller();
    
    void handleModelSizeChanged();
    void setPreferredFrameRateRange(float minFPS, float maxFPS);
    
public:
    Canvas();
    Canvas(const std::initializer_list<InputDevice::Event>& events, const Geometry* g = nullptr);
    ~Canvas();
    gui::ObjType getObjType() const override { return ObjType::Canvas;}

    void getVisiblePartInModelCoordinates(gui::Rect& r) const;
    
    void getSize(Size& sz) const;
    
    void scale(double newScale) override;
    void scaleToPoint(double newScale, const gui::Point& toPoint) override;
    
    double getScale() const override;

    void setCursor(Cursor::Type ct, bool showImmediately = false) const;
    
    //Do not use Tiners or threads to implement animation
    //These two methods should be used for animation
    void startAnimation();
    void stopAnimation();
    bool isAnimating() const;
    
//    void showCursor(bool bShow); //Show/hide cursor
//    void moveCursorToCenter() const; //moves cursor to center of this view
    
    void setFixedWidth(td::WORD w);
    td::WORD getFixedWidth() const;
    
    void setFixedHeight(td::WORD h);
    td::WORD getFixedHeight() const;
    
    bool drawToImage(gui::Image& imgToDrawOn, double resScale=1.0, bool onlyVisiblePart=true);
    bool drawToImage(gui::Image& imgToDrawOn, const gui::Rect& modelBound, double resScale = 1.0);
    
    bool exportToEPS(const td::String& fileName, bool onlyVisiblePart=true);
    bool exportToEPS(const gui::Rect& modelBound, const td::String& fileName);
    
    bool exportToPDF(const td::String& fileName, bool onlyVisiblePart=true);
    bool exportToPDF(const gui::Rect& modelBound, const td::String& fileName);
    
    bool exportToSVG(const td::String& fileName, bool onlyVisiblePart=true);
    bool exportToSVG(const gui::Rect& modelBound, const td::String& fileName);
    
    bool startPrinting(const PageInfo* pi);
    bool startPrintingToPDF(const PageInfo* pi, const td::String& fileName);
    
//    bool exportToArtwork(gui::Artwork artworkType, const td::String& fileName, bool onlyVisiblePart=true);
//    bool exportToSVG(gui::Artwork artworkType, const gui::Rect& modelBound, const td::String& fileName);

};

} //namespace gui
