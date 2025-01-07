//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/PropertyEditorSwitcher.h>
#include "DefaultSettingsPropertyEditor.h"
#include "../model/IShape2D.h"

class PropertyEditors : public gui::PropertyEditorSwitcher
{
private:
protected:
    DefaultSettingsPropertyEditor _defaultPropEditor;
    gui::PropertyEditor _rectPropEditor;
    gui::PropertyEditor _roundRectPropEditor;
    gui::PropertyEditor _circPropEditor;
public:
    PropertyEditors()
    : gui::PropertyEditorSwitcher(4)
    , _rectPropEditor(gui::tr("Rectangle"), IShape2D::getProperties(IShape2D::Type::Rect))
    , _roundRectPropEditor(gui::tr("RoundedRect"), IShape2D::getProperties(IShape2D::Type::RoundRect))
    , _circPropEditor(gui::tr("Circle"), IShape2D::getProperties(IShape2D::Type::Circle))
    {
//        setMargins(0, 0, 15, 5);
        addView(&_defaultPropEditor);
        addView(&_rectPropEditor);
        addView(&_roundRectPropEditor);
        addView(&_circPropEditor);        
        setCurrentEditor(0, &g_defaultSettings, true);
    }
    
};
