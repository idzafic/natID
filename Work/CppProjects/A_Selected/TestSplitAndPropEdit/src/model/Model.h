//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "IShape2D.h"
#include <cnt/PushBackVector.h>
#include <arch/FileSerializer.h>
#include <arch/ArchiveIn.h>
#include <arch/ArchiveOut.h>

//Interface to 2d shapes (Rect, Circle,...)
class Model
{
    cnt::PushBackVector<IShape2D*, 1024> _shapes;
    gui::Size _modelSize;
protected:
    void clean()
    {
        for (auto pShape : _shapes)
        {
            pShape->release();
        }
    }
public:
    Model()
    : _modelSize(10,10)
    {
    }
    
    ~Model()
    {
        clean();
    }
    
    void draw(const gui::Rect& rDraw) const
    {
        gui::Rect boundingRect;
        size_t nDrawn = 0;
        for (auto pShape : _shapes)
        {
            pShape->getBoundingRect(boundingRect);
            if (boundingRect.intersects(rDraw))
            {
                pShape->draw();
                ++nDrawn;
            }
        }
        //mu::dbgLog("#Drawn=%d", nDrawn);
    }
    
    void appendShape(IShape2D* pShape)
    {
        gui::Rect boundRect;
        pShape->getBoundingRect(boundRect);
        boundRect.inflate(10, 0, 0);
        gui::Rect currentModelRect(gui::Point(0,0), _modelSize);
        currentModelRect.unija(boundRect);
        _modelSize.width = currentModelRect.width();
        _modelSize.height = currentModelRect.height();
//        mu::dbgLog("Model w=%.1f, h=%.1f", _modelSize.width, _modelSize.height);
        
        _shapes.push_back(pShape);        
    }
    
    bool load(const td::String& fileName)
    {
        arch::FileSerializerIn fs;
        if (!fs.open(fileName))
            return false;
        clean();
        _shapes.clean();
        
        arch::ArchiveIn ar(fs);
        ar.setSupportedMajorVersion("GETF");
        try
        {
            td::UINT4 nElems = 0;
            double modelW = 0;
            double modelH = 0;
            ar >> nElems >> modelW >> modelH;
            _modelSize.width = modelW;
            _modelSize.height = modelH;
            
            if (nElems == 0)
                return false;
            
            _shapes.reserve(nElems);
    
            for (td::UINT4 iElem = 0; iElem < nElems; ++iElem)
            {
                td::BYTE sht = 0;
                ar >> sht;
                IShape2D::Type shType = (IShape2D::Type) sht;
                switch(shType)
                {
                    case IShape2D::Type::Rect:
                    {
                        gui::Rect r(0,0, 0, 0);
                        IShape2D* pShape = IShape2D::createRect(gui::Shape::Attribs::LineAndFill, r, td::ColorID::SysText, td::ColorID::SysText, 1.0f, td::LinePattern::Solid);
                        pShape->load(ar);
                        pShape->init();
                        appendShape(pShape);
                    }
                        break;
                    case IShape2D::Type::RoundRect:
                    {
                        gui::Rect r(0,0, 0, 0);
                        float radius = 0;
                        IShape2D* pShape = IShape2D::createRoundedRect(gui::Shape::Attribs::LineAndFill, r, radius, td::ColorID::SysText, td::ColorID::SysText, 1.0f, td::LinePattern::Solid);
                        pShape->load(ar);
                        pShape->init();
                        appendShape(pShape);
                    }
                        break;
                    case IShape2D::Type::Circle:
                    {
                        gui::Point pt(0, 0);
                        float radius = 0;
                        IShape2D* pShape = IShape2D::createCircle(gui::Shape::Attribs::LineAndFill, pt, radius, td::ColorID::SysText, td::ColorID::SysText, 1.0f, td::LinePattern::Solid);
                        pShape->load(ar);
                        pShape->init();
                        appendShape(pShape);
                    }
                        break;
                    default:
                    {
                        assert(false);
                        return false;
                    }
                        
                }
            }
        }
        catch(...)
        {
            return false;
        }
        return true;
    }
    
    bool save(const td::String& fileName) const
    {
        arch::FileSerializerOut fs;
        if (!fs.open(fileName))
            return false;
        arch::ArchiveOut ar("GETF", fs);
        try
        {
            td::UINT4 nElems = (td::UINT4) _shapes.size();
            double modelW = _modelSize.width;
            double modelH = _modelSize.height;
            ar << nElems << modelW << modelH;
            //td::UINT4 iElem = 0;
            for (auto pShape : _shapes)
            {
                td::BYTE shType = (td::BYTE) pShape->getType();
                ar << shType;
                pShape->save(ar);
                //++iElem;
            }
        }
        catch(...)
        {
            return false;
        }
        return true;
    }
    
    const gui::Size& getModelSize() const
    {
        return _modelSize;
    }
    
    IShape2D* getSelectedElement(const gui::Point& pt)
    {
        size_t nElems = _shapes.size();
        if (nElems == 0)
            return nullptr;
        for (size_t i=nElems; i>0; --i)
        {
            IShape2D* pShape = _shapes[i-1];
            if (pShape->canBeSelected(pt))
            {
                return pShape;
            }
        }
        return nullptr;
    }
    
    void remove(IShape2D*& pShape)
    {
        if (pShape)
        {
            _shapes.remove(pShape);
            pShape->release();
            pShape = nullptr;
        }
    }
    
    bool setFront(IShape2D* pShape)
    {
        auto nShapes = _shapes.size();
        if (nShapes <= 1)
            return false;
        
        if (pShape)
        {
            //TODO: Za vjezbu
            auto pos = _shapes.find(pShape);
            if (pos > 0)
                return _shapes.move(pos, 0);
            return false;
        }
        return false;
    }
    
    bool setBack(IShape2D* pShape)
    {
        auto nShapes = _shapes.size();
        if (nShapes <= 1)
            return false;
        
        if (pShape)
        {
            //TODO: Za vjezbu
            
            auto pos = _shapes.find(pShape);
            if (pos < nShapes -1)
                return _shapes.move(pos, nShapes-1);
            return false;
        }
        return false;
    }
};
