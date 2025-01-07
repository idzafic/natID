#pragma once
#include <gui/Canvas.h>
#include <gui/Shape.h>
#include <math/math.h>
#include <gui/Image.h>
#include <gui/Symbol.h>
#include <gui/Transformation.h>
#include <math/Constants.h>
#include <math/math.h>
#include <gui/PopoverView.h>
#include <vector>
#include <time.h>
#include "DialogSettings.h"
#include "StatusBar.h"

#include "GlobalEvents.h"

#define MaxGridSize 32


class ViewMinesweeper : public gui::Canvas
{
public:
    enum class Difficulty { Easy = 0, Medium, Hard };

protected:
    gui::Image _tileImages[12];

    StatusBar* _pStatusBar;
    td::INT4 _index;
    td::INT4 _width=0, _height=0;
    td::INT4 _bombs = 0;
    
    td::INT4 _grid[MaxGridSize][MaxGridSize];
    td::INT4 _sgrid[MaxGridSize][MaxGridSize];
    
    td::INT4 _brojacz;
    td::INT4 _brojac;
    
    const gui::CoordType _w = 32;
    
    gui::Size _size;
    gui::Rect _tileRect;
    const td::UINT4 _cSettingsDlgID = 17; //any unique id among dialogs
    Difficulty _difficulty = Difficulty::Medium;

    bool _gameOver = false;
    bool _gameStarted = false;
    bool _animateDifficultyTransition = false;
protected:
    bool onToolbarsComboBoxSelectionChange(gui::ComboBox* pCmb, td::UINT2 ctrlID, int selection) override
    {
        //no need to check ctrlID, since there is only 1 combo box in toolbar
        switch (selection)
        {
            case 0:
                setDifficulty(ViewMinesweeper::Difficulty::Easy);
                return true;
            case 1:
                setDifficulty(ViewMinesweeper::Difficulty::Medium);
                return true;
            case 2:
                setDifficulty(ViewMinesweeper::Difficulty::Hard);
                return true;
            case 3: //Restart game
                pCmb->selectIndex((int) _difficulty);
                return true;
            default:
                assert(false);
        }
        return false;
    }
    
    bool onToolbarsPopoverSelectionChange(gui::PopoverView* pPOView, td::UINT2 ctrlID, td::UINT2 selection) override
    {
        //no need to check ctrlID, since there is only 1 combo box in toolbar
        switch (selection)
        {
            case 0:
                setDifficulty(ViewMinesweeper::Difficulty::Easy);
                return true;

            case 1:
                setDifficulty(ViewMinesweeper::Difficulty::Medium);
                return true;
            case 2:
                setDifficulty(ViewMinesweeper::Difficulty::Hard);
                return true;
            case 3: //Restart game
                setDifficulty(_difficulty);
                pPOView->setCurrentSelection((td::UINT2) _difficulty);
                return true;
            default:
                assert(false);
        }
        return false;
    }
    
    
    bool onClick(gui::Dialog* dlg, td::UINT4 dlgID) override
    {

        //get notification from dialog
        if (dlgID == _cSettingsDlgID)
        {
            if (dlg->getClickedButtonID() == gui::Dialog::Button::ID::OK)
            {
                DialogSettings* pDlg = (DialogSettings*) dlg;
                const auto& view = pDlg->getView();
//                _animateDifficultyTransition = view.isAnimateDifficultyTransition();
                
                getAppProperties()->setKeyValue("Theme", view.getTheme());

                updateTheme();

                if (view.isRestartRequired())
                {
                    gui::Alert::showYesNoQuestion(tr("RestartRequired"), tr("RestartRequiredInfo"), tr("Restart"), tr("DoNoRestart"), [this] (gui::Alert::Answer answer) {
                        if (answer == gui::Alert::Answer::Yes)
                        {
                            auto pApp = getApplication();
                            //clean up and save here whatever you need, the application is about to terminate... and restart fresh....
                            pApp->restart();
                        }
                    });
                }
            }
            return true;
        }
        return false;
    }

    enum class Theme{classic, modern, modern2} currentTheme;

