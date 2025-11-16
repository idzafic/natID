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
