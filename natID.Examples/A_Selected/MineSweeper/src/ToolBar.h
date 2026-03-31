//
//  Created by Izudin Dzafic on 10 Nov 2022.
//  Copyright © 2022 IDz. All rights reserved.
//

#pragma once
#include <gui/ToolBar.h>
#include <gui/Image.h>
#include <gui/Symbol.h>
#include <gui/ComboBox.h>
#include "SymbolsPopoverVerticalWithText.h"

class ToolBar : public gui::ToolBar
{
    gui::Image _settings;
    gui::ComboBox _cmbDifficulty;
    SymbolsPopoverVerticalWithText _poDifficulty;
public:
    ToolBar()
    : gui::ToolBar("mainTB", 2)
    , _settings(":settings")
    {
        reserve(2);
        auto appProps = getAppProperties();
        bool bShowComboInTB = appProps->getValue("showComboInTB", false);
        if (bShowComboInTB)
        {
            _cmbDifficulty.addItem(tr("Easy"));
            _cmbDifficulty.addItem(tr("Medium"));
            _cmbDifficulty.addItem(tr("Hard"));
            _cmbDifficulty.addItem(tr("Restart"));
            _cmbDifficulty.selectIndex(1); //Medium
            addItem(tr("Level"), tr("LevelTT"), &_cmbDifficulty, 100);
        }
        else
        {
            _poDifficulty.transferLabelToButton(false);
            _poDifficulty.setCurrentSelection(1); //Medium
            addItem(&_poDifficulty, 101, tr("Level"), tr("LevelTT"));
        }
        
        addItem(tr("settings"), &_settings, tr("settingsTT"), 10, 0, 0, 10);
    }
};

