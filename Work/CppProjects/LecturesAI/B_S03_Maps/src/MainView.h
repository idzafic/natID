//
//  Created by Izudin Dzafic on 18/10/2025.
//  Copyright © 2025 IDz. All rights reserved.
//
#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/NumericEdit.h>
#include <gui/CheckBox.h>
#include <gui/ColorPicker.h>
#include <gui/Button.h>
#include <gui/GridLayout.h>
#include <gui/GridComposer.h>
#include <gui/Slider.h>
#include <gui/ComboBox.h>
#include <gui/Timer.h>

#include "ViewMap.h"

class MainView : public gui::View
{
private:
protected:
    using ComboIDs = struct ids
    {
        GraphType townID;
        int posInCombo;
    };
    
    gui::Label _lblMethod;
    gui::ComboBox _cmbMethod;
    gui::Label _lblSpeed;
    gui::Slider _slSpeed;
    gui::Label _lblVisitedColor;
    gui::ColorPicker _visitedColor;
    gui::Label _lblPathColor;
    gui::ColorPicker _pathColor;
    gui::Label _lblStart;
    gui::ComboBox _cmbStart;
    gui::Label _lblGoal;
    gui::ComboBox _cmbGoal;
    
    cnt::SafeFullVector<ComboIDs> _comboIDs;
    
    ViewMap _animation;
    gui::GridLayout _gl;
    const int _maxDelay = 2000;
protected:
    
    bool onKeyPressed(const gui::Key& key) override
    {        
        if (key.getType() == gui::Key::Type::ASCII)
        {
            char ch = key.getChar();
            if (ch == ' ')
            {
                startStop();
                return true;
            }
        }
        return false;
    }
    
    void setupEventHandlers()
    {
        _cmbMethod.onChangedSelection([this](){
            auto iSel = _cmbMethod.getSelectedIndex();
            Primitive::Options& options = _animation.getOptions();
            options.algorithm = Algorithm(iSel);
            _animation.refresh();
            //store properties for the next app start
            auto pApp = gui::getApplication();
            auto appProperties = pApp->getProperties();
            appProperties->setValue("algorithm", options.algorithm);
        });
        
        _slSpeed.onChangedValue([this](){
            double val = _slSpeed.getValue();
            Primitive::Options& options = _animation.getOptions();
            options.sleepMS = math::Max(_maxDelay - int(val), 0);
            //store properties for the next app start
            auto pApp = gui::getApplication();
            auto appProperties = pApp->getProperties();
            appProperties->setValue("sleepMS", options.sleepMS);
        });
        
        _pathColor.onChangedValue([this](){
            td::ColorID colorID = _pathColor.getValue();
            Primitive::Options& options = _animation.getOptions();
            options.pathColor = colorID;
            //store properties for the next app start
            auto pApp = gui::getApplication();
            auto appProperties = pApp->getProperties();
            appProperties->setValue("pathColor", options.pathColor);
        });
        
        _visitedColor.onChangedValue([this](){
            td::ColorID colorID = _visitedColor.getValue();
            Primitive::Options& options = _animation.getOptions();
            options.visitedColor = colorID;
            //store properties for the next app start
            auto pApp = gui::getApplication();
            auto appProperties = pApp->getProperties();
            appProperties->setValue("visitedColor", options.visitedColor);
        });
        
        _cmbStart.onChangedSelection([this](){
            int iSel = _cmbStart.getSelectedIndex();
            if (iSel >= 0)
            {
                Primitive::Options& options = _animation.getOptions();
                options.startID = _comboIDs[iSel].townID;
                _animation.refresh();
                //store properties for the next app start
                auto pApp = gui::getApplication();
                auto appProperties = pApp->getProperties();
                appProperties->setValue("startID", options.startID);
            }
        });
        
        _cmbGoal.onChangedSelection([this](){
            int iSel = _cmbGoal.getSelectedIndex();
            if (iSel >= 0)
            {
                Primitive::Options& options = _animation.getOptions();
                options.goalID = _comboIDs[iSel].townID;
                _animation.refresh();
                //store properties for the next app start
                auto pApp = gui::getApplication();
                auto appProperties = pApp->getProperties();
                appProperties->setValue("goalID", options.goalID);
            }
            
        });
    }
    