    void updateTheme()
    {
        auto oldTheme = currentTheme;
        td::String newThemeStr = getApplication()->getProperties()->getValue("Theme", "Modern");
        if(newThemeStr.cCompareNoCase("Classic") == 0)
            currentTheme = Theme::classic;
        if(newThemeStr.cCompareNoCase("Modern") == 0)
            currentTheme = Theme::modern;
        if(newThemeStr.cCompareNoCase("Modern2") == 0)
            currentTheme = Theme::modern2;
        

        td::String prefix(":");
        if(currentTheme == Theme::classic)
            prefix += "";
        if(currentTheme == Theme::modern || currentTheme == Theme::modern2)
        {
            if(getApplication()->isDarkMode())
                prefix += "D";
            else
                prefix += "L";
        }

         if(currentTheme == Theme::modern2)
            prefix += "2";

        constexpr const char* names[]{"otkrivena", "kec", "dvica", "trica", "cetvorka", "petica", "sestica", "sedmica", "osmica", "bomb", "skrivena", "flag"};

        td::String path;
        for(int i = 0; i<12; ++i)
        {
            path = prefix;
            path += names[i];
            _tileImages[i].load(gui::getResFileName(path.c_str()));
        }

        reDraw();
    }
    
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
        auto pAI = aiDesc.getActionItem();
        
        if (menuID == 10 & actionID == 10)
        {
            auto pDlg = getAttachedWindow(_cSettingsDlgID);
            if (pDlg)
                pDlg->setFocus();
            else
            {
                gui::Window* pWnd = getParentWindow();
                auto statusBar = pWnd->getStatusBar();
                gui::Size sz;
                getFieldSize(sz);
                DialogSettings* pSettingsDlg = new DialogSettings(sz, statusBar, this, _animateDifficultyTransition, _cSettingsDlgID);
                pSettingsDlg->keepOnTopOfParent();
                
                auto mainTB = pWnd->getToolBar();
                auto& view = pSettingsDlg->getView();
                view.setMainTB(mainTB);
                
                pSettingsDlg->open();
            }
            return true;
        }
        
