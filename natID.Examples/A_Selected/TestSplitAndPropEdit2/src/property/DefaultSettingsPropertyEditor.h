//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/ViewSwitcher.h>
#include <gui/PropertyEditor.h>
#include "DefaultSettings.h"

extern DefaultSettings g_defaultSettings;

class DefaultSettingsPropertyEditor : public gui::PropertyEditor
{
protected:
    gui::Properties _properties;
public:
    DefaultSettingsPropertyEditor()
    : gui::PropertyEditor(gui::tr("Defaults"), true)
    {
        g_defaultSettings.createPropertiesForEditor(_properties);
        setProperties(&_properties);
    }
    
};
