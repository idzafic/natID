//
//  Created by Izudin Dzafic on 10 Nov 2022.
//  Copyright © 2022 IDz. All rights reserved.
//

#pragma once

struct X
{
    virtual ~X() {}
};



struct A : public X
{
    td::String value = "ABDS";
    auto getValue() const { return value; }
};


struct B : public A
{
    int intVal;
    B(int x) : intVal(x) {}
    auto getInt() const { return intVal; }
};

struct B2 : public B
{
    B2(int x) : B(x) {}
};

struct C
{
    A* bp;
    void setBase(A* _bp)
    {
        bp = _bp;
    }
    auto getBase() 
    {
        return bp;
    }
    virtual ~C() {}

};

struct D : public C
{
    B2 b;
    D()
        : b(231) 
    {
        C::setBase(&b);
    }
};

inline void doStructTests()
{
    D dialog;
    auto Aptr = dialog.getBase();
    auto Bptr = dynamic_cast<B2*>(Aptr);
}