        if (menuID == 20 && firstSubMenuID== 0 && lastSubMenuID == 0)
        {
            switch (actionID)
            {
                case 20:
                    setDifficulty(ViewMinesweeper::Difficulty::Easy);
                    return true;

                case 21:
                    setDifficulty(ViewMinesweeper::Difficulty::Medium);
                    return true;
                case 22:
                    setDifficulty(ViewMinesweeper::Difficulty::Hard);
                    return true;
                case 23: //Restart
                    setDifficulty(_difficulty);
                    return true;
                case 30:
                {
                    return true;
                }
            }
        }
        return false;
    }
    
    void onDraw(const gui::Rect& rect) override
    {
       
        for (td::INT4 i = 0; i < _width; i++) {
            for (td::INT4 j = 0; j < _height; j++)
            {
                td::INT4 broj = _sgrid[i][j];
                assert (broj < 12);
                _tileRect = {i * _w, j * _w, _w };
                {
                    gui::Transformation::saveContext();
                    gui::Transformation tr;
                    _tileImages[broj].draw(_tileRect);
                    tr.restoreContext();
                    
                }
            }
        }
    }

    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override
    {
        gui::Point click = inputDevice.getFramePoint();
        td::INT4 x = click.x / _w;
        td::INT4 y = (click.y) / _w;
        if (!_gameStarted) {
            newGame(x, y);
            _gameStarted = true;
        }
        if (_sgrid[x][y] != 10)
        {
            _pStatusBar->setMessage(tr("EmptyFieldClicked"));
            return; //no need to redraw
        }
        else if (_grid[x][y] == 0)
        {
            clearEmptySpace(x, y);
            _pStatusBar->setMessage(tr("BulkClear"));
        }
        else if (_grid[x][y] == 9)
        {
            _index = 3;
            gameOver();
        }
        else
        {
            _pStatusBar->setMessage(tr("ClearedField"));
            _sgrid[x][y] = _grid[x][y];
        }
        reDraw();
    }

    void onSecondaryButtonPressed(const gui::InputDevice& inputDevice) override
    {
        gui::Point click = inputDevice.getFramePoint();
        td::INT4 x = click.x / _w;
        td::INT4 y = (click.y) / _w;
        bool markerPlaced = false;
        if (_sgrid[x][y] == 11)
        {
            _sgrid[x][y] = 10;
            _brojacz++;
            _pStatusBar->setMessage(tr("FlagRemoved"));
            _pStatusBar->setNoOfFlags(_bombs - _brojacz);
            if (_grid[x][y] == 9)
                _brojac++;
            markerPlaced = true;
        }
        else if (_sgrid[x][y] == 10) 
        {
            _sgrid[x][y] = 11;
            _brojacz--;
            _pStatusBar->setNoOfFlags(_bombs - _brojacz);
            _pStatusBar->setMessage(tr("FlagAdded"));
            if (_grid[x][y] == 9)
                _brojac--;
            markerPlaced = true;
        }
        
        if (_brojac == 0)
        {
            _index = 2;
            gameOver();
            markerPlaced = true;
        }
        reDraw();
        if (!markerPlaced)
        {
            _pStatusBar->setMessage(tr("FlagFailed"));
        }
    }

    void clearEmptySpace(td::INT4 i, td::INT4 j)
    {
        _sgrid[i][j] = _grid[i][j];
        if (i < _width - 1 && _sgrid[i + 1][j] == 10) {
            if (_grid[i + 1][j] == 0)
                clearEmptySpace(i + 1, j);
            else
                _sgrid[i + 1][j] = _grid[i + 1][j];
        }
        if (j < _height - 1 && _sgrid[i][j + 1] == 10) {
            if (_grid[i][j + 1] == 0)
                clearEmptySpace(i, j + 1);
            else
                _sgrid[i][j + 1] = _grid[i][j + 1];
        }
        if (i > 0 && _sgrid[i - 1][j] == 10) {
            if (_grid[i - 1][j] == 0)
                clearEmptySpace(i - 1, j);
            else
                _sgrid[i - 1][j] = _grid[i - 1][j];
        }
        if (j > 0 && _sgrid[i][j - 1] == 10) {
            if (_grid[i][j - 1] == 0)
                clearEmptySpace(i, j - 1);
            else
                _sgrid[i][j - 1] = _grid[i][j - 1];
        }
        if (i < _width - 1 && j < _height - 1 && _sgrid[i + 1][j + 1] == 10) {
            if (_grid[i + 1][j + 1] == 0)
                clearEmptySpace(i + 1, j + 1);
            else
                _sgrid[i + 1][j + 1] = _grid[i + 1][j + 1];
        }
        if (i > 0 && j > 0 && _sgrid[i - 1][j - 1] == 10) {
            if (_grid[i - 1][j - 1] == 0)
                clearEmptySpace(i - 1, j - 1);
            else
                _sgrid[i - 1][j - 1] = _grid[i - 1][j - 1];
        }
        if (i > 0 && j < _height - 1 && _sgrid[i - 1][j + 1] == 10) {
            if (_grid[i - 1][j + 1] == 0)
                clearEmptySpace(i - 1, j + 1);
            else
                _sgrid[i - 1][j + 1] = _grid[i - 1][j + 1];
        }
        if (i < _width - 1 && j>0 && _sgrid[i + 1][j - 1] == 10) {
            if (_grid[i + 1][j - 1] == 0)
                clearEmptySpace(i + 1, j - 1);
            else
                _sgrid[i + 1][j - 1] = _grid[i + 1][j - 1];
        }
    }
    void gameOver(bool showAlert = true)
    {
        for (td::INT4 i = 0; i < _width; i++) {
            for (td::INT4 j = 0; j < _height; j++)
            {
                if (_sgrid[i][j] == 10 || _sgrid[i][j] == 11)
                    _sgrid[i][j] = _grid[i][j];
            }
        }
        _gameStarted = false;
        
        if (showAlert)
        {
            if (_brojac != 0)
            {
                td::String strDead(tr("Dead"));
                _pStatusBar->setMessage(strDead);
                gui::Alert::show(tr("Done"), strDead);
            }
            else
            {
                td::String strOK(tr("AllOK"));
                _pStatusBar->setMessage(strOK);
                gui::Alert::show(tr("Done"), tr("AllOK"));
            }
        }
    }
    
    ViewMinesweeper(){}

