#pragma once
#include "Shape.h"
#include <td/RectNormalized.h>

typedef td::RectNormalized<double> TdRect;

class SHAPE_IO_API Rect : public Shape
{
protected:
    TdRect _rect;
public:
    Rect(const TdRect& r, td::ColorID fillColor, td::ColorID lineColor, td::LinePattern lp);
    Rect();
    virtual Shape::Type getType() const;
    virtual void show(std::ostream& o) const;
    virtual void save(arch::ArchiveOut& aOut) const;
    virtual void load(arch::ArchiveIn& aIn);
};


