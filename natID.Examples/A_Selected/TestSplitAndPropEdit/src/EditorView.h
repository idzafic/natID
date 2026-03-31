//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Canvas.h>
//#include <gui/Image.h>
#include <gui/Context.h>
#include <gui/Symbol.h>
//#include <gui/GraphicsShapeFactory.h>
#include <gui/Image.h>
#include <gui/Shape.h>
#include <gui/DrawableString.h>
#include "property/DefaultSettings.h"
#include "model/Model.h"
#include <gui/Cursor.h>
#include <gui/PropertyEditorSwitcher.h>

//global parameters
extern DefaultSettings g_defaultSettings;

const double deltaKeyboardMove = 30;

class EditorView : public gui::Canvas
{
    enum class LastEvent : unsigned char {None, MouseClick, Drag};
protected:
    Model _model;
    IShape2D* _pSelectedShape = nullptr;
    gui::PropertyEditorSwitcher* _pPropSwitcher = nullptr;
    gui::Point _lastMouseClickPoint;
    LastEvent _lastEvent = LastEvent::None;
    gui::Alert::CallBack _callBackDeleteSelectedShape;
protected:
    
    void updatePropertyValues()
    {
        _pPropSwitcher->updateValues();
        setFocus(); //to this
    }
    
    void deleteSelectedShape()
    {
        if (!_pSelectedShape)
            return;
        _model.remove(_pSelectedShape);        
        _pPropSwitcher->showView(0);
        setFocus(); //to this
        reDraw();
    }
    
    void checkDeleteSelectedAnswer(gui::Alert::Answer answer)
    {
        if (answer == gui::Alert::Answer::Yes)
        {
            deleteSelectedShape();
        }
        setFocus(); //to this
    }
    
    bool getModelSize(gui::Size& modelSize) const override
    {
        modelSize = _model.getModelSize();
        return true;
    }
    
    void onDraw(const gui::Rect& rDraw) override
    {
        //mu::dbgLog("onDraw x=%.1f y=%.1f w=%.1f h=%.1f", rDraw.left, rDraw.top, rDraw.width(), rDraw.height());
        //
        //gui::Transformation::saveContext();
        //{
        //    auto r = rDraw;
        //    gui::Size sz;
        //    getSize(sz);
        //    r.left = 0;
        //    r.top = 0;
        //    r.setSize(sz);
        //    r.inflate(-3);
        //    gui::Transformation transf;
        //    transf.scale(1.0);
        //    transf.setToContext();
        //    gui::Shape::drawRect(r, td::ColorID::DarkGreen, td::ColorID::Yellow, 1.0f);
        //}
        //gui::Transformation::restoreContext();
        
        _model.draw(rDraw);
        if (_pSelectedShape)
        {
            gui::Rect r;
            _pSelectedShape->getBoundingRect(r);
            gui::Shape::drawSelectionRect(r);
        }
    }
    
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
        auto pAI = aiDesc.getActionItem();
        