    void populateCombos(Primitive::Options& options, mu::IAppProperties* appProperties)
    {
        _comboIDs.clean();
        _cmbStart.clean();
        _cmbGoal.clean();
        const auto& mapTownNameToID = _animation.getMapNameToID();
        auto nPairs = mapTownNameToID.size();
        _comboIDs.reserve(nPairs);
        _comboIDs.zeros();
        int iPos = 0;
        for (const auto& pair : mapTownNameToID)
        {
            const td::String townName = pair.first;
            GraphType townID = pair.second;
            _cmbStart.addItem(townName);
            _cmbGoal.addItem(townName);
            _comboIDs[townID-1].posInCombo = iPos;
            _comboIDs[iPos++].townID = townID;
        }
        
        options.startID = appProperties->getValue("startID", options.startID);
        options.goalID = appProperties->getValue("goalID", options.goalID);
        
        if (options.startID >= _comboIDs.size())
            options.startID = 1;
        if (options.goalID >= _comboIDs.size())
            options.goalID = GraphType(_comboIDs.size());
        
        if (_comboIDs.size() > 0)
        {
            iPos = _comboIDs[options.startID - 1].posInCombo;
            _cmbStart.selectIndex(iPos);
            iPos = _comboIDs[options.goalID - 1].posInCombo;
            _cmbGoal.selectIndex(iPos);
        }
    }
    
public:
    MainView(const std::function<void()>& fnUpdateMenuAndTB, td::BYTE modelID)
    : _lblMethod(tr("lblAlgorithm"))
    , _lblSpeed(tr("lblSpeed"))
    , _lblVisitedColor(tr("lblVisColor"))
    , _lblPathColor(tr("pathColor"))
    , _lblStart(tr("lblStart"))
    , _lblGoal(tr("lblGoal"))
    , _gl(2,13)
    , _animation(fnUpdateMenuAndTB)
    {
        _slSpeed.setRange(0, _maxDelay);
        _cmbMethod.addItem("BFS");
        _cmbMethod.addItem("DFS");
        _cmbMethod.addItem("DFS+Heuristic");
        _cmbMethod.addItem("GreedyBestFS");
        _cmbMethod.addItem("UCS");
        _cmbMethod.addItem("A*");
        
        Primitive::Options& options = _animation.getOptions();
        
        //adjust options
        auto pApp = gui::getApplication();
        auto appProperties = pApp->getProperties();
        
        options.algorithm = appProperties->getValue("algorithm", options.algorithm);
        options.sleepMS = appProperties->getValue("sleepMS", options.sleepMS);
        
        options.pathColor = appProperties->getValue("pathColor", options.pathColor);
        options.visitedColor = appProperties->getValue("visitedColor", options.visitedColor);
        
        //auto fileName = getResFileName(":map");
        loadModel(modelID);
        //populate combos
        populateCombos(options, appProperties);
        
        //put it to gui elements
        _cmbMethod.selectIndex(int(options.algorithm));
        _slSpeed.setValue(_maxDelay - options.sleepMS);
        _visitedColor.setValue(options.visitedColor);
        _pathColor.setValue(options.pathColor);
        
        //setup event handlers
        setupEventHandlers();
        
        gui::GridComposer gc(_gl);
        
        gc.startNewRowWithSpace(5,0) << _lblMethod << _cmbMethod  << _lblSpeed << _slSpeed << _lblVisitedColor << _visitedColor << _lblPathColor << _pathColor << _lblStart << _cmbStart << _lblGoal << _cmbGoal;
        gc.appendRow(_animation, -1);
        
        setLayout(&_gl);
    }
    
    bool isRunning() const
    {
        return _animation.isRunning();
    }
    
    void startStop()
    {
        if (_animation.isRunning())
        {
            _animation.stop();
        }
        else
        {
            _animation.start();
        }
    }

    void setFocusToAnimation()
    {
        _animation.setFocus(); //for keyboard events
    }
    
    bool loadModel(td::BYTE modelID)
    {
        td::String modelPath;
        switch (modelID)
        {
            case 10:
                modelPath = getResFileName(":4Node");
                break;
            case 20:
                modelPath = getResFileName(":12Node");
                break;
            case 30:
                modelPath = getResFileName(":map");
                break;
            default:
                assert(false);
        }
        if (modelPath.isEmpty())
            return false;
        bool bToRet = _animation.loadModel(modelPath);
        Primitive::Options& options = _animation.getOptions();
        auto pApp = gui::getApplication();
        auto appProperties = pApp->getProperties();
        populateCombos(options, appProperties);
        return bToRet;
    }
    
    ViewMap& getViewMap()
    {
        return _animation;
    }
};
