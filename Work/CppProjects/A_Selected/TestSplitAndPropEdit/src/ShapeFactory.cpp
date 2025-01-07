
#include "model/Circle.h"
#include "model/Rectangle.h"
#include "model/RoundedRectangle.h"
#include <gui/Properties.h>


IShape2D::Tool IShape2D::currentTool = IShape2D::Tool::Selector;
float IShape2D::maxLineWidth = 20;
gui::CoordType IShape2D::selectionDisance2 = 5*5;
gui::CoordType IShape2D::refreshOffset = 2;

gui::Properties IShape2D::_circProperties;
gui::Properties IShape2D::_rectPropertes;
gui::Properties IShape2D::_roundRectPropertes;
static cnt::SafeFullVector<td::String> s_attribStrings;

IShape2D* IShape2D::createRect(gui::Shape::Attribs attribs, const gui::Rect& rect, td::ColorID fillColor, td::ColorID lineColor, float lineWidth, td::LinePattern linePattern)
{
    Rectangle* pShape = new Rectangle(attribs, rect, fillColor, lineColor, lineWidth, linePattern);
    pShape->init();
    return pShape;
}

IShape2D* IShape2D::createRoundedRect(gui::Shape::Attribs attribs, const gui::Rect& r, float radius, td::ColorID fillColor, td::ColorID lineColor, float lineWidth, td::LinePattern linePattern)
{
    RoundedRectangle* pShape = new RoundedRectangle(attribs, r, radius, fillColor, lineColor, lineWidth, linePattern);
    pShape->init();
    return pShape;
}

IShape2D* IShape2D::createCircle(gui::Shape::Attribs attribs, const gui::Point& center, float radius, td::ColorID fillColor, td::ColorID lineColor, float lineWidth, td::LinePattern linePattern)
{
    Circle* pShape =  new Circle(attribs, center, radius, fillColor, lineColor, lineWidth, linePattern);
    pShape->init();
    return pShape;
}

gui::Properties* IShape2D::getProperties(IShape2D::Type shapeType)
{
    initProperties();
    switch(shapeType)
    {
        case Type::Circle: return &_circProperties;
        case Type::Rect: return &_rectPropertes;
        case Type::RoundRect: return &_roundRectPropertes;
    default:
        assert(false);
    }
    return nullptr;
}

void IShape2D::initProperties()
{
    if (_rectPropertes.size() > 0)
        return;
    
    {
        //create dummy Rectangle on stack and init properties
        gui::Rect r(0,0,0,0);
        
        Rectangle sh(gui::Shape::Attribs::FillOnly, r, td::ColorID::SysText, td::ColorID::SysText, 1, td::LinePattern::Solid);
        _rectPropertes.reserve(11);
        sh.initProperties(&_rectPropertes);
    }
    
    {
        //create dummy RoundedRectangle on stack and init properties
        gui::Rect r(0,0,0,0);
        
        RoundedRectangle sh(gui::Shape::Attribs::FillOnly, r, 0, td::ColorID::SysText, td::ColorID::SysText, 1, td::LinePattern::Solid);
        _roundRectPropertes.reserve(12);
        sh.initProperties(&_roundRectPropertes);
    }
    
    {
        //create dummy Circle on stack and init properties
        gui::Point pt(0,0);
        
        Circle sh(gui::Shape::Attribs::FillOnly, pt, 0, td::ColorID::SysText, td::ColorID::SysText, 1, td::LinePattern::Solid);
        _circProperties.reserve(10);
        sh.initProperties(&_circProperties);
    }
}

td::String* IShape2D::getAttribsDesc()
{
    if (s_attribStrings.size() == 0)
    {
        s_attribStrings.reserve(3);
        s_attribStrings[0] = "Line only";
        s_attribStrings[1] = "Fill only";
        s_attribStrings[2] = "Line & fill";
    }
    
    return s_attribStrings.begin();
}
