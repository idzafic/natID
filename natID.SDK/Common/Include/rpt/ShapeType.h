// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ShapeType.h
    @brief Defines enumerations for report container types, shape types, and content types. */
#pragma once

namespace rpt
{
	/// @brief Enumerates the types of containers that can appear in a report layout.
	enum class ContainerType : td::BYTE { CNT_None = 0, CNT_Formula, CNT_Report, CNT_Layout, CNT_Memo, CNT_PageHeader, CNT_DocHeader, CNT_DocFooter, CNT_Header, CNT_Body, CNT_Row, CNT_Group, CNT_Replicable, CNT_Caption, CNT_Table, CNT_Grouper, CNT_GrouperHead, CNT_GrouperFoot, CNT_Footer, CNT_PageFooter } ; ///< No container ///< Formula container ///< Root report container ///< Layout container ///< Memo container ///< Page header container ///< Document header container ///< Document footer container ///< Section header ///< Body container ///< Table row container ///< Group container ///< Replicable container ///< Caption container ///< Table container ///< Grouper container ///< Grouper head container ///< Grouper foot container ///< Section footer ///< Page footer container
	/// @brief Enumerates the types of shapes that can appear inside a report container.
	enum class ShapeType : td::BYTE {SHP_None = 0, SHP_StaticPicture, SHP_ResPicture, SHP_StaticText, SHP_ResText, SHP_ValueText, SHP_Picture, SHP_Text, SHP_Counter, SHP_CompositeText, SHP_TranslatedText, SHP_Numeric, SHP_Date, SHP_Time, SHP_DateTime, SHP_Line, SHP_Rectangle, SHP_FillRectangle, SHP_Aggregate, SHP_Note }; ///< No shape ///< Static picture shape ///< Resource picture shape ///< Static text shape ///< Resource text shape ///< Dynamic value text shape ///< Dynamic picture shape ///< Dynamic text shape ///< Counter shape ///< Composite text shape ///< Translated text shape ///< Numeric value shape ///< Date shape ///< Time shape ///< Date-time shape ///< Line shape ///< Rectangle outline shape ///< Filled rectangle shape ///< Aggregate value shape ///< Note shape
    /// @brief Enumerates the content rendering modes for a report element.
    enum class ContentType : td::BYTE { Content_Normal=0, Content_Repetitive, Content_Replicate, Content_Fall}; ///< Normal single rendering ///< Repetitive rendering across rows ///< Replicated rendering ///< Fallthrough rendering
}
