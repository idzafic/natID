//
//  Created by Izudin Dzafic on 16/12/2023.
//  Copyright © 2023 IDz. All rights reserved.
//
#pragma once
#include <gui/TabView.h>
#include "BackView.h"

class DynTabView : public gui::TabView
{
private:
	BackView _backgroundView;
protected:
public:
    DynTabView()
	: gui::TabView(gui::TabHeader::Type::FitToText, 8, 64)
    {
		setBackgroundView(&_backgroundView);
    }
};
