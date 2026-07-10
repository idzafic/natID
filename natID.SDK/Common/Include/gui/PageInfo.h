// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file PageInfo.h @brief Provides page information (size, paper, number of pages) used for printing and layout. */
#pragma once
#include "PaperInfo.h"
#include <gui/Types.h>


namespace gui
{
/// @brief Holds information about a printed or displayed page, including size, paper settings, and page count.
	class NATGUI_API PageInfo
	{
        PageInfo(){}
	public:
        gui::Size sizeOfPageInLogicalPixels = {0,0}; ///< Page size expressed in logical pixels.
	    td::UINT4 numberOfPages = 0; ///< Total number of pages.
        PaperInfo paperInfo; ///< Paper type and orientation information.
        bool showPrintDialog = true; ///< Whether to show the print dialog before printing.

        /// @brief Constructs a PageInfo with the given paper info and page size.
        /// @param paperInfo Paper type and orientation to use.
        /// @param szOfPageInLgPx Page size in logical pixels.
        PageInfo(const gui::PaperInfo& paperInfo, gui::Size szOfPageInLgPx)
        : sizeOfPageInLogicalPixels(szOfPageInLgPx)
        , paperInfo(paperInfo)
        {
        }

        /// @brief Returns the page size in millimetres.
        /// @param sz Output size in millimetres.
        void getSizeInMm(gui::Size& sz) const;

        /// @brief Returns the portrait-orientation page size in millimetres.
        /// @param sz Output size in millimetres (portrait orientation).
        void getPortraitSizeInMm(gui::Size& sz) const;

        /// @brief Returns the paper width in millimetres for a given paper size and orientation.
        /// @param paperSize The paper size enumeration value.
        /// @param paperOrientation The paper orientation.
        /// @return Paper width in millimetres.
        static double paperWidth(gui::PaperInfo::Size paperSize, gui::PaperInfo::Orientation paperOrientation);

        /// @brief Returns the paper height in millimetres for a given paper size and orientation.
        /// @param paperSize The paper size enumeration value.
        /// @param paperOrientation The paper orientation.
        /// @return Paper height in millimetres.
        static double paperHeight(gui::PaperInfo::Size paperSize, gui::PaperInfo::Orientation paperOrientation);

        /// @brief Fills a Size structure with the paper dimensions in millimetres.
        /// @param paperSize The paper size enumeration value.
        /// @param paperOrientation The paper orientation.
        /// @param sz Output size in millimetres.
        static void getPaperSize(gui::PaperInfo::Size paperSize, gui::PaperInfo::Orientation paperOrientation, gui::Size& sz);

	};
}
