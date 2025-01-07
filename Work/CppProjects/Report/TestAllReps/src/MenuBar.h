//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/MenuBar.h>

class MenuBar : public gui::MenuBar
{
private:
    gui::SubMenu _subApp;
protected:
    void populateSubAppMenu()
    {
        auto& items = _subApp.getItems();

        items[0].initAsActionItem(tr("RacunV1"), 10);
        items[1].initAsActionItem(tr("RacunV2"), 20);
        items[2].initAsActionItem(tr("RacunV3"), 30);
        items[3].initAsActionItem(tr("RacunV4"), 40);
        items[4].initAsActionItem(tr("RacunV5.1"), 50);
        items[5].initAsActionItem(tr("RacunV5.2"), 60);
        items[6].initAsActionItem(tr("RacunParallelTables"), 70);

        items[7].initAsActionItem(tr("KarticaReport1"), 110);
        items[8].initAsActionItem(tr("KarticaReport4"), 120);
        items[9].initAsActionItem(tr("LandScapeWithFormulaTest"), 123);
        items[10].initAsActionItem(tr("Port2LandscapeRotation"), 124);
        items[11].initAsActionItem(tr("MultiLayout-1"), 125);
        items[12].initAsActionItem(tr("MultiLayout-2"), 126);
        items[13].initAsActionItem(tr("MultiLayout-3"), 127);
        items[14].initAsActionItem(tr("MultiLayout-4"), 128);

        items[15].initAsActionItem(tr("KarticaMultiReport"), 130);
        items[16].initAsActionItem(tr("KarticaMultiReport2"), 131);
        items[17].initAsActionItem(tr("KarticaMultiReport_WF"), 135);

        items[18].initAsActionItem(tr("ItemsForStoreWindow6x3"), 140);
        items[19].initAsActionItem(tr("ItemsForStoreWindow7x3"), 150);
        items[20].initAsActionItem(tr("ShowWaterfallSimple"), 170);
        items[21].initAsActionItem(tr("ShowWaterfallAdvanced"), 180);
        items[22].initAsActionItem(tr("ShowWaterfallAdvanced2"), 190);
        items[23].initAsActionItem(tr("DynanicBodyAndContextMenus"), 200);

    }

    
public:
    MenuBar()
    : gui::MenuBar(1) 
    , _subApp(10, "Izvjestaji", 24)
    {
        populateSubAppMenu();
        _menus[0] = &_subApp;

    }
    
    ~MenuBar()
    {
    }
};

