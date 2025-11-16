//
//  Created by Izudin Dzafic on 18/10/2025.
//  Copyright © 2025 IDz. All rights reserved.
//

#pragma once
#include <thread>
#include <mutex>
#include <gui/Thread.h>
#include <thread/Thread.h>
#include <gui/Image.h>
#include <vector>
#include <stack>
#include <cmath>
#include <random>
#include <chrono>
#include <gui/Shape.h>
#include <gui/Application.h>

constexpr size_t N = 8;

class QueenModel
{
public:
    class Options
    {
    public:
        gui::Size viewSize;
        float speed = 0.5f;
        int delayAfterPlacementInMs = 1000;
        bool playSoundOnQueenPlacement = true;
        bool generateRandomSolution = true;
        QueenColor queenColor = QueenColor::CellReverse;
        Options()
        {
            auto appProperties = gui::getApplication()->getProperties();
            queenColor = appProperties->getValue("queenColor", queenColor);
            playSoundOnQueenPlacement = appProperties->getValue("playSound", playSoundOnQueenPlacement);
            generateRandomSolution = appProperties->getValue("randomSolution", generateRandomSolution);
            speed = appProperties->getValue("speed", speed);
            setSpeed(speed);
        }
        
        void setSpeed(float speedIn)
        {
            //speedIn is in range 0 to 1
            speed = speedIn;
            delayAfterPlacementInMs = cMinDelayInMs + (cMaxDelayInMs - cMinDelayInMs) * (1-speed);
        }
    };
    
protected:
    class State
    {
        td::LUINT8 _queens = 0;  //64 bit unsigned integer instead of vector
        int _row = 0;
    public:
        // Add a queen at the given column position for the current row
        void addQueen(int columnPos)
        {
            assert(columnPos < N);
            td::LUINT8 pos = td::LUINT8((columnPos) & 0xFF); // use 8 bits
            pos <<= (_row * 8); // shift to the row's position
            _queens |= pos;
            ++_row;
        }

        // Get the column of a queen at a given row
        int getQueenColumn(int r) const
        {
            int col = int((_queens >> (r * 8)) & 0xFF);
            assert(col < N);
            return col;
        }
        
        int getRow() const
        {
            return _row;
        }
        
        void clean()
        {
            _queens = 0;
            _row = 0;
        }
        
        bool isSafe(int row, int col) const
        {
            for (int i = 0; i < row; ++i)
            {
                int qCol = getQueenColumn(i);
                if (qCol == col || abs(qCol - col) == abs(i - row))
                    return false;
            }
            return true;
        }
    };
    
    State _currentState; //this one will be used for drawing and needs to be protected by mutex
    gui::Image _board;
    gui::Image _blackQueen;
    gui::Image _whiteQueen;
    std::thread _workingThread;
    std::mutex _mutex;
    gui::Rect _boardPlacement;
    gui::Size _boardImgSize;
    const float _originXMargin = 0.045f;   //relative to biard width, after fine-tuning
    const float _originYMargin = 0.045f;    //relative to board height
    Options _options;
    
    bool _searching = false;
    bool _stopSearching = false;
    bool _calcBoardPlacement = true;

protected:
    // Check if placing a queen at (row, col) is safe
    bool isSafe(const std::vector<int>& queens, int row, int col)
    {
        for (int i = 0; i < row; ++i)
        {
            int qCol = queens[i];
            if (qCol == col || math::abs(qCol - col) == math::abs(i - row))
                return false;
        }
        return true;
    }
    
    std::vector<int> getRandomColons(int N)
    {
        // Create vector with numbers 0..N-1
        std::vector<int> vec(N);
        for (int i = 0; i < N; ++i) {
            vec[i] = i;
        }

        // Seed with a real random value, different each run
        static std::random_device rd;
        static std::mt19937 gen(rd());

        // Shuffle vector randomly
        std::shuffle(vec.begin(), vec.end(), gen);

        return vec;
    }
    