        if (menuID == 100)
        {
            assert(_pSelectedShape);
            if (!_pSelectedShape)
                return true;
            //context menu;
            switch (actionID)
            {
                case 10:
                    //move front
                    //TODO: vjezba
                    if (_model.setBack(_pSelectedShape))
                        reDraw();
                    break;
                case 20:
                    //move back
                    //TODO: vjezba
                    if (_model.setFront(_pSelectedShape))
                        reDraw();
                    break;
                case 30:
                {
                    //Delete without checking
                    deleteSelectedShape();
                    break;
                }
            }
            return true;
        }
        return false;
    }
    
    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override
    {
        const gui::Point& modelPoint = inputDevice.getModelPoint();
        
        _lastMouseClickPoint = modelPoint;
        _lastEvent = LastEvent::MouseClick;
        mu::dbgLog("EditorView onMouseDown!");
        switch (IShape2D::currentTool)
        {
            case IShape2D::Tool::Selector:
            {
                IShape2D* pSelected = _model.getSelectedElement(modelPoint);
                
                if (_pSelectedShape != pSelected)
                {
                    reDraw();
                    if (pSelected)
                    {
                        int pos = (int) pSelected->getType();
                        ++pos;
                        if (_pPropSwitcher)
                            _pPropSwitcher->setCurrentEditor(pos, pSelected, true);
                    }
                    else
                    {
                        _pPropSwitcher->showView(0);
                    }
                    
                }
                setFocus(); //to this
                _pSelectedShape = pSelected;
            }
                break;
            case IShape2D::Tool::AddRect:
            {
                gui::Size sz(g_defaultSettings.getRectWidth(), g_defaultSettings.getRectHeight());
                gui::Rect rect(modelPoint, sz);
                
                IShape2D* pShape = IShape2D::createRect(g_defaultSettings.getAttribs(), rect, g_defaultSettings.getFillColor(), g_defaultSettings.getLineColor(), g_defaultSettings.getLineWidth(), g_defaultSettings.getLinePattern());
                _model.appendShape(pShape, true);
                reDraw();
            }
                break;
            case IShape2D::Tool::AddRoundRect:
            {
                gui::Size sz(g_defaultSettings.getRectWidth(), g_defaultSettings.getRectHeight());
                gui::Rect rect(modelPoint, sz);

                IShape2D* pShape = IShape2D::createRoundedRect(g_defaultSettings.getAttribs(), rect, g_defaultSettings.getRoundedRectRadius(), g_defaultSettings.getFillColor(), g_defaultSettings.getLineColor(), g_defaultSettings.getLineWidth(), g_defaultSettings.getLinePattern());
                _model.appendShape(pShape, true);
                reDraw();
            }
                break;
            case IShape2D::Tool::AddCircle:
            {
                IShape2D* pShape = IShape2D::createCircle(g_defaultSettings.getAttribs(), modelPoint, g_defaultSettings.getCircleRadius(), g_defaultSettings.getFillColor(), g_defaultSettings.getLineColor(), g_defaultSettings.getLineWidth(), g_defaultSettings.getLinePattern());
                _model.appendShape(pShape, true);
                reDraw();
            }
                break;
            default:
                assert(false);
        }
    }

    void onPrimaryButtonReleased(const gui::InputDevice& inputDevice) override
    {
        _lastEvent = LastEvent::None;
    }
    
    void onSecondaryButtonPressed(const gui::InputDevice& inputDevice) override
    {
        if (IShape2D::currentTool != IShape2D::Tool::Selector)
            return;
        
        if (!_pSelectedShape)
            return;
        gui::Rect boundRect;
        _pSelectedShape->getBoundingRect(boundRect);
        boundRect.inflate(5.0);
        
        const gui::Point& modelPoint = inputDevice.getModelPoint();
        if (!boundRect.contains(modelPoint))
            return;
        
        td::BYTE ctxMenuID = 100;
        Frame::openContextMenu(ctxMenuID, inputDevice);
        return;
    }
    
    void onCursorDragged(const gui::InputDevice& inputDevice) override //onMouseDragged(const gui::Point& modelPoint, td::UINT4 keyModifiers)
    {
        if ((_lastEvent != LastEvent::Drag) && (_lastEvent != LastEvent::MouseClick))
        {
            return;
        }
        _lastEvent = LastEvent::Drag;
        //mu::dbgLog("Cursor dragged!");

        const gui::Point& modelPoint = inputDevice.getModelPoint();
        if (_pSelectedShape == nullptr)
            return;
        gui::Point delta(modelPoint.x - _lastMouseClickPoint.x, modelPoint.y - _lastMouseClickPoint.y);
        
        _pSelectedShape->translate(delta);
        updatePropertyValues();
        _lastMouseClickPoint = modelPoint;
        reDraw();
        
    }
   
    bool onZoom(const gui::InputDevice& inputDevice) override //onScrollWheel(const gui::Point& modelPoint, const gui::Point& dXdY, td::UINT4 keyModifiers)
    {
        auto& modelPoint = inputDevice.getModelPoint();
        double scale = inputDevice.getScale();
        double oldScale = getScale();
        double newScale = oldScale * scale; //scale * (1+ dXdY.y/100);
        scaleToPoint(newScale, modelPoint);
        return true;
    }
    
    bool onKeyPressed(const gui::Key& key) override
    {
        //call base class
        if (gui::Canvas::onKeyPressed(key))
            return true;
        
        if (_pSelectedShape)
        {
            double mult = 1;
            if (key.isCtrlPressed() || key.isCmdPressed())
                mult = 3;
            double dMove = deltaKeyboardMove * mult;
            gui::Key::Virtual vk = key.getVirtual();
            switch(vk)
            {
                case gui::Key::Virtual::Left:
                case gui::Key::Virtual::NumLeft:
                {
                    gui::Point delta(-dMove, 0);
                    _pSelectedShape->translate(delta);
                    updatePropertyValues();
                    reDraw();
                }
                    break;
                case gui::Key::Virtual::Right:
                case gui::Key::Virtual::NumRight:
                {
                    gui::Point delta(dMove, 0);
                    _pSelectedShape->translate(delta);
                    updatePropertyValues();
                    reDraw();
                }
                    break;
                case gui::Key::Virtual::Up:
                case gui::Key::Virtual::NumUp:
                {
                    gui::Point delta(0, -dMove);
                    _pSelectedShape->translate(delta);
                    updatePropertyValues();
                    reDraw();
                }
                    break;
                case gui::Key::Virtual::Down:
                case gui::Key::Virtual::NumDown:
                {
                    gui::Point delta(0, dMove);
                    _pSelectedShape->translate(delta);
                    updatePropertyValues();
                    reDraw();
                }
                    break;
                case gui::Key::Virtual::Delete:
                case gui::Key::Virtual::NumDelete:
                    showYesNoQuestionAsync(&_callBackDeleteSelectedShape, tr("DelShape"), tr("SureDelShape"), tr("Yes"), tr("No"));
                    break;
                default:
                    break;
                    
            }
        }
        return true;
    }
    
