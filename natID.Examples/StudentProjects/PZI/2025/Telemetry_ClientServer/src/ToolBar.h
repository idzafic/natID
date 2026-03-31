#pragma once
#include <gui/ToolBar.h>
#include <gui/Image.h>

class ToolBar : public gui::ToolBar
{
protected:
    gui::Image imgAddLineGraph;
    gui::Image imgAddTable;
    gui::Image imgAddCollum;
public:
    ToolBar(gui::Image* connection)
        : gui::ToolBar("myToolBar", 7)
        , imgAddLineGraph(":line")
        , imgAddTable(":table")
        , imgAddCollum(":collum")
    {
        addItem("Line Graph", &imgAddLineGraph, "Open new Line Graph", 10, 0, 0, 10);

        addItem("Data Table", &imgAddTable, "Open new Data Table", 10, 0, 0, 20);

        addItem("Column Graph", &imgAddCollum, "Open new Column Graph", 10, 0, 0, 30);

        addSpaceItem();

        addItem("Network Graph", connection, "Open new Network Graph", 10, 0, 0, 40);
    }

};
