//
//  Created by Izudin Dzafic on 18/10/2025.
//  Copyright © 2025 IDz. All rights reserved.
//
#include "ToolBar.h"

ToolBar::ToolBar(ViewChessBoard* pViewChessBoard, gui::Image* imgRun)
: gui::ToolBar("mainTB", 2)
, _viewSettings(pViewChessBoard, this)
, _imgReset(":reset")
{
    addItem(&_viewSettings, 1000, tr("settings"), tr("settingsTT"));
    addItem(tr("reseet"), &_imgReset, tr("resetTT"), cMenuGame, 0, 0, cResetActionItem);
    addItem(tr("start"), imgRun, tr("startTT"), cMenuGame, 0, 0, cStartStopActionItem);
    
}
