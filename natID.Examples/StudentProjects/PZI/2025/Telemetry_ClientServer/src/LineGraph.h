#pragma once
#include <vector>
#include <memory>
#include <gui/Canvas.h>
#include <gui/Shape.h>
#include <gui/DrawableString.h>
#include <dp/IDatabase.h>
#include <dp/IDataSet.h>

#include"Data.h"


class LineGraph{

    int x_id=0; //the id-s of x, y, and the run that is currently being displayed
    int y_id=0;
    int run_id=1;

    double xmax=0,xmin=0,ymax=0,ymin=0; //highest and lowest values on the x and y axis

    std::vector <std::shared_ptr<gui::Shape>>graf; //vector of all the lines for the graph

    std::vector<double> y; //vectors with the values of x and y
    std::vector<double> x;

    td::String xOsa="x";  //the names of the types of data on the x and y axis
    td::String yOsa="y";
public:
    LineGraph()
    {
        setX(0);
        setY(0);
    }
    std::vector <std::shared_ptr<gui::Shape>> &Graph(){return graf;} //returns a reference to the graph for use in other classes

    void drawCoordinates(const gui::Size& sz, int zeroPos, double xmult=1) //draws the x and y axis of the graph
    {
        const gui::CoordType c_dx = 0;
        gui::CoordType arrH = 6;
        gui::CoordType arrW = 10;
        //x osa
        gui::Point p1(c_dx, zeroPos);
        gui::Point p2(getXmax()*xmult, zeroPos);
        gui::Shape::drawLine(p1, p2, td::ColorID::SysText, 0);
        //strelica x
        p1.x = p2.x - arrW;
        p1.y = p2.y - arrH;
        gui::Shape::drawLine(p1, p2, td::ColorID::SysText, 0);
        p1.y = p2.y + arrH;
        gui::Shape::drawLine(p1, p2, td::ColorID::SysText, 0);
        p1.x = p2.x - 2*arrW;
        gui::DrawableString(xOsa).draw(p1, gui::Font::ID::SystemLargest, td::ColorID::SysText);

        //y osa
        p1.x = 5;
        p1.y = c_dx;
        p2.x = p1.x;
        p2.y = sz.height;
        gui::Shape::drawLine(p1, p2, td::ColorID::SysText, 0);

        //strelica y
        p2.x = p1.x + arrH;
        p2.y = p1.y + arrW;
        gui::Shape::drawLine(p1, p2, td::ColorID::SysText, 0);
        p2.x = p1.x - arrH;
        gui::Shape::drawLine(p1, p2, td::ColorID::SysText, 0);
        p2.x += 3*arrW;
        p2.y = p1.y + 3.5*arrW;
        gui::DrawableString(yOsa).draw({p1.x+5,p1.y+5}, gui::Font::ID::SystemLargest, td::ColorID::SysText);
    }

    void Draw(const gui::Size& sz, double xmult=1, double thickness=1, td::ColorID color=td::ColorID::Yellow, td::LinePattern pattern=td::LinePattern::Solid){
        graf.clear();
        double zeroPosition=mapValues(0, sz); //finds where '0' should be in regards to the rest of the data to determine where the x axis should be drawn
        drawCoordinates(sz, zeroPosition, xmult);
        for(int i=1; i<y.size();i++){
            graf.push_back(std::make_shared<gui::Shape>());                         //creates a vector of all the lines needed for the
            graf[i-1]->createBezier(thickness,pattern).moveTo({x[i-1]*xmult+5,      //graph with the values properly mapped to fit the
            mapValues(y[i-1], sz)}).lineTo({x[i]*xmult+5,mapValues(y[i], sz)});     //size of the canvas they are being drawn to
        }

        for(int i=0; i<graf.size();i++){
            graf[i]->drawWire(color);       //draws each of the lines
        }
    }

    double mapValues(double y, const gui::Size& sz){        //mapps values on the y axis to fit the size of the canvas and flips them
                                                            //because canvas coordinates start from (0,0) in the top left
        double maxVal=getYmax();
        double minVal=getYmin();
        double B=sz.height;
        double range = maxVal - minVal;
        double outMin = (maxVal <= 0) ? 25 : 0.0;
        double outMax = (minVal >= 0) ? (B - 25) : B;
        double mapped = outMin + (outMax - outMin) * (maxVal - y) / range;
        return (y == 0.0) ?((minVal > 0) ? outMax :(maxVal < 0) ? outMin : mapped): mapped;
    }


    int getXnum()const{return x.size();}
    int getYnum()const{return y.size();}

    double getXmax()const{return xmax;}
    double getYmax()const{return ymax;}
    double getXmin()const{return xmin;}
    double getYmin()const{return ymin;}

    double getRun()const{return run_id;}

    td::String getXname()const{return xOsa;}
    td::String getYname()const{return yOsa;}

    void setRun(int r){
        run_id=r;
        setX(x_id);         //changes the run_id
        setY(y_id);
    }

