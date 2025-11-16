#include "../../common/include/RoundedRect.h"

RoundedRect::RoundedRect(const TdRect& r, double radius, td::ColorID fillColor, td::ColorID lineColor, td::LinePattern lp)
: Rect(r, fillColor, lineColor, lp)
, _radius(radius)
{
}

RoundedRect::RoundedRect()
{
}

Shape::Type RoundedRect::getType() const
{
    return Shape::Type::RoundedRect;
}

void RoundedRect::show(std::ostream& o) const
{
    o << "RoundingRect radius: " << _radius << " ";
    Rect::show(o);
}

void RoundedRect::save(arch::ArchiveOut& aOut) const
{
    aOut << _radius;
    Rect::save(aOut);
}

void RoundedRect::load(arch::ArchiveIn& aIn)
{
    aIn >> _radius;
    Rect::load(aIn);
}

void RoundedRect::save(xml::Writer& w) const
{
    w.attribute("r", _radius);
    Rect::save(w);
}

void RoundedRect::load(const xml::FileParser::node_iterator& it)
{
    it.getAttribValue("r", _radius);
    Rect::load(it);
}

