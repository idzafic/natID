// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ViewScroller.h
    @brief Scrollable container view that wraps a content view and provides horizontal and vertical scrollers. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "BaseView.h"
#include "Types.h"
#include "IScrollConsumer.h"

namespace gui
{

class ViewScrollerHelper;
/// @brief Wraps any BaseView in a scrollable viewport with configurable horizontal and vertical scrollers.
class NATGUI_API ViewScroller : public BaseView
{
    friend class ViewScrollerHelper;

public:
    /// @brief Describes the visibility and behaviour of a single scroll bar.
    enum class Type : unsigned char {NoScroll = 0,           ///< No scroll bar; content is clipped.
        ScrollerAlwaysVisible,  ///< Scroll bar is always displayed.
        ScrollAndAutoHide,      ///< Scroll bar appears while scrolling and auto-hides.
        ScrollExternal          ///< Scrolling is handled by an external IScrollConsumer.
    };
protected:
    BaseView* _pContentView = nullptr;        ///< The view whose content is scrolled.
    IScrollConsumer* _pScrollConsumer = nullptr; ///< Optional external scroll consumer.

    ViewScroller::Type _horizontal; ///< Scroll behaviour for the horizontal axis.
    ViewScroller::Type _vertical;   ///< Scroll behaviour for the vertical axis.
protected:
    /// @brief Internal constructor used by subclasses.
    /// @param dummy Unused discriminator.
    /// @param horizontal Horizontal scroll type.
    /// @param vertical Vertical scroll type.
    ViewScroller(int, Type, Type);
    /// @brief Measures the preferred size of the scroller.
    /// @param cell Cell information structure to populate.
    void measure(CellInfo& cell) override;
    /// @brief Re-measures the scroller after a content change.
    /// @param cell Cell information structure to update.
    void reMeasure(CellInfo& cell) override;

    /// @brief Called when the scroller's geometry changes.
    /// @param newGeometry New bounding geometry.
    void onGeometryChange(const Geometry& newGeometry) override;

    /// @brief Called when the visible origin of the content changes due to scrolling.
    /// @param newOrigin New top-left point in content coordinates.
    virtual void onContentScrolled(const gui::Point& newOrigin);

    ViewScroller() = delete;
    ViewScroller(const ViewScroller&) = delete;
    ViewScroller& operator =(const ViewScroller&) = delete;
public:
    /// @brief Constructs a ViewScroller with the specified scroll bar behaviour on each axis.
    /// @param horizontal Scroll behaviour for the horizontal axis.
    /// @param vertical Scroll behaviour for the vertical axis.
    ViewScroller(Type horizontal, Type vertical);
    /// @brief Destructor.
    ~ViewScroller();

    /// @brief Returns the GUI object type identifier.
    /// @return ObjType::ViewScroller.
    gui::ObjType getObjType() const override { return ObjType::ViewScroller;}
    /// @brief Sets the logical size of the scrollable content area.
    /// @param size New content size in content-space units.
    void setContentSize(const gui::Size& size);
    /// @brief Returns the logical size of the scrollable content area.
    /// @param size Output parameter filled with the content size.
    void getContentSize(gui::Size& size);

    /// @brief Sets the view whose content will be scrolled.
    /// @param pContentView Pointer to the content view.
    void setContentView(BaseView* pContentView);
    /// @brief Returns a const pointer to the content view.
    /// @return Const pointer to the current content view.
    const BaseView* getContentView() const;
    /// @brief Returns a pointer to the content view.
    /// @return Pointer to the current content view.
    BaseView* getContentView();
    /// @brief Returns the scroll behaviour on the horizontal axis.
    /// @return Current horizontal Type value.
    ViewScroller::Type getHorizontalScrollerType() const;
    /// @brief Returns the scroll behaviour on the vertical axis.
    /// @return Current vertical Type value.
    ViewScroller::Type getVerticalScrollerType() const;
    /// @brief Scrolls the content so that the specified rectangle is visible.
    /// @param rToShow Rectangle (in content coordinates) to bring into view.
    void makeVisible(const Rect& rToShow);
    /// @brief Returns the currently visible rectangle of the content area.
    /// @param r Output parameter filled with the visible rectangle in content coordinates.
    void getVisibleRect(Rect& r) const;

    /// @brief Scales the content by the given factor.
    /// @param newScale New uniform scale factor.
    void scaleContent(double newScale);
    /// @brief Scales the content around a specific point.
    /// @param newScale New uniform scale factor.
    /// @param atPoint Anchor point (in view coordinates) that remains fixed during scaling.
    void scaleContentAtPoint(double newScale, const gui::Point& atPoint);

    /// @brief Returns the current visible origin in content coordinates.
    /// @param origin Output parameter filled with the top-left visible point.
    void getVisibleOrigin(gui::Point& origin) const;
    /// @brief Scrolls so that the given content point appears at the top-left of the viewport.
    /// @param newOrigin Desired new top-left origin in content coordinates.
    void scrollVisibleOriginTo(const gui::Point& newOrigin);

    /// @brief Returns the model-space origin of the viewport.
    /// @param modelOrigin Output parameter filled with the model-space origin point.
    void getOrigin(gui::Point& modelOrigin) const;
    /// @brief Returns the model-space size of the visible page.
    /// @param modelPageSize Output parameter filled with the model-space page size.
    void getPageSize(gui::Size& modelPageSize) const;

    /// @brief Returns the current content scale factor.
    /// @return Scale factor applied to the content view.
    double getScale() const override;

    //void reserveScrollConsumers(size_t nScrollConsumers);
    /// @brief Sets an external object that receives scroll notifications.
    /// @param pConsumers Pointer to the IScrollConsumer implementation.
    void setScrollConsumer(IScrollConsumer* pConsumers);

    /// @brief Notifies the scroller that the model's logical size has changed and the layout should be updated.
    void handleModelSizeChanged();
};

} //namespace gui
