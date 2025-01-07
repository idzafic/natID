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
