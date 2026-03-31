//
//  Created by Izudin Dzafic on 18/10/2025.
//  Copyright © 2025 IDz. All rights reserved.
//

#pragma once
#include <gui/ToolBar.h>
#include <gui/PopupView.h>
#include <gui/Symbol.h>
#include "Constants.h"
#include "ViewSettings.h"
//#include "ViewChessBoard.h"

class ViewChessBoard;

class ToolBar : public gui::ToolBar
{
    ViewSettings _viewSettings;
    gui::Image _imgReset;
public:
    ToolBar(ViewChessBoard* pViewChessBoard, gui::Image* imgRun);
};

