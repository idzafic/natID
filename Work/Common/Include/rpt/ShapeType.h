// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once

namespace rpt
{
	enum class ContainerType : td::BYTE { CNT_None = 0, CNT_Formula, CNT_Report, CNT_Layout, CNT_Memo, CNT_PageHeader, CNT_DocHeader, CNT_DocFooter, CNT_Header, CNT_Body, CNT_Row, CNT_Group, CNT_Replicable, CNT_Caption, CNT_Table, CNT_Grouper, CNT_GrouperHead, CNT_GrouperFoot, CNT_Footer, CNT_PageFooter } ;
	enum class ShapeType : td::BYTE {SHP_None = 0, SHP_StaticPicture, SHP_ResPicture, SHP_StaticText, SHP_ResText, SHP_ValueText, SHP_Picture, SHP_Text, SHP_Counter, SHP_CompositeText, SHP_TranslatedText, SHP_Numeric, SHP_Date, SHP_Time, SHP_DateTime, SHP_Line, SHP_Rectangle, SHP_FillRectangle, SHP_Aggregate, SHP_Note };
    enum class ContentType : td::BYTE { Content_Normal=0, Content_Repetitive, Content_Replicate, Content_Fall};
}