public:
    ViewMinesweeper(StatusBar* pStatusBar)
    : gui::Canvas({ gui::InputDevice::Event::PrimaryClicks, gui::InputDevice::Event::SecondaryClicks })
    , _pStatusBar(pStatusBar)
    {
        bool bDark = getApplication()->isDarkMode();        
        loadUserBoard();
        auto props = getAppProperties();
        _animateDifficultyTransition = props->getValue("animateTransition", false);
        //updateTheme();
        systemAppearanceChangedEvent.emplace_back([this](bool darkMode){updateTheme();});
    }

    void loadUserBoard()
    {
        _index = 0;
        for (td::INT4 i = 0; i < MaxGridSize ; i++)
        {
            for (td::INT4 j = 0; j < MaxGridSize; j++)
            {
                _sgrid[i][j] = 10;
                _grid[i][j] = 0;
            }
        }
        reDraw();
    }

    void newGame(td::INT4 a, td::INT4 b)
    {

        _brojac = _bombs;
        _brojacz = _bombs;
        srand((unsigned int) time(0));

        td::INT4 cnt = 0;
        do {
            td::INT4 x = rand() % _width;
            td::INT4 y = rand() % _height;
            if (!(x == a && y == b) && _grid[x][y] != 9) {
                _grid[x][y] = 9;
                cnt++;
            }
        } while (cnt < _bombs);


        for (td::INT4 i = 0; i < _width; i++)
            for (td::INT4 j = 0; j < _height; j++)
            {
                td::INT4 n = 0;
                if (_grid[i][j] == 9) 
                    continue;
                if (i < _width - 1 && _grid[i + 1][j] == 9) 
                    n++;
                if (j < _height - 1 && _grid[i][j + 1] == 9) 
                    n++;
                if (i > 0 && _grid[i - 1][j] == 9) 
                    n++;
                if (j > 0 && _grid[i][j - 1] == 9) 
                    n++;
                if (i < _width - 1 && j < _height - 1 && _grid[i + 1][j + 1] == 9) 
                    n++;
                if (j > 0 && i > 0 && _grid[i - 1][j - 1] == 9) 
                    n++;
                if (i > 0 && j < _height - 1 && _grid[i - 1][j + 1] == 9)
                    n++;
                if (j > 0 && i < _width - 1 && _grid[i + 1][j - 1] == 9)
                    n++;
                _grid[i][j] = n;
            }
        reDraw();
    }

    bool setDifficulty(Difficulty d)
    {
        if (_gameStarted)
            gameOver(false);

        _difficulty = d;
        
        switch (d)
        {
            case Difficulty::Easy:
                _bombs = 15;
                _width = 12;
                _height = 12;
                break;
            case Difficulty::Medium:
                _bombs = 40;
                _width = 16;
                _height = 16;
                break;
            case Difficulty::Hard:
                _bombs = 99;
                _width = 32;
                _height = 24;
                break;
        }
        assert(_width <= MaxGridSize);
        assert(_height <= MaxGridSize);
        
        _pStatusBar->setMessage(tr("ClStart"));
        _pStatusBar->setNoOfFlags(0);
        _pStatusBar->setNoOfBombs(_bombs);
        _pStatusBar->setFieldInfo(_width, _height);
        
        gui::Window* wnd = getParentWindow();
        gui::Size sz(_width * _w, _height * _w);
        wnd->adjustSizeToContentSize(sz, _animateDifficultyTransition); //adjust window size to its content size
        loadUserBoard();
        return true;
    }
    
    void getFieldSize(gui::Size& sz)
    {
        sz.width = _width * _w;
        sz.height = _height * _w;
    }

};