    void setX(int xid){     //takes all the values for x for a specific run from the database and populates the vector of data for it
        x_id=xid;
        auto types=Data::instance().getTypes();
        xOsa=types[xid];
        dp::IDataSetPtr xData = Data::instance().getDB()->createDataSet("SELECT m.value FROM measurement m, data_type dt WHERE dt.id=m.data_type_id AND dt.name=? AND m.run_id=?;", dp::IDataSet::Execution::EX_MULT);
        dp::Params par(xData->allocParams());
        td::Variant strVar(td::string8, td::nch, 40);
        strVar.setValue(xOsa);
        par << strVar<<run_id;
        double value;
        dp::DSColumns colsX(xData->allocBindColumns(1));
        colsX << "value" << value;

        if (!xData->execute())
        {
            //show log
            mu::dbgLog("x");
            xData = nullptr;
            return;
        }
        x.clear();
        while(xData->moveNext()){
            x.push_back(value);
        }

        setMaxX();
        setMinX();
        xData=nullptr;
    }
    void setY(int yid){     //takes all the values for y for a specific run from the database and populates the vector of data for it
        y_id=yid;
        auto types=Data::instance().getTypes();
        yOsa=types[yid];
        dp::IDataSetPtr yData = Data::instance().getDB()->createDataSet("SELECT m.value FROM measurement m, data_type dt WHERE dt.id=m.data_type_id AND dt.name=? AND m.run_id=?;", dp::IDataSet::Execution::EX_MULT);
        dp::Params par(yData->allocParams());
        td::Variant strVar(td::string8, td::nch, 40);
        strVar.setValue(yOsa);
        par << strVar<<run_id;
        double value;
        dp::DSColumns colsY(yData->allocBindColumns(1));
        colsY << "value" << value;

        if (!yData->execute())
        {
            //show log
            mu::dbgLog("y");
            yData = nullptr;
            return;
        }


        y.clear();
        while(yData->moveNext()){
            y.push_back(value);
        }

        setMaxY();
        setMinY();
        yData=nullptr;
    }

    void setMaxX(){  //finds and stores the highest value of x for lates use so it doesnt need to be looked for every time its needed
        dp::IDataSetPtr xMax = Data::instance().getDB()->createDataSet("SELECT MAX(m.value) FROM measurement m, data_type dt WHERE dt.id=m.data_type_id AND dt.name=? AND m.run_id=?;", dp::IDataSet::Execution::EX_MULT);
        dp::Params par(xMax->allocParams());
        td::Variant strVar(td::string8, td::nch, 40);
        strVar.setValue(xOsa);
        par << strVar<<run_id;
        double value;
        dp::DSColumns colsX(xMax->allocBindColumns(1));
        colsX << "MAX(m.value)" << value;
        if (!xMax->execute())
        {
            //show log
            mu::dbgLog("xmx");
            xMax = nullptr;
            return;
        }
        xMax->moveNext();
        xmax=value;
        xMax = nullptr;
    }
    void setMinX(){     //finds and stores the lowes value of x for lates use so it doesnt need to be looked for every time its needed
        dp::IDataSetPtr xMin = Data::instance().getDB()->createDataSet("SELECT MIN(m.value) FROM measurement m, data_type dt WHERE dt.id=m.data_type_id AND dt.name=? AND m.run_id=?;", dp::IDataSet::Execution::EX_MULT);
        dp::Params par(xMin->allocParams());
        td::Variant strVar(td::string8, td::nch, 40);
        strVar.setValue(xOsa);
        par << strVar<<run_id;
        double value;
        dp::DSColumns colsX(xMin->allocBindColumns(1));
        colsX << "MIN(m.value)" << value;
        if (!xMin->execute())
        {
            //show log
            mu::dbgLog("xmi");
            xMin = nullptr;
            return;
        }
        xMin->moveNext();
        xmin=value;
        xMin = nullptr;
    }

    void setMaxY(){     //finds and stores the highest value of y for lates use so it doesnt need to be looked for every time its needed
        dp::IDataSetPtr yMax = Data::instance().getDB()->createDataSet("SELECT MAX(m.value) FROM measurement m, data_type dt WHERE dt.id=m.data_type_id AND dt.name=? AND m.run_id=?;", dp::IDataSet::Execution::EX_MULT);
        dp::Params par(yMax->allocParams());
        td::Variant strVar(td::string8, td::nch, 40);
        strVar.setValue(yOsa);
        par << strVar<<run_id;
        double value;
        dp::DSColumns colsY(yMax->allocBindColumns(1));
        colsY << "MAX(m.value)" << value;
        if (!yMax->execute())
        {
            //show log
            mu::dbgLog("ymx");
            yMax = nullptr;
            return;
        }
        yMax->moveNext();
        ymax=value;
        yMax = nullptr;
        mu::dbgLog("%f", ymax);
    }
    void setMinY(){     //finds and stores the lowest value of y for lates use so it doesnt need to be looked for every time its needed
        dp::IDataSetPtr yMin = Data::instance().getDB()->createDataSet("SELECT MIN(m.value) FROM measurement m, data_type dt WHERE dt.id=m.data_type_id AND dt.name=? AND m.run_id=?;", dp::IDataSet::Execution::EX_MULT);
        dp::Params par(yMin->allocParams());
        td::Variant strVar(td::string8, td::nch, 40);
        strVar.setValue(yOsa);
        par << strVar<<run_id;
        double value;
        dp::DSColumns colsY(yMin->allocBindColumns(1));
        colsY << "MIN(m.value)" << value;
        if (!yMin->execute())
        {
            //show log
            mu::dbgLog("ymi");
            yMin = nullptr;
            return;
        }
        yMin->moveNext();
        ymin=value;
        yMin = nullptr;
        mu::dbgLog("%f", ymin);
    }
};
