#include "../../common/include/Circ.h"

Circ::Circ(const Circle& c, td::ColorID fillColor, td::ColorID lineColor, td::LinePattern lp)
: Shape(fillColor, lineColor, lp)
, _circle(c)
{
}

Circ::Circ(){}

Shape::Type Circ::getType() const
{
    return Shape::Type::Circ;
}

void Circ::show(std::ostream& o) const
{
    o << "Circle data:";
    _circle.show(o);
    
    Shape::show(o);
}

void Circ::save(arch::ArchiveOut& aOut) const
{
    aOut << _circle;
    Shape::save(aOut);
}

void Circ::load(arch::ArchiveIn& aIn)
{
    aIn >> _circle;
    Shape::load(aIn);
}

void Circ::save(xml::Writer& w) const
{
    w.attribute("x", _circle.center.x);
    w.attribute("y", _circle.center.y);
    w.attribute("r", _circle.r);
    Shape::save(w);
}

void Circ::load(const xml::FileParser::node_iterator& it)
{
    double x=0, y=0, r=0;
    it.getAttribValue("x", x);
    it.getAttribValue("y", y);
    it.getAttribValue("r", r);
    _circle.center.x = x; _circle.center.y = y; _circle.r = r;
    Shape::load(it);
}

