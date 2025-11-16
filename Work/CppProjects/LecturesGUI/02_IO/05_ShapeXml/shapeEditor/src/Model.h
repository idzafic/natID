#pragma once
#include "../../common/include/RoundedRect.h"
#include "../../common/include/Circ.h"
#include <cnt/PushBackVector.h>
#include <iostream>
#include <arch/FileSerializer.h>
#include <arch/ArchiveIn.h>
#include <arch/ArchiveOut.h>
#include <xml/DOMParser.h>
#include <xml/Writer.h>

class Model
{
    cnt::PushBackVector<Shape*> _shapes;
    
protected:
    void clean()
    {
        for (auto& shape : _shapes)
            delete shape;
    }
    
public:
    Model(size_t initialCapacity = 16)
    : _shapes(initialCapacity)
    {
    }
    
    ~Model()
    {
        clean();
    }
    
    void addRect()
    {
        double x;
        std::cout << "Unesite TL koord. (x): ";
        std::cin >> x;
        double y;
        std::cout << "Unesite TL koord. (y): ";
        std::cin >> y;
        double w;
        std::cout << "Unesite sirinu: ";
        std::cin >> w;
        double h;
        std::cout << "Unesite visinu: ";
        std::cin >> h;
        
        td::Point<double> pt(x,y);
        td::Size<double> sz(w,h);
        TdRect r(pt, sz);
        Shape* pShape = new Rect(r, td::ColorID::White, td::ColorID::Black, td::LinePattern::Solid);
        _shapes.push_back(pShape);
    }
    
    void addRoundedRect()
    {
        double x;
        std::cout << "Unesite TL koord. (x): ";
        std::cin >> x;
        double y;
        std::cout << "Unesite TL koord. (y): ";
        std::cin >> y;
        double w;
        std::cout << "Unesite sirinu: ";
        std::cin >> w;
        double h;
        std::cout << "Unesite visinu: ";
        std::cin >> h;
        
        double radius;
        std::cout << "Unesite radijus zakrivljenja: ";
        std::cin >> radius;
        
        td::Point<double> pt(x,y);
        td::Size<double> sz(w,h);
        TdRect r(pt, sz);
        Shape* pShape = new RoundedRect(r, radius, td::ColorID::White, td::ColorID::Black, td::LinePattern::Solid);
        _shapes.push_back(pShape);
    }
    
    void addCircle()
    {
        double x;
        std::cout << "Unesite xc: ";
        std::cin >> x;
        
        double y;
        std::cout << "Unesite yc: ";
        std::cin >> y;
        
        double radius;
        std::cout << "Unesite radijus: ";
        std::cin >> radius;
        
        td::Point<double> pt(x,y);

        td::Circle<double> c(pt, radius);

        Shape* pShape = new Circ(c, td::ColorID::White, td::ColorID::Black, td::LinePattern::Solid);
        _shapes.push_back(pShape);
    }
    
    bool save(const char* fileName) const
    {
        arch::FileSerializerOut fs;
        if (!fs.open(fileName))
            return false;
        arch::ArchiveOut ar("ETFA", fs);
        td::UINT4 nShapes = _shapes.size();
        try
        {
            ar << nShapes;
            
            for (auto pShape : _shapes)
            {
                td::BYTE shType = (td::BYTE) pShape->getType();
                ar << shType;
                pShape->save(ar);
            }
        }
        catch(...)
        {
            return false;
        }
        return true;
    }
    
    bool load(const char* fileName)
    {
        arch::FileSerializerIn fs;
        if (!fs.open(fileName))
            return false;
        
        //delete previous model entries
        clean();
        
        arch::ArchiveIn ar(fs);
        ar.setSupportedMajorVersion("ETFA");
        td::UINT4 nShapes = _shapes.size();
        
        try
        {
            ar >> nShapes;
            for (size_t i=0; i<nShapes; ++i)
            {
                td::BYTE shType;
                ar >> shType;
                Shape::Type st = (Shape::Type) shType;
                Shape* pShape = nullptr;
                switch (st)
                {
                    case Shape::Type::Rect: pShape = new Rect; break;
                    case Shape::Type::Circ: pShape = new Circ; break;
                    case Shape::Type::RoundedRect: pShape = new RoundedRect; break;
                }
                assert(pShape);
                pShape->load(ar);
                _shapes.push_back(pShape);
            }
        }
        catch(...)
        {
            return false;
        }
        return true;
    }
    
    bool saveXml(const td::String& fileName) const
    {
        xml::Writer w(fileName);
        w.startDocument();
        w.startNode("Shapes");
        
        for (auto pShape : _shapes)
        {
            auto st = pShape->getType();
            switch (st)
            {
                case Shape::Type::Rect:
                    w.startNode("Rect");
                    break;
                case Shape::Type::RoundedRect:
                    w.startNode("RoundedRect");
                    break;
                case Shape::Type::Circ:
                    w.startNode("Circ");
                    break;
                default:
                    assert(false);
                    return false;
            }
            pShape->save(w);
            w.endNode();
        }
        w.endDocument();
        return true;
    }
    
    bool loadXml(const td::String& fileName)
    {
        xml::FileParser parser;
        if (!parser.parseFile(fileName))
            return false;
        
        const auto& root = parser.getRootNode();
        if (root->getName().cCompare("Shapes") != 0)
            return false;

        //delete previous model entries
        clean();
        auto it = root.getChildNode();
        
        while (it.isOk())
        {
            Shape* pShape = nullptr;
            if (it->getName().cCompare("Rect") == 0)
                pShape = new Rect;
            else if (it->getName().cCompare("RoundedRect") == 0)
                pShape = new RoundedRect;
            else if (it->getName().cCompare("Circ") == 0)
                pShape = new Circ;
            if (!pShape)
                return false;
            pShape->load(it);
            _shapes.push_back(pShape);
            ++it;
        }
        return true;
    }
    
    void show(std::ostream& o) const
    {
        o << td::endl << "Shapes:" << td::endl;
        o << "=====================================" << td::endl;
        for(const auto& shape : _shapes)
        {
            shape->show(o);
            o << td::endl;
        }
        o << "=====================================" << td::endl;
    }
};
