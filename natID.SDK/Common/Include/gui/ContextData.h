// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <td/String.h>
#include <dp/IDataSet.h>

namespace gui
{

class BaseView;
class Control;

class ContextData
{
public:
    enum class Source : td::BYTE {Unknown = 0, Report, View, Control, TabView, Custom};
    td::String configName; //report configuration name
    td::String layoutName;
    td::String elementName; //report element name or grid name
//    rpt::IViewer* pViewer = nullptr;
    dp::IDataSet* pDP = nullptr;
    
    union
    {
        gui::BaseView* pView = nullptr;
        gui::Control* pControl;
    };
    
    union
    {
        td::UINT4 recNo = 0;
        td::INT4 viewPos;
    };
    Source type = Source::Unknown;
};

}
