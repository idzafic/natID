
#pragma once
#include <gui/StandardTabView.h>
#include "ViewConv.h"
#include "ViewOptions.h"

class TabView : public gui::StandardTabView
{
private:
protected:
    ViewConv _v1;
    ViewOptions _v2;
    TabView() = delete;
public:
    TabView(sc::IPlugin* pIPlugin, const sc::IPlugin::CallBack& onComplete)
    : _v1(pIPlugin, onComplete)
    {
        _v1.setOptions(&_v2);
        
        addView(&_v1, tr("Converter"));
        addView(&_v2, tr("Options"));
    }
    
    td::String getOutFileName() const
    {
        return _v1.getOutFileName();
    }
    
};
