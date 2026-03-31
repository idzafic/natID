#pragma once
#include <vector>
#include <memory>
#include <gui/Canvas.h>
#include <gui/Shape.h>
#include <gui/DrawableString.h>
#include <dp/IDatabase.h>
#include <dp/IDataSet.h>

class NetLineGraph{ //same as the LineGraph class but doesnt have the setX and setY methods and instead
                    //just has one method to add points to the graph one by one as they are recieved
    double xmax=0,xmin=0,ymax=0,ymin=0;

    std::vector <std::shared_ptr<gui::Shape>>graf;

    std::vector<double> y;
    std::vector<double> x;

    td::String xOsa="x";
    td::String yOsa="y";
    double thickness=2.0;
public:
    NetLineGraph(double thickness=2.0):thickness(thickness)
    {
    }

    std::vector <std::shared_ptr<gui::Shape>> &Graph(){return graf;}

    void drawCoordinates(const gui::Size& sz, int zeroPos, double xmult=1)
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
        double zeroPosition=mapValues(0, sz);
        drawCoordinates(sz, zeroPosition, xmult);
        if (y.size()>1){
            for(int i=1; i<y.size();i++){
                graf.push_back(std::make_shared<gui::Shape>());
                graf[i-1]->createBezier(thickness,pattern).moveTo({x[i-1]*xmult+5,
                mapValues(y[i-1], sz)}).lineTo({x[i]*xmult+5,mapValues(y[i], sz)});
            }

            for(int i=0; i<graf.size();i++){
                graf[i]->drawWire(color);
            }
        }
    }

    double mapValues(double y, const gui::Size& sz){

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

    td::String getXname()const{return xOsa;}
    td::String getYname()const{return yOsa;}

    void appendPoint(double x_novi, double y_novi){
        x.push_back(x_novi);
        y.push_back(y_novi);
        if(x_novi>xmax)xmax=x_novi;
        else if(x_novi<xmin)xmin=x_novi;
        if(y_novi>ymax)ymax=y_novi;
        else if(y_novi<ymin)ymin=y_novi;
    }

};
