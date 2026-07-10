// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Property.h
    @brief A labeled property editor widget combining a name label with a value control. */
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

/// @brief A single-row property widget that pairs a label with either a text or numeric
///        edit control, suitable for use in property panels.
class NATPLOT_API ElementProperty : public gui::View {
	gui::Label p_name;       ///< Label displaying the property name.
	gui::Control* edit;      ///< Pointer to the value editor control (LineEdit or NumericEdit).
	bool numeric;            ///< True if the property value is numeric; false for text.
	gui::HorizontalLayout layout; ///< Horizontal layout arranging the label and editor side by side.

public:

	/// @brief Constructs an ElementProperty widget.
	/// @param name Display name shown in the label.
	/// @param type Data type of the property value, determines which editor control is created.
	/// @param tooltip Optional tooltip text shown when hovering over the widget.
	/// @param defaultValue Optional initial value assigned to the editor control.
	ElementProperty(const td::String& name, td::DataType type, const td::String& tooltip = "", td::Variant defaultValue = td::Variant(td::DataType::TD_NONE));

	/// @brief Sets the minimum pixel width of the name label.
	/// @param width Minimum width in pixels.
	void setLabelMinSize(int width);
	/// @brief Returns the current value held by the editor control.
	/// @return A td::Variant containing the current property value.
	td::Variant getValue();
	/// @brief Sets the editor control to the given value, optionally triggering the Action callback.
	/// @param value New value to assign to the editor.
	/// @param doAction If true, the Action callback is invoked after setting the value.
	void setValue(const td::Variant &value, bool doAction = false);

	/// @brief Called when the user finishes editing the text control.
	/// @param pCtrl Pointer to the LineEdit that was edited.
	/// @return True if the input was accepted and handled.
	bool onFinishEdit(gui::LineEdit* pCtrl) override;
	std::function<void(const td::Variant &)> Action = nullptr; ///< Optional callback invoked when the property value changes.
	/// @brief Called when the user activates (presses Enter in) the text control.
	/// @param pCtrl Pointer to the LineEdit that was activated.
	/// @return True if the activation event was handled.
	bool onActivate(gui::LineEdit* pCtrl) override;

	/// @brief Destructor. Releases the dynamically allocated editor control.
	~ElementProperty();

};

} //namespace plot
} //namespace gui
