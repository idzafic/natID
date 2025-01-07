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
#include "DataCtrl.h"

namespace gui
{
    class NATGUI_API BaseComboBox : public DataCtrl
	{
    protected:
        void measure(CellInfo&) override;
        void reMeasure(CellInfo&) override;
        
        BaseComboBox(td::DataType dt);
        BaseComboBox();
        
//        void removeItem(int pos);
    public:
        ~BaseComboBox();
        void selectIndex(int index);
        int getSelectedIndex() const;
        td::String getSelectedText() const;
        int getNoOfItems() const;
        gui::CoordType getWidthToFitLongestItem() const;
        void sizeToFit() override;
        
	};
} //namespace gui
