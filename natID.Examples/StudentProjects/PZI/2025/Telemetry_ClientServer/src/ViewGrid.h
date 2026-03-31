#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/TextEdit.h>
#include <gui/NumericEdit.h>
#include <gui/Slider.h>
#include <gui/ComboBox.h>
#include <gui/ColorPicker.h>
#include <gui/LinePatternPicker.h>
#include <gui/Button.h>

#include <gui/HorizontalLayout.h>
#include <gui/VerticalLayout.h>

#include <gui/GridLayout.h>
#include <gui/GridComposer.h>

#include "Data.h"

class ViewGrid : public gui::View
{
private:
protected:
    gui::Label lbl;
    gui::Label lbl2;
    gui::Label lblKol;
    gui::NumericEdit kol;
    gui::Label labelSlid;
    gui::Label labelx;
    gui::NumericEdit numx;
    gui::NumericEdit numy;
    gui::Label labely;
    gui::Label labelThickness;
    gui::Label labelColor;
    gui::Label labelPattern;
    gui::Button refresh;

    gui::HorizontalLayout layoutH;
    gui::VerticalLayout controlls;
    gui::HorizontalLayout red1;
    gui::HorizontalLayout red2;
    gui::HorizontalLayout red3;
    gui::HorizontalLayout red4;
    gui::HorizontalLayout red5;
    gui::HorizontalLayout red6;
    gui::GridLayout xAndY;

    gui::TextEdit textEdit;

    gui::ComboBox cbxvalue;
    gui::ComboBox cbyvalue;
    gui::ComboBox cbrun;

    gui::Slider xscale;
    gui::Slider thickness;
    gui::ColorPicker color;
    gui::LinePatternPicker pattern;

    std::function<void(int)> callChangeX;
    std::function<void(int)> callChangeY;
    std::function<void(int)> callChangeRun;
    std::function<void(double)> callChangeScale;
    std::function<void(double)> callChangeThickness;
    std::function<void(td::ColorID)> callChangeColor;
    std::function<void(td::LinePattern)> callChangePattern;

    int runs;

    void findRunNum(){
        dp::IDataSetPtr Runs = Data::instance().getDB()->createDataSet("SELECT MAX(id) FROM run;", dp::IDataSet::Execution::EX_MULT);
        int value;
        dp::DSColumns colsX(Runs->allocBindColumns(1));
        colsX << "MAX(id)" << value;
        if (!Runs->execute())
        {
            //show log
            mu::dbgLog("a");
            Runs = nullptr;
            return;

        }
        Runs->moveNext();
        runs=value;
    }

public:
    ViewGrid(std::function<void(int)> CallChangeX, std::function<void(int)> CallChangeY,
             std::function<void(int)> CallChangeRun, std::function<void(double)> CallChangeScale,
             std::function<void(double)> CallChangeThickness, std::function<void(td::ColorID)> CallChangeColor,
             std::function<void(td::LinePattern)> CallChangePattern)
        : lbl("X osa:")
        , lbl2("Y osa:")
        , lblKol("Run:")
        , labelSlid("X Scale:")
        , labelx("x:")
        , numx(td::decimal3, gui::LineEdit::Messages::Send)
        , labely("y:")
        , numy(td::decimal3, gui::LineEdit::Messages::Send)
        , labelThickness("Line Thickness")
        , labelColor("Line Color")
        , labelPattern("Line Pattern")
        , kol(td::decimal3, gui::LineEdit::Messages::Send)
        , layoutH(3)
        , controlls(6)
        , red1(4),  red2(2), red3(2), red4(2), red5(4), red6(1)
        , callChangeX(CallChangeX)
        , callChangeY(CallChangeY)
        , callChangeRun(CallChangeRun)
        , callChangeScale(CallChangeScale)
        , callChangeThickness(CallChangeThickness)
        , callChangeColor(CallChangeColor)
        , callChangePattern(CallChangePattern)
        , xAndY(2,2)
        , refresh("Refresh")
    {
        findRunNum();
        auto types=Data::instance().getTypes();
        for(int i=0; i<types.size(); i++){
            cbxvalue.addItem(types[i]);
            cbyvalue.addItem(types[i]);
        }

        for(int i=0; i<runs; i++){
            cbrun.addItem(std::to_string(i+1));
        }
        numx.setAsReadOnly();
        numy.setAsReadOnly();
        cbxvalue.selectIndex(0);
        cbyvalue.selectIndex(1);

        xscale.setRange(-100,100);
        xscale.setValue(1);

        thickness.setRange(1, 5);
        thickness.setValue(2);

        textEdit.setAsReadOnly();

        refresh.setAsDefault();

        color.setValue(td::ColorID::Yellow);

        red1.append(lbl);
        red1.append(cbxvalue);
        red1.append(lbl2);
        red1.append(cbyvalue);
        red2.append(lblKol);
        red2.append(cbrun);
        red3.append(labelSlid);
        red3.append(xscale);
        red4.append(labelThickness);
        red4.append(thickness);
        red5.append(labelColor);
        red5.append(color);
        red5.append(labelPattern);
        red5.append(pattern);
        red6.append(refresh);


        controlls.append(red1);controlls.append(red2);controlls.append(red3);controlls.append(red4);controlls.append(red5);controlls.append(red6);
        gui::GridComposer gc(xAndY);
        gc.appendRow(labelx)<<numx;
        gc.appendRow(labely)<<numy;

        layoutH.append(controlls);
        layoutH.append(xAndY);
        layoutH.append(textEdit);

        setLayout(&layoutH);
    }
    
    bool onChangedValue(gui::Slider* pSlider) override  //calls for the scale or thickness to be changed as the sliders are being moved
    {
        if (pSlider == &xscale)
        {
            double val = xscale.getValue();
            callChangeScale((val >= 0) ? (1.0 + (val / 100.0) * (100.0 - 1.0)): (1.0 + (val / -100.0) * (0.01 - 1.0)));
            return true;
        }
        else if (pSlider == &thickness){
            callChangeThickness(thickness.getValue());
            return true;
        }
        return false;
    }
    
    bool onChangedSelection(gui::ComboBox* pCB) override    //changes the x or y axis or the run being displayed
    {
        if (pCB == &cbxvalue){
            callChangeX(cbxvalue.getSelectedIndex());
            return false;
        }
        else if(pCB == &cbyvalue){
            callChangeY(cbyvalue.getSelectedIndex());
            return false;
        }
        else if(pCB == &cbrun){
            callChangeRun(cbrun.getSelectedIndex()+1);
            return false;
        }
        return true;
    }

    bool onClick(gui::Button* pB) override  //refreshes color and line pattern to the current selection
    {
        if(pB==&refresh){
            callChangePattern(pattern.getValue());
            callChangeColor(color.getValue());
        }
        return true;
    }

    void setXY(td::Decimal3 x, td::Decimal3 y){ //sets the values of the numeric edit boxes to show the value at mouse position
        numx.setValue(x);
        numy.setValue(y);
    }

    void setInfo(double xmax, double xmin, double ymax, double ymin, int run){ //sets the contents of the text edit to show some data about the graph
        textEdit.clean();
        textEdit.appendString("X max: "+std::to_string(xmax)+"\nX min: "+std::to_string(xmin));
        textEdit.appendString("\n");
        textEdit.appendString("Y max: "+std::to_string(ymax)+"\nY min: "+std::to_string(ymin));
        textEdit.appendString("\n");
        textEdit.appendString("Run: "+std::to_string(run));
    }
};
