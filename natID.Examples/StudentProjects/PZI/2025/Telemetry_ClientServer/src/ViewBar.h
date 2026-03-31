#pragma once
#include <gui/Canvas.h>
#include <gui/Shape.h>
#include <gui/Transformation.h>
#include <gui/DrawableString.h>
#include <gui/ViewScroller.h>
#include <gui/SplitterLayout.h>
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


class ViewBarGraphCanvas : public gui::Canvas{
    std::vector<td::String> types;
    std::vector<double> values;
    int runs;
protected:
    td::ColorID color=td::ColorID::Yellow;
    std::function<void(std::vector<double>)> setInfo;

    double findMax(const std::vector<double>& vec) { //finds the highest value of all the maximums per run for scaling later
        if (vec.empty()) {
            throw std::runtime_error("Vector is empty");
        }

        double maxVal = vec[0];
        for (double value : vec) {
            if (value > maxVal) {
                maxVal = value;
            }
        }

        return maxVal;
    }
    
    void onDraw(const gui::Rect& rect) override{
        gui::Size sz;
        getSize(sz);
        double width=(sz.width-runs*20)/runs; //finds appropariate with for the bars
        double height=(sz.height-100)/findMax(values); //finds a multiplyer to make the bar appropriately tall
        std::vector<gui::Shape> r(values.size());
        for(int i=0; i<values.size(); i++){ //draws solid squares for each value
            mu::dbgLog("%f", values[i]);
            r[i].createRect(gui::Rect(i*width+20, sz.height-values[i]*height, (i+1)*width, sz.height));
            r[i].drawFill(color);
        }
        setInfo(values);
    }

public:
    ViewBarGraphCanvas(std::function<void(std::vector<double>)> FsetInfo)
        : gui::Canvas()
        ,setInfo(FsetInfo)
    {
        enableResizeEvent(true);
        setClipsToBounds();
        registerForScrollEvents();
        types=Data::instance().getTypes();
        changeType(0);
        setInfo(values);
    }

    void changeType(int xid) //sets the type of data being shown and retireves it
    {
        auto types=Data::instance().getTypes();
        dp::IDataSetPtr Data = Data::instance().getDB()->createDataSet("SELECT MAX(m.value) AS max_value FROM measurement m JOIN data_type dt ON dt.id = m.data_type_id JOIN run r ON r.id = m.run_id WHERE dt.name = ? GROUP BY r.id ORDER BY r.number;", dp::IDataSet::Execution::EX_MULT);
        dp::Params par(Data->allocParams());
        td::Variant strVar(td::string8, td::nch, 40);
        strVar.setValue(types[xid]);
        par << strVar;
        double value;
        dp::DSColumns colsX(Data->allocBindColumns(1));
        colsX << "max_value" << value;

        if (!Data->execute())
        {
            //show log
            mu::dbgLog("x");
            Data = nullptr;
            return;
        }
        values.clear();
        runs=Data->getNumberOfRows();
        mu::dbgLog("%d", runs);
        while(Data->moveNext()){
            if(value==0)values.push_back(0);
            else values.push_back(value);

            //mu::dbgLog("%f", value);
        }
        Data=nullptr;
        mu::dbgLog("cx");
        reDraw();
    }

    void changeColor(td::ColorID c){
        color=c;
        reDraw();
    }

};

class ViewGridBar : public gui::View //the grid layout with controlls and to show some of the data from the graph
{
private:
protected:
    gui::Label lbl;
    gui::Label labelColor;
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

    gui::ColorPicker color;

    std::function<void(int)> callChangeType;
    std::function<void(td::ColorID)> callChangeColor;

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
    ViewGridBar(std::function<void(int)> CallChangeType, std::function<void(td::ColorID)> CallChangeColor)
        : lbl("X osa:")
        , labelColor("Line Color")
        , layoutH(3)
        , controlls(6)
        , red1(4),  red2(2), red3(2), red4(2), red5(4), red6(1)
        , callChangeType(CallChangeType)
        , callChangeColor(CallChangeColor)
        , xAndY(2,2)
        , refresh("Refresh")
    {
        findRunNum();
        auto types=Data::instance().getTypes();
        for(int i=0; i<types.size(); i++){
            cbxvalue.addItem(types[i]);
        }

        cbxvalue.selectIndex(0);

        textEdit.setAsReadOnly();

        refresh.setAsDefault();

        color.setValue(td::ColorID::Yellow);

        red1.append(lbl);
        red1.append(cbxvalue);
        red5.append(labelColor);
        red5.append(color);
        red6.append(refresh);


        controlls.append(red1);controlls.append(red2);controlls.append(red3);controlls.append(red4);controlls.append(red5);controlls.append(red6);
        gui::GridComposer gc(xAndY);

        layoutH.append(controlls);
        layoutH.append(xAndY);
        layoutH.append(textEdit);

        setLayout(&layoutH);
    }

    bool onChangedSelection(gui::ComboBox* pCB) override
    {
        if (pCB == &cbxvalue){
            callChangeType(cbxvalue.getSelectedIndex());
            return false;
        }
        return true;
    }

    bool onClick(gui::Button* pB) override
    {
        if(pB==&refresh){
            callChangeColor(color.getValue());
        }
        return true;
    }

    void setInfo(std::vector<double> values){
        textEdit.clean();
        textEdit.appendString("No. of Runs: "+std::to_string(values.size())+"\n");
        for (int i=0; i<values.size(); i++){
            textEdit.appendString("Run "+std::to_string(i)+": "+std::to_string(values[i])+"\n");
        }
    }
};

class ViewBar : public gui::View //makes the grid layout and canvas into a split layout and handles comunication between them
{
    gui::SplitterLayout _splitter;
    std::function<void(std::vector<double>)> fcnsetInfo;
    ViewBarGraphCanvas canvas;
    std::function<void(int)> fcnChangeType;
    std::function<void(td::ColorID)> fcnChangeColor;

    ViewGridBar grid;


public:
    ViewBar():_splitter(gui::SplitterLayout::Orientation::Vertical, gui::SplitterLayout::AuxiliaryCell::Second)
        ,fcnsetInfo(std::bind(&ViewGridBar::setInfo, &grid, std::placeholders::_1))
        ,canvas(fcnsetInfo)
        ,fcnChangeType(std::bind(&ViewBarGraphCanvas::changeType, &canvas, std::placeholders::_1))
        ,fcnChangeColor(std::bind(&ViewBarGraphCanvas::changeColor, &canvas, std::placeholders::_1))
        ,grid(fcnChangeType, fcnChangeColor)
    {
        _splitter.setContent(canvas, grid);
        setLayout(&_splitter);
    }
};
