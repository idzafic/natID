//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/SymbolPopoverView.h>
#include <gui/Image.h>
#include <gui/Symbol.h>

class SymbolsPopover : public gui::SymbolPopoverView
{
    gui::Symbol _pointer;
    gui::Symbol _bus;
    gui::Symbol _resistor;
    gui::Symbol _capacitor;
    gui::Symbol _inductor;
    gui::Symbol _shunt;
    gui::Symbol _generator;
    gui::Symbol _asArrowTL;
    gui::Symbol _asGen;
    gui::Symbol _asMotor;
    gui::Symbol _asSC;
    gui::Symbol _load;
    gui::Symbol _breaker;
    gui::Symbol _disconnector;
    gui::Symbol _fuse;
    gui::Symbol _twoWindingTransf;
    gui::Symbol _threeWindingTransf;
    gui::Symbol _fourWindingTransf;
    gui::Symbol _testRoundedRect;
    gui::Symbol _testOval;
    gui::Symbol _testPie;
    gui::Symbol _testArc;
    gui::Image _toolbarItemImage;
public:
    SymbolsPopover()
    : SymbolPopoverView(16, 8)
    , _pointer(":pointer")
    , _bus(":bus")
    , _asArrowTL(":ASArrowTL")
    , _resistor(":resistor")
    , _capacitor(":capacitor")
    , _inductor(":inductor")
    , _generator(":generator")
    , _asGen(":ASGen")
    , _asMotor(":ASMotor")
    , _asSC(":ASSynchComp")
    , _load(":load")
    , _shunt(":shunt")
    , _breaker(":breaker")
    , _disconnector(":disconctr")
    , _fuse(":fuse")
    , _twoWindingTransf(":Xfmr2")
    , _threeWindingTransf(":Xfmr3")
    , _fourWindingTransf(":Xfmr4")
    , _testRoundedRect(":TestRoundRect")
    , _testOval(":TestOval")
    , _testPie(":TestPie")
    , _testArc(":TestArc")
    , _toolbarItemImage(":poly128")
    {
        gui::Symbol* nullSymbol = nullptr;
        setItem(0, &_pointer, nullSymbol,  tr("pointer"), tr("pointerTT"), false, 0);
        setItem(1, &_bus, nullSymbol, tr("bus"), tr("busTT"), true, 0);
        setItem(2, &_resistor, nullSymbol, tr("resistor"), tr("resistorTT"), true, 90);
        setItem(3, &_capacitor, nullSymbol, tr("Capacitor"), tr("capacitorTT"), true, 90);
        setItem(4, &_inductor, nullSymbol, tr("inductor"), tr("inductorTT"), true, 90);
        setItem(5, &_shunt, nullSymbol, tr("shunt"), tr("shuntTT"), true, 90);
        setItem(6, &_generator, &_asGen, tr("generator"), tr("generatorTT"), true);
        setItem(7, &_generator, &_asMotor, tr("motor"), tr("motorTT"), true);
        setItem(8, &_generator, &_asSC, tr("SC"), tr("SCTT"), true);
        setItem(9, &_load, nullSymbol, tr("load"), tr("loadTT"), true);
        setItem(10, &_breaker, nullSymbol, tr("breaker"), tr("breakerTT"), true);
        setItem(11, &_disconnector, nullSymbol, tr("disconctr"), tr("disconctrTT"), true);
        setItem(12, &_fuse, nullSymbol, tr("fuse"), tr("fuseTT"), true);
        setItem(13, &_twoWindingTransf, nullSymbol, tr("Xfmr2"), tr("Xfmr2TT"), true);
        setItem(14, &_threeWindingTransf, nullSymbol, tr("Xfmr3"), tr("Xfmr3TT"), true);
        setItem(15, &_fourWindingTransf, nullSymbol, tr("Xfmr4"), tr("Xfmr4TT"), true);
        
        styleOnParent(false); //remove this line if you want to see color symbols on toolbar
    }
    
};