public:
    EditorView()
    : gui::Canvas({gui::InputDevice::Event::CursorShape, gui::InputDevice::Event::PrimaryClicks, gui::InputDevice::Event::SecondaryClicks, gui::InputDevice::Event::CursorDrag, gui::InputDevice::Event::Zoom, gui::InputDevice::Event::Keyboard})
    , _model(&g_defaultSettings)
    , _callBackDeleteSelectedShape(std::bind(&EditorView::checkDeleteSelectedAnswer, this, std::placeholders::_1))
    {
//        setCursor(gui::Cursor::Type::Default);
    }
    
    const Model* getModel() const
    {
        return &_model;
    }
    
    void save(const td::String& fileName)
    {
        if (_model.save(fileName))
        {
            mu::dbgLog("Shapes saved in file = %s", fileName.c_str());
        }
    }
    
    void load(const td::String& fileName)
    {
        _pSelectedShape = nullptr;
        if (_pPropSwitcher)
        {
            _pPropSwitcher->showView(0);
        }
        if (_model.load(fileName))
        {
            mu::dbgLog("Shapes opened from file = %s", fileName.c_str());
        }
        scale(1.0);
        reDraw();
    }
    
    void setPropSwitcher(gui::PropertyEditorSwitcher* pPropSwitcher)
    {
        _pPropSwitcher = pPropSwitcher;
        if (_pPropSwitcher)
        {
            gui::PropertyEditor* pDefSettings = _pPropSwitcher->getPropertyEditor(0);
            g_defaultSettings.setPropViewer(pDefSettings);
        }
        else
        {
            g_defaultSettings.setPropViewer(nullptr);
        }
    }
    
    void updateCursor() const
    {
        if (IShape2D::currentTool == IShape2D::Tool::Selector)
        {
            setCursor(gui::Cursor::Type::Default);
        }
        else
        {
            setCursor(gui::Cursor::Type::Adder);
        }
    }
    
};
