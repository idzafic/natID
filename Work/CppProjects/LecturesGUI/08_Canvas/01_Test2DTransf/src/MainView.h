//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Canvas.h>
#include <gui/Shape.h>
#include <gui/DrawableString.h>
#include <gui/Transformation.h>
#include <gui/Font.h>


class MainView : public gui::Canvas
{
private:
protected:
    gui::Shape _bomber;
    gui::DrawableString _strETF;

protected:
    
    void createShapes()
    {
        gui::Point points[] = {{-65,3}, {-58,5}, {-62,29}, {-53,29},{-44,7},{-7,10.5},{-16,68}, {-11,68}, {2.793, 43},{13,43}, {13,34}, {7.7586, 34}, {21,10}, {49,10}, {59,6},
            {65,0},
            {59, -6}, {49,-10}, {21,-10}, {7.7586, -34}, {13,-34}, {13,-43}, {2.793, -43}, {-11,-68},{-16,-68},{-7,-10.5},{-44,-7},{-53,-29}, {-62,-29}, {-58,-5},{-65,-3}};
        size_t nPoints = sizeof(points)/sizeof(gui::Point);
        _bomber.createPolygon(points, nPoints);
        _strETF = "ETF";
    }
    
public:
    MainView()
    {
        createShapes();
    }
    
    void onDraw(const gui::Rect& rect) override
    {
        gui::Transformation zoomTr;
        double zoom = 1.; //experiment with this and see impact
        double scrollX = 0;
        double scrollY = 0;
        zoomTr.translate(scrollX, scrollY);
        zoomTr.scale(zoom);
        zoomTr.appendToContext(); //this is transformationfor all objects
        
        double rotationDeg = 30;
        {
            gui::Transformation::saveContext();
            gui::Transformation tr;
            tr.translate(200, 200);
            tr.rotateDeg(rotationDeg);
            tr.appendToContext();
            _bomber.drawFillAndWire(td::ColorID::DarkBlue, td::ColorID::SysText);
            gui::Point pt(-1,-5);
            _strETF.draw(pt, gui::Font::ID::SystemNormal, td::ColorID::Yellow); //text will be translated and rotated as well
            gui::Transformation::restoreContext();
        }

        
        //scaled bomber (2xlarger)
        {
            double scaleFactor = 2;
            gui::Transformation::saveContext();
            gui::Transformation tr;
            tr.translate(450, 200);
            tr.rotateDeg(rotationDeg);
            tr.scale(scaleFactor);
            tr.appendToContext();
            _bomber.drawFillAndWire(td::ColorID::DarkBlue, td::ColorID::SysText);
            gui::Point pt(-1,-5);
            _strETF.draw(pt, gui::Font::ID::SystemNormal, td::ColorID::Yellow); //text will be translated and rotated as well
            gui::Transformation::restoreContext();
        }
        
        rotationDeg = 90;
        {
            gui::Transformation::saveContext();
            gui::Transformation tr;
            tr.translate(200, 500);
            tr.rotateDeg(rotationDeg);
            tr.appendToContext();
            _bomber.drawFillAndWire(td::ColorID::DarkBlue, td::ColorID::SysText);
            gui::Point pt(-1,-5);
            _strETF.draw(pt, gui::Font::ID::SystemNormal, td::ColorID::Yellow); //text will be translated and rotated as well
            gui::Transformation::restoreContext();
        }

        
        //scaled bomber (2xlarger)
        {
            double scaleFactor = 2;
            gui::Transformation::saveContext();
            gui::Transformation tr;
            tr.translate(450, 500);
            tr.rotateDeg(rotationDeg);
            tr.scale(scaleFactor);
            tr.appendToContext();
            _bomber.drawFillAndWire(td::ColorID::DarkBlue, td::ColorID::SysText);
            gui::Point pt(-1,-5);
            _strETF.draw(pt, gui::Font::ID::SystemNormal, td::ColorID::Yellow); //text will be translated and rotated as well
            gui::Transformation::restoreContext();
        }
        
        //scaled bomber (3xlarger)
        {
            double scaleFactor = 3;
            gui::Transformation::saveContext();
            gui::Transformation tr;
            tr.translate(850, 500);
            tr.rotateDeg(rotationDeg);
            tr.scale(scaleFactor);
            tr.appendToContext();
            _bomber.drawFillAndWire(td::ColorID::DarkBlue, td::ColorID::SysText);
            gui::Point pt(-1,-5);
            _strETF.draw(pt, gui::Font::ID::SystemNormal, td::ColorID::Yellow); //text will be translated and rotated as well
            gui::Transformation::restoreContext();
        }
    }
    
};
