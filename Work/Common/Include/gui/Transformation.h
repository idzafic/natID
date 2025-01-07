// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "NatObject.h"

namespace gui
{

class NATGUI_API Transformation : public NatObject
{
protected:
public:
    Transformation();
    virtual ~Transformation();
    
    gui::ObjType getObjType() const override { return ObjType::Transformation; }
    
    //add rotation
    void rotateDeg(double angleDeg);
    void rotateRad(double angleRad);
    //add translation
    void translate(CoordType dx, CoordType dy);
    void translate(const Point& pt);
    //add scaling
    void scale(CoordType sxAndsy);
    void scale(CoordType sx, CoordType sy);
    //set to identity
    void identity();
    //put as last in transformation chai
    void appendToContext() const;
    //set as current transfomatoin matrix -> previous transformations will not be included
    void setToContext() const;
    
    //save current transfomation (graphics) context
    //could be called multiple times
    static void saveContext();
    
    //restore context previously saved with saveContext
    //paired with saveContext
    static void restoreContext();
    
    static void setClip(const gui::Rect& clippingRect);
};

}
