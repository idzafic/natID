#pragma once
#include <sc/ISolutionBuffer.h>
#include <fo/FileOperations.h>
#include <td/DateTime.h>
#include <thread/BoundedBuffer.h>
#include "SimulationParams.h"

struct SolEntry
{
    td::DateTime dt;
    float x;
    float y;
    float v;
};

class SolutionBuffer : public sc::ISolutionBuffer
{
    double* _pX = nullptr;
    SOLVERTYPE* _pParam = nullptr;
    
    thread::BoundedBuffer<struct SolEntry, 100> _buffer;
    
    td::DateTime _dt;
    td::Time _dMs;
#ifdef USE_ODE_MODEL
    SOLVERTYPE _length = 0;
    int _iTheta = -1;
    int _iOmega = -1;
    int _iL = -1;
    
#else
    int _ix=-1;
    int _ivx=-1;
    int _iy=-1;
    int _ivy=-1;
#endif
    int _iEntry = 0;
    bool _producing = true;
    
public:
    
    SolutionBuffer()
    {
    }
    
    SolutionBuffer(int dMs, sc::ISolver<SOLVERTYPE>* pSolver)
    {
        init(dMs, pSolver);
    }
    
    void init(int dMs, sc::ISolver<SOLVERTYPE>* pSolver)
    {
        _dMs.setTime(0, 0, 0, dMs);
        _pX = pSolver->getVariablesPtr();
        _pParam = pSolver->getParamsPtr();
#ifdef USE_ODE_MODEL
        _iTheta = pSolver->getVariableIndex("theta");
        _iOmega = pSolver->getVariableIndex("omega");
        _iL = pSolver->getParamIndex("l");
        _length = _pParam[_iL];
#else
        _ix = pSolver->getVariableIndex("x");
        _ivx = pSolver->getVariableIndex("v_x");
        _iy = pSolver->getVariableIndex("y");
        _ivy = pSolver->getVariableIndex("v_y");
#endif
        _dt.now();
    }
    
    void put() override
    {        
        _producing = true;
        if (_iEntry == 0)
            _dt.now();
        struct SolEntry solEntry;
        _dt += _dMs;
//        std::cout << _dt;
        solEntry.dt = _dt;
#ifdef USE_ODE_MODEL
        SOLVERTYPE theta = _pX[_iTheta];
        solEntry.x = (float) (_length * std::sin(theta));
        solEntry.y = (float) (_length * std::cos(theta));
        SOLVERTYPE omega = _pX[_iOmega];
        float v = (float) std::abs((omega * _length));
#else
        solEntry.x = _pX[_ix];
        solEntry.y = _pX[_iy];
        float vx = (float)_pX[_ivx];
        float vy = (float)_pX[_ivy];
        float v = std::sqrt(vx*vx+vy*vy);
#endif
        solEntry.v = v;
        _buffer.push(solEntry);
        ++_iEntry;
//        std::cout << "Entry " << _iEntry << std::endl;
    }
    
    bool get(const td::DateTime& afterDT, struct SolEntry& val)
    {
//        return false;
        while (_producing)
        {
            _buffer.pop(val);
            if (val.dt >= afterDT)
                return true;
            int k=0;
        }
        return false;
    }
    
    //to wake up producing thread
    void removeOneEntry()
    {
        struct SolEntry val;
        _buffer.tryPop(val);
    }
    
    void removeAllEntries()
    {
        struct SolEntry val;
        while (true)
        {
            if (!_buffer.tryPop(val))
                break;
        }
    }
    
    void finalize(sc::ISolutionBuffer::Status status) override
    {
        _iEntry = 0;
        _producing = false;
        _buffer.clear();
    }
#ifdef USE_ODE_MODEL
    int getThetaIndex() const
    {
        return _iTheta;
    }
    
    int getOmegaIndex() const
    {
        return _iOmega;
    }
    
#else
    int getXIndex() const
    {
        return _ix;
    }
    
    int getYIndex() const
    {
        return _iy;
    }
#endif
};
