#pragma once
#include "Rect.h"


class SHAPE_IO_API RoundedRect : public Rect
{
protected:
    double _radius;
public:
    RoundedRect(const TdRect& r, double radius, td::ColorID fillColor, td::ColorID lineColor, td::LinePattern lp);
    RoundedRect();
    
    virtual Shape::Type getType() const;
    virtual void show(std::ostream& o) const;
    virtual void save(arch::ArchiveOut& aOut) const;
    virtual void load(arch::ArchiveIn& aIn);
    virtual void save(xml::Writer& w) const;
    virtual void load(const xml::FileParser::node_iterator& node);
};


