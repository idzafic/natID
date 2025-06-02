// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <gui/View.h>
#include <gui/HorizontalLayout.h>
#include <gui/LineEdit.h>
#include <gui/Label.h>
#include <gui/NumericEdit.h>
#include <td/Variant.h>
#include <td/String.h>
#include "natPlotLib.h"

namespace gui
{
namespace plot
{

class NATPLOT_API ElementProperty : public gui::View {
	gui::Label p_name;
	gui::Control* edit;
	bool numeric;
	gui::HorizontalLayout layout;

public:

	ElementProperty(const td::String& name, td::DataType type, const td::String& tooltip = "", td::Variant defaultValue = td::Variant(td::DataType::TD_NONE));

	void setLabelMinSize(int width);
	td::Variant getValue();
	void setValue(const td::Variant &value, bool doAction = false);

	bool onFinishEdit(gui::LineEdit* pCtrl) override;
	std::function<void(const td::Variant &)> Action = nullptr;
	bool onActivate(gui::LineEdit* pCtrl) override;

	~ElementProperty();

};

} //namespace plot
} //namespace gui

