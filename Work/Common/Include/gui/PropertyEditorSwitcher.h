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
#include <gui/ViewSwitcher.h>
#include  <initializer_list>
#include <gui/PropertyEditor.h>


namespace gui
{

class NATGUI_API PropertyEditorSwitcher : public ViewSwitcher
{
public:
    PropertyEditorSwitcher(size_t nPropEditors);
    ~PropertyEditorSwitcher();
    
    void setCurrentEditor(int pos, IProperty* propConsumer, bool bUpdateData);
    void setCanvas(int pos, Canvas* pCanvas);
    void updateValues();
    void updateValues(int pos);
    PropertyEditor* getPropertyEditor(int pos);
};

} //namespace gui
