// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file SpingCtrl.h
    @brief Provide spin control for NumericEdit */
#pragma once
#include "NumericEdit.h"
#include <td/Types.h>

namespace gui
{

/// @brief Base class for GUI controls that are bound to a typed data value.
///
/// DataCtrl extends Control with a data type, control identifier, messaging
/// infrastructure, and decimal/formatting settings shared by all data-entry
/// controls (line edits, combo boxes, sliders, date/time pickers, etc.).
class NATGUI_API Stepper : public Control
{
public:
    enum class Orientation {Horizontal, Vertical};
protected:
    NumericEdit* _pNE;             ///< NumericEdit which will values will be changed by clicking on the spin.
    float _stepSize;              ///< Step size (value of a numeric edit will be indreased/decreased by this step
    Stepper::Orientation _orientation;
    bool _sendMessageOnEachUpdate;  ///< If true, each update will generate a message on NumericEdit

protected:
    /// @brief Measures the preferred size of this control.
    /// @param cellInfo Cell info to populate with size data.
    void measure(CellInfo&) override;

    /// @brief Re-measures the preferred size of this control after a change.
    /// @param cellInfo Cell info to update with new size data.
    void reMeasure(CellInfo&) override;
    
    Stepper(const Stepper&) = delete;
    Stepper& operator =(const Stepper&) = delete;
public:
    /// @brief Constructs a Stepper bound to the numierc edit.
    /// @param step Increase/Decrease step size.
    /// @param orientation Horizontal: '-' and '+' buttons horizontally aligned next to  th pNE, Vertica: small 'up' and 'down' buttons vertically aligned,
    /// @param sendMessageOnEachUpdate If true, messages (data changed) will be generated each time user presses the buttons
    Stepper(NumericEdit* pNE, float step, Stepper::Orientation orientation = Stepper::Orientation::Vertical, bool sendMessageOnEachUpdate = true);
    
    /// @brief Returns the object type identifier for this control.
    /// @return ObjType::SpinCtrl.
    virtual gui::ObjType getObjType() const override { return ObjType::Stepper;}
    
    const NumericEdit* getNumericEdit() const
    {
        return _pNE;
    }
    
    NumericEdit* getNumericEdit()
    {
        return _pNE;
    }
    
    float getStepSize() const
    {
        return _stepSize;
    }
    
    bool isSendingMessageOnEachUpdate() const
    {
        return _sendMessageOnEachUpdate;
    }
    
    Stepper::Orientation getOrientation() const
    {
        return _orientation;
    }
};

}