    void mainSearch(const gui::thread::MainThreadSharedFunction1& completionCallBack)
    {
        //set seed (not to start always with the same combinations)
        unsigned seed = unsigned(std::chrono::system_clock::now().time_since_epoch().count());
        std::mt19937 gen(seed);  // Mersenne Twister seeded differently each run
        
        //use DFS for search
        std::stack<State> frontier;
        State initialState; // start: empty board, row 0, defauot constructor of State
        frontier.push(initialState);
        
        size_t prevRow = 0;
        while (!frontier.empty())
        {
            //protect _currentState
            _mutex.lock();
            _currentState = frontier.top();
            _mutex.unlock();
            
            frontier.pop();
            
            auto timeToSleepInMS = _options.delayAfterPlacementInMs;
            int currentRow = _currentState.getRow();
            if (currentRow != 0)
            {
                //generate sound
                td::Variant param;
                if (currentRow < prevRow)
                {
                    param = td::INT4(Message::PlayPiecesCollected);
                    gui::thread::asyncExecInMainThread(completionCallBack, param); //async call main thread
                    thread::sleepMilliSeconds(timeToSleepInMS/5);
                    timeToSleepInMS = 4*timeToSleepInMS/5;
                }
                prevRow = currentRow;
                param = td::INT4(Message::PlayQueenPlaced);
                gui::thread::asyncExecInMainThread(completionCallBack, param); //async call main thread
            }
            
            if (currentRow == N)
            {
                //cout << "✅ Found solution:\n";
                return; // stop after first solution
            }
            
            if (_stopSearching)
                return;
            
            //take some rest :-)
            thread::sleepMilliSeconds(timeToSleepInMS);

            if (_stopSearching)
                return;
            
            // Try placing queens in columns (DFS: push later options first)
            if (_options.generateRandomSolution)
            {
                auto cols = getRandomColons(N);
                for (int col : cols)
                {
                    if (_currentState.isSafe(currentRow, col))
                    {
                        State next = _currentState;
//                        next.queens.push_back(col);
                        next.addQueen(col);
//                        next.row++;
                        frontier.push(next);
                    }
                }
            }
            else
            {
                for (int col = N - 1; col >= 0; --col)
                {
                    if (_currentState.isSafe(currentRow, col))
                    {
                        State next = _currentState;
                        next.addQueen(col);
//                        next.queens.push_back(col);
//                        next.row++;
                        frontier.push(next);
                    }
                }
            }
            if (_stopSearching)
                return;
        }
    }
    
public:
    QueenModel()
    : _board(":board")
    , _blackQueen(":blackQueen")
    , _whiteQueen(":whiteQueen")
    {
        reset();
        _board.getSize(_boardImgSize);
    }
    
    void lock()
    {
        _mutex.lock();
    }
    
    void unlock()
    {
        _mutex.unlock();
    }
    
    void reset()
    {
        _currentState.clean();
        _stopSearching = false;
    }
    
    void draw()
    {
        gui::Point pt(0,0);
        gui::Rect r(pt, _options.viewSize);
        if (_calcBoardPlacement)
            _board.draw(r, gui::Image::AspectRatio::Keep, td::HAlignment::Center, td::VAlignment::Center, &_boardPlacement);
        else
            _board.draw(r);
        
        gui::CoordType origX = _boardPlacement.left + _originXMargin * _boardPlacement.width();
        gui::CoordType origY = _boardPlacement.bottom - _originYMargin * _boardPlacement.height();
        
        gui::CoordType matrixRight = _boardPlacement.right - _originXMargin * _boardPlacement.width();
        gui::CoordType matrixTop = _boardPlacement.top + _originYMargin * _boardPlacement.height();
        
        gui::Rect rMatrix(origX, matrixTop, matrixRight, origY);
        if (_currentState.getRow() == N)
            gui::Shape::drawRect(rMatrix, td::ColorID::Green, 5.5f); //we have solution, show it in green
        else
            gui::Shape::drawRect(rMatrix, td::ColorID::White, 2.5f);  //otherwise use white
        
        gui::CoordType cellWidth = (matrixRight - origX) / N;
        gui::CoordType cellHeight = (origY - matrixTop) / N;
        
        gui::Point ptOrig(origX, origY - cellHeight);
        gui::Size cellSize(cellWidth, cellHeight);

        lock();
        //worker thread cannot change _current state until we complete drawing
        //we should be fast here
        int lastRow = _currentState.getRow();
        for (int row = 0; row < lastRow; ++row)
        {
            //just one loop now
            int col = _currentState.getQueenColumn(row);
            
            gui::Point queenTL(ptOrig.x + col*cellWidth, ptOrig.y - row*cellHeight);
            gui::Rect cellRect(queenTL, cellSize);

            switch (_options.queenColor)
            {
                case QueenColor::Black: _blackQueen.draw(cellRect); break;
                case QueenColor::White: _whiteQueen.draw(cellRect); break;
                case QueenColor::CellReverse:
                    if ((row + col) % 2 == 1)
                        _blackQueen.draw(cellRect);
                    else
                        _whiteQueen.draw(cellRect);
                    break;
                default: assert(false);
            }
        }
        unlock();
    }
    
    std::thread& getThread()
    {
        return _workingThread;
    }
    
    //this one starts the quest
    //it can be stopped by calling stopSearching()
    void solve(const gui::thread::MainThreadSharedFunction1& completionCallBack)
    {
        if (_searching)
            return;

        reset(); //reset the problem state
        
        _searching = true;
        //start worker thread
        _workingThread = std::thread([this, completionCallBack]()
        {
            mainSearch(completionCallBack);
            _searching = false;
            td::Variant param = td::INT4(Message::Stop); //no solution
            if (_currentState.getRow() == N)
                param = td::INT4(Message::StopOnGoal); //solution found
            
            gui::thread::asyncExecInMainThread(completionCallBack, param);
        });
    }
    
    void stopSearching()
    {
        _stopSearching = true;
    }
    
    QueenModel::Options& getOptions()
    {
        return _options;
    }
    
    bool isSearching() const
    {
        return _searching;
    }
    
    void updateModelSize(const gui::Size& newSize)
    {
        _calcBoardPlacement = true;
        _options.viewSize = newSize;
    }
};

