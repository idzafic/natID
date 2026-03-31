#pragma once
#include <gui/TabView.h>


class MainView : public gui::TabView{
    int viewPos = 0;
public:

    MainView(): gui::TabView(gui::TabHeader::Type::FitToText, 8, 64)
    {

    }
};
