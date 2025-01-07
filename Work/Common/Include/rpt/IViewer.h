// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once

#include "rptLib.h"
#include <td/String.h>
#include <rpt/ReportData.h>
#include <gui/Image.h>
#include <gui/Image.h>
#include <gui/Symbol.h>
#include <rpt/IResources.h>

namespace rpt
{

class IViewer
{
public:
    virtual const td::String& getConfigName() const = 0;
    virtual void show(const td::String& title, const gui::Image* pRepIcon) = 0;
    virtual void show(const td::String& title, const gui::Symbol* pRepIcon) = 0;
    virtual void getMinMaxZoom(double& minZoom, double& maxZoom) = 0;
    virtual void setMinMaxZoom(double minZoom, double maxZoom) = 0;
    virtual void release() = 0;
    
    NATRPT_API static IViewer* create(const td::String& configName, const ReportData& repData);
    NATRPT_API static void setPlaySoundOnSelectionChange(bool bPlay);
    NATRPT_API static bool shouldPlaySoundOnSelectionChange();
    
    NATRPT_API static void setDefaultZoom(float defaultPortraitZoom, float defaultLandscapeZoom);
    NATRPT_API static void getDefaultZoom(float& defaultPortraitZoom, float& defaultLandscapeZoom);
};


}
