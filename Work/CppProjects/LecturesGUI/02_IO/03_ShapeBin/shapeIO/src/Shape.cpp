#include "../../common/include/Shape.h"
#include <td/ColorID.h>

Shape::Shape(td::ColorID fillColor, td::ColorID lineColor, td::LinePattern lp)
: _fillColor(fillColor)
, _lineColor(lineColor)
, _linePattern(lp)
{
}

Shape::Shape()
{}

Shape::~Shape()
{
}

void Shape::show(std::ostream& o) const
{
    o << " fill clr:" << td::toString(_fillColor) << ", line clr:" << td::toString(_lineColor)
      << " line pattern:" << td::toString(_linePattern);
}

void Shape::save(arch::ArchiveOut& aOut) const
{
    arch::EnumSaver<td::LinePattern> lnPattern(_linePattern);
    aOut << _fillColor << _lineColor << lnPattern;
}

void Shape::load(arch::ArchiveIn& aIn)
{
    arch::EnumLoader<td::LinePattern> lnPattern(_linePattern, td::LinePattern::NA, td::LinePattern::Solid);
    aIn >> _fillColor >> _lineColor >> lnPattern;
}
