#pragma once
#include "ShapeIOLib.h"
#include <td/Types.h>
#include <arch/ArchiveIn.h>
#include <arch/ArchiveOut.h>
#include <xml/DOMParser.h>
#include <xml/Writer.h>

class SHAPE_IO_API Shape
{
public:
    enum class Type : td::BYTE {Rect=0, RoundedRect, Circ};
protected:
    td::ColorID _fillColor;
    td::ColorID _lineColor;
    td::LinePattern _linePattern;
public:
    Shape(td::ColorID fillColor, td::ColorID lineColor, td::LinePattern lp);
    Shape();
    
    virtual ~Shape();
    virtual Shape::Type getType() const = 0;
    virtual void show(std::ostream& o) const;
    virtual void save(arch::ArchiveOut& aOut) const;
    virtual void load(arch::ArchiveIn& aIn);
    virtual void save(xml::Writer& w) const;
    virtual void load(const xml::FileParser::node_iterator& node);
};


