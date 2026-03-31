#include "../../common/include/Rect.h"

Rect::Rect(const TdRect& r, td::ColorID fillColor, td::ColorID lineColor, td::LinePattern lp)
: Shape(fillColor, lineColor, lp)
, _rect(r)
{
}

Rect::Rect(){}

Shape::Type Rect::getType() const
{
    return Shape::Type::Rect;
}

void Rect::show(std::ostream& o) const
{
    o << "Rect data:";
    _rect.show(o);
    
    Shape::show(o);
}

void Rect::save(arch::ArchiveOut& aOut) const
{
    aOut << _rect;
    Shape::save(aOut);
}

void Rect::load(arch::ArchiveIn& aIn)
{
    aIn >> _rect;
    Shape::load(aIn);
}

void Rect::save(xml::Writer& w) const
{
    w.attribute("x", _rect.left);
    w.attribute("y", _rect.top);
    double width = _rect.width();
    w.attribute("width", width);
    double height = _rect.height();
    w.attribute("height", height);
    Shape::save(w);
}

void Rect::load(const xml::FileParser::node_iterator& it)
{
    double x=0, y=0, w=0, h=0;
    it.getAttribValue("x", x);
    it.getAttribValue("y", y);
    it.getAttribValue("width", w);
    it.getAttribValue("height", h);
    _rect.left = x; _rect.top = y;
    _rect.setWidth(w); _rect.setHeight(h);
    Shape::load(it);
}
