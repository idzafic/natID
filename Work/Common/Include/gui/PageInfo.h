// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include "PaperInfo.h"
#include <gui/Types.h>


namespace gui
{
	class NATGUI_API PageInfo
	{
        PageInfo(){}
	public:
        gui::Size sizeOfPageInLogicalPixels = {0,0};
	    td::UINT4 numberOfPages = 0;
        PaperInfo paperInfo;
        bool showPrintDialog = true;
        PageInfo(const gui::PaperInfo& paperInfo, gui::Size szOfPageInLgPx)
        : sizeOfPageInLogicalPixels(szOfPageInLgPx)
        , paperInfo(paperInfo)
        {
        }
        void getSizeInMm(gui::Size& sz) const;
        void getPortraitSizeInMm(gui::Size& sz) const;
        static double paperWidth(gui::PaperInfo::Size paperSize, gui::PaperInfo::Orientation paperOrientation);
        
        static double paperHeight(gui::PaperInfo::Size paperSize, gui::PaperInfo::Orientation paperOrientation);
    
        static void getPaperSize(gui::PaperInfo::Size paperSize, gui::PaperInfo::Orientation paperOrientation, gui::Size& sz);

	};
}
