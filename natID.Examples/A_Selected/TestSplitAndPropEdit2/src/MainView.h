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
    EditorView _editorView;
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
        _splitter.setContent(_editorView, _propEditors);
        setLayout(&_splitter);
//        EditorView* pEditorView = _viewScroller.getEditor();
        _editorView.setPropSwitcher(&_propEditors);
        _propEditors.setCanvas(1, &_editorView);
        _propEditors.setCanvas(2, &_editorView);
        _propEditors.setCanvas(3, &_editorView);
    }
    
    EditorView* getEditor()
    {
        return &_editorView;
    }
};
