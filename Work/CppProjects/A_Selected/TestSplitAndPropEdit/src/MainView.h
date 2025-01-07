//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/SplitterLayout.h>
#include "EditorViewScroller.h"
#include "property/PropertyEditors.h"
#include "property/DefaultSettingsPropertyEditor.h"
#include "property/PropertyEditors.h"


class MainView : public gui::View
{
    
protected:
    gui::SplitterLayout _splitter;
    EditorViewScroller _viewScroller;
    //DefaultSettingsPropertyEditor _propertyEditorForDefaultSettings;
    PropertyEditors _propEditors;
public:
    MainView()
    : _splitter(gui::SplitterLayout::Orientation::Horizontal, gui::SplitterLayout::AuxiliaryCell::Second)
    {
        IShape2D::initProperties();
        
        //_propertyEditorForDefaultSettings.setPropertyConsumer(&g_defaultSettings);
        _splitter.setToolTipForMinimizedAux("Property Editor");
        setMargins(0, 0, 0, 0);
        _splitter.setContent(_viewScroller, _propEditors);
        setLayout(&_splitter);
        EditorView* pEditorView = _viewScroller.getEditor();
        pEditorView->setPropSwitcher(&_propEditors);
        _propEditors.setCanvas(1, pEditorView);
        _propEditors.setCanvas(2, pEditorView);
        _propEditors.setCanvas(3, pEditorView);
    }
    
    EditorView* getEditor()
    {
        return _viewScroller.getEditor();
    }
};
