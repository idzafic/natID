#pragma once
#include "Shape.h"
#include <td/Circle.h>

typedef td::Circle<double> Circle;

class SHAPE_IO_API Circ : public Shape
{
protected:
    Circle _circle;
public:
    Circ(const Circle& c, td::ColorID fillColor, td::ColorID lineColor, td::LinePattern lp);
    Circ();
    virtual Shape::Type getType() const;
    virtual void show(std::ostream& o) const;
    virtual void save(arch::ArchiveOut& aOut) const;
    virtual void load(arch::ArchiveIn& aIn);
};

