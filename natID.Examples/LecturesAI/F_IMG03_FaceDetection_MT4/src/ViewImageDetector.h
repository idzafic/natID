//
//  Created by Izudin Dzafic on 18/10/2025.
//  Copyright © 2025 IDz. All rights reserved.
//
#pragma once
#include <gui/Canvas.h>
#include <gui/Sound.h>
#include <gui/Texture.h>
#include <gui/Image.h>
#include <gui/Shape.h>
#include <rnd/MinMax.h>
#include <gui/ProgressIndicator.h>
#include <gui/Application.h>
#include <gui/Thread.h>
#include <gui/DrawableProgressIndicator.h>
#include <mu/Timer.h>
#include <mutex>

class ViewImageDetector : public gui::Canvas
{
protected:
    std::thread _workingThreads[4];
    std::mutex _mutex; //protected shared res
    gui::Size _size;
    gui::Image _imgWatters;
    gui::Texture _textureWithoutNoise;
    gui::Texture _textureForDetection;
    gui::Texture _watters;
    gui::Texture _merlin;
    gui::Texture _hetfield;
    cnt::PushBackVector<td::Point<td::INT4>> _kernelPoints;
    std::multimap<float, td::Point<td::INT4>> _matchesAboveTreshold;
    td::Point<td::INT4> _realFacePositions[3];
    gui::Point _tl = {10,10};
    gui::ProgressIndicator* _progIndicator = nullptr;
    std::function<void()> _fnUpdateMenuAndTB;
    gui::DrawableProgressIndicator _shapeIndicators[4] = {{ {70, 70}, gui::DrawableProgressIndicator::Type::FillCircle}, {{70, 70}, gui::DrawableProgressIndicator::Type::FillCircle},
        {{70, 70}, gui::DrawableProgressIndicator::Type::FillCircle}, {{70, 70}, gui::DrawableProgressIndicator::Type::FillCircle}};
    mu::Timer<> _timer;
    float _brightness = cBrightness;       //negative vales: darker, positive values lighter
    float _correlationTreshold = cCorrelatoinTreshold;  //correlations above this will be taken into consideration
    float _finalTreshold = cFinalTreshold;    //correlations which have value > _finalTreshold * avg_correlations are faces
    int _maxRndImages = cMaxRndImages;
    int _nWorkingThreads = 0;
    td::BYTE _noiseLevel = cNoiseLevel;   //noise level from 0 to 128
    bool _detectionInProgress = false;
    bool _bShowRealLocations = false;
    bool _stopDetectionProcess = false;
protected:
    
    void onResize(const gui::Size& newSize) override
    {
        _size = newSize;
    }
    
    void onDraw(const gui::Rect& rect) override
    {
        gui::Point textureTL(_tl);
        _textureForDetection.draw(textureTL);
        textureTL.x += 10 + _textureForDetection.getWidth();
        gui::Size szWatter;
        _imgWatters.getSize(szWatter);
        
        gui::Rect rectWatters(textureTL, szWatter);
        _imgWatters.draw(rectWatters);
        
        textureTL.y += szWatter.height + 10;
        _watters.draw(textureTL);
        
        _watters.getSize(szWatter);
        auto x = textureTL.x;
        textureTL.y += szWatter.height + 10;
        _shapeIndicators[0].draw(textureTL);
        
        textureTL.x += _shapeIndicators[0].getSize().width + 10;
        _shapeIndicators[1].draw(textureTL);
        
        textureTL.x = x;
        textureTL.y += _shapeIndicators[0].getSize().height + 10;
        _shapeIndicators[2].draw(textureTL);
        
        textureTL.x += _shapeIndicators[0].getSize().width + 10;
        _shapeIndicators[3].draw(textureTL);
        
        if (_bShowRealLocations)
        {
            for (int i=0; i<3; ++i)
            {
                gui::Size sz;
                _watters.getSize(sz);
                gui::Point ptRect= _realFacePositions[i] + _tl;
                gui::Rect r(ptRect, sz);
                gui::Shape::drawRect(r, td::ColorID::Green, 2);
            }
        }
        
        //draw detected faces
        for (const auto& pt : _kernelPoints)
        {
            gui::Size sz;
            _watters.getSize(sz);
            gui::Point ptRect= pt+_tl;
            gui::Rect r(ptRect, sz);
            gui::Shape::drawRect(r, td::ColorID::Yellow, 2);
        }
    }
    
    void fillTextureWithRandomFaces(const gui::Texture textures[2], size_t nTimes)
    {
        // Use a high-quality random number generator (Mersenne Twister)
        // Seed the generator once outside the loop for better randomness
        
        const int destWidth = _textureWithoutNoise.getWidth();
        const int destHeight = _textureWithoutNoise.getHeight();

        for (size_t i = 0; i < nTimes; ++i)
        {
            const gui::Texture& sourceTexture = (i % 2 == 0) ? textures[0] : textures[1];
            
            const int sourceWidth = sourceTexture.getWidth();
            const int sourceHeight = sourceTexture.getHeight();
            
            // 1. Calculate the valid range for the random position.
            // The top-left corner (pos.x, pos.y) must allow the texture to fit.
            // Max valid X = destination width - source width
            const int maxX = destWidth - sourceWidth;
            // Max valid Y = destination height - source height
            const int maxY = destHeight - sourceHeight;

            // If the source texture is larger than the destination, the valid range is negative.
            if (maxX < 0 || maxY < 0)
            {
                // Cannot place this texture fully. You might choose to skip,
                // or place it at (0, 0) and let 'replace' handle the clipping.
                // We'll skip for a true 'fit'.
                continue;
            }

            // 2. Define the distributions for X and Y coordinates
            // The range is inclusive: [0, maxX] and [0, maxY]
            rnd::MinMax<int> distX(0, maxX);
            rnd::MinMax<int> distY(0, maxY);

            // 3. Generate random coordinates
            td::INT4 randomX = distX.next();
            td::INT4 randomY = distY.next();
            
            // 4. Perform the replacement
            _textureWithoutNoise.replace({randomX, randomY}, sourceTexture);
        }
    }
    
    void extractTopMatches(std::multimap<float, td::Point<td::INT4>>& matches)
    {
        if (matches.empty())
            return;

        // Copy matches sorted descending
        std::vector<std::pair<float, td::Point<td::INT4>>> sortedMatches;
        for (auto it = matches.rbegin(); it != matches.rend(); ++it)
            sortedMatches.push_back(*it);

//        std::vector<td::Point<td::INT4>> results;
        if (_kernelPoints.capacity() == 0)
            _kernelPoints.reserve(5);
        _kernelPoints.reset();
        
        _kernelPoints.push_back(sortedMatches[0].second);

        float prevDelta = 0.f;
        float sumDelta = 0.f;
        size_t countDelta = 0;

        for (size_t i = 1; i < sortedMatches.size(); ++i)
        {
            float delta = sortedMatches[i-1].first - sortedMatches[i].first;

            // Compute average of previous deltas
            float avgDelta = countDelta > 0 ? sumDelta / countDelta : delta;

            // Detect jump: delta significantly bigger than previous deltas
            if (delta > _finalTreshold * avgDelta && i > 1)
                break;

            _kernelPoints.push_back(sortedMatches[i].second);

            sumDelta += delta;
            ++countDelta;
        }

        return;
    }

    void copyToWorkingTexture()
    {
        _textureForDetection.copyFrom(_textureWithoutNoise);
        _textureForDetection.replace(_realFacePositions[0], _watters);
        _textureForDetection.replace(_realFacePositions[1], _watters);
        _textureForDetection.replace(_realFacePositions[2], _watters);
        _textureForDetection.addNoise({_noiseLevel, _noiseLevel, _noiseLevel, 0});
        _textureForDetection.adjustBrightness(_brightness);
    }
    
    double getOVerallProgress() const
    {
        double sum = 0;
        for (int i=0; i<4; ++i)
            sum += _shapeIndicators[i].getValue();
        return sum / 4.;
    }
    
    void detectFaces(int quarter)
    {
        //test correlations
        gui::Size textureSize;
        auto nW = _textureForDetection.getWidth();
        auto nH = _textureForDetection.getHeight();
        
        auto nRows = nH; //-_watters.getHeight();
        auto nCols = nW;
        td::INT4 iRowStart = 0;
        td::INT4 iColStart = 0;
        td::INT4 lastRow = nH - _watters.getHeight();
        td::INT4 lastCol = nW - _watters.getWidth();
        td::INT4 midRow = (nH - _watters.getHeight()) / 2;
        td::INT4 midCol = (nW - _watters.getWidth()) / 2;
        
        switch (quarter)
        {
            case 0: //top-left corent
                nRows = midRow;
                nCols = midCol;
                break;
            case 1:
                iColStart = midCol;
                nRows = midRow;
                nCols = lastCol;
                break;
            case 2:
                iRowStart = midRow;
                nRows = lastRow;
                nCols = midCol;
                break;
            case 3:
                iRowStart = midRow;
                iColStart = midCol;
                nRows = lastRow;
                nCols = lastCol;
                break;
        }
        
        //calculate correllation
        for (td::INT4 iRow = iRowStart; iRow < nRows; ++iRow)
        {
            if (_stopDetectionProcess)
                break;
            
            //update progress indicator
            
            double percentDone = double(iRow-iRowStart) / (nRows-iRowStart);
            gui::thread::asyncExecInMainThread([this, percentDone, quarter](){
                //this code will be executed in Main Thread
                if (_progIndicator)
                {
                    auto overallProgress = getOVerallProgress();
                    _progIndicator->setValue(percentDone);
                }
                    
                _shapeIndicators[quarter].setValue(percentDone);
                reDraw();
            });
            
            for (td::INT4 iCol=iColStart; iCol < nCols; ++iCol)
            {
                td::Point pt(iCol, iRow);
                float f = _textureForDetection.calculateNormalizedCrossCorrelation(pt, _watters);
                if (f >= _correlationTreshold)
                {
                    std::scoped_lock lock(_mutex); //protect acces to matchesAboveTreshold
                    _matchesAboveTreshold.insert({f, pt});
                }
            }
        }
        
        bool bFinalDetectionInThisThread = false;
        {
            std::scoped_lock lock(_mutex); //protect acces to number of _nWorkingThreads threads
            --_nWorkingThreads;
            if (_nWorkingThreads == 0)
                bFinalDetectionInThisThread = true;
        }
        
        if (bFinalDetectionInThisThread)
        {
            if (!_stopDetectionProcess)
            {
                //only last running thread comes here
                //no need for mutex (this is the only running thread
                size_t nMatches = _matchesAboveTreshold.size();
                if (nMatches > 3)
                    extractTopMatches(_matchesAboveTreshold);
                else
                {
                    if (nMatches > 0)
                    {
                        _kernelPoints.reserve(nMatches);
                        for (auto it : _matchesAboveTreshold)
                            _kernelPoints.push_back(it.second);
                    }
                }
            }
            
            _matchesAboveTreshold.clear(); //clear above treshold
            
            //thread completed, inform MT (GUI)
            gui::thread::asyncExecInMainThread([this, quarter](){
                //this code will be executed in Main Thread
                if (!_stopDetectionProcess)
                {
                    if (_progIndicator)
                        _progIndicator->setValue(1.0);
                    _shapeIndicators[quarter].setValue(1.0);
                    _timer.pause();
                }
                else
                    _stopDetectionProcess = false; //reset it for the next start
                
                _workingThreads[quarter].join();
                _bShowRealLocations = true;
                _detectionInProgress = false;
                _fnUpdateMenuAndTB();
                reDraw();
            });
        }
        else
        {
            //thread completed, inform MT (GUI)
            gui::thread::asyncExecInMainThread([this, quarter](){
                //this code will be executed in Main Thread
                if (!_stopDetectionProcess)
                {
                    _shapeIndicators[quarter].setValue(1.0);
                    _timer.pause();
                }
                _workingThreads[quarter].join();

                reDraw();
            });
        }
  
    }
    
    void resetIndicators()
    {
        if (_progIndicator)
            _progIndicator->setValue(0.0);
        
        for (int i=0; i<4; ++i)
        {
            _shapeIndicators[i].setValue(0.0);
        }
    }
public:
    //enable mouse clicks, drags, etc..
    ViewImageDetector(const std::function<void()>& fnUpdateMenuAndTB)
    : gui::Canvas({gui::InputDevice::Event::Zoom}) //scrolling and zooming without scrollbars
    , _imgWatters(":watters")
    , _textureWithoutNoise(cTextureSize, {0,0,0,255})
    , _textureForDetection(cTextureSize, {0,0,0,255})
    , _watters(":watters")
    , _merlin(":merlin")
    , _hetfield(":hetfield")
    , _fnUpdateMenuAndTB(fnUpdateMenuAndTB)
    {
        gui::Application* pApp = getApplication();
        auto appProperties = pApp->getProperties();
        assert (appProperties);
        
        _noiseLevel = appProperties->getValue("noise", _noiseLevel);
        _brightness = appProperties->getValue("brightness", _brightness);
        _correlationTreshold = appProperties->getValue("tresCorr", _correlationTreshold);
        _finalTreshold = appProperties->getValue("tresFinal", _finalTreshold);
        _maxRndImages = appProperties->getValue("maxRndImages", _maxRndImages);
        enableResizeEvent(true); //required to obtain events whenever this view resizes (see onResize method in this class)
        reset();
        setClipsToBounds();
        registerForScrollEvents(); //Event::Zoom must be enabled, otherwise no scroll events
    }
    
    void startStop()
    {
        _timer.reset();
        if (_detectionInProgress)
        {
            _stopDetectionProcess = true;
            return;
        }
        
        _timer.measure();
        _nWorkingThreads = 4;
        _matchesAboveTreshold.clear(); //clear previous
        //run 4 working threads (face detection threads)
        for (int i=0; i<4; ++i)
            _workingThreads[i] = std::thread(&ViewImageDetector::detectFaces, this, i);
        
        _detectionInProgress = true;
        _fnUpdateMenuAndTB();
    }
    
    bool isDetecting() const
    {
        return _detectionInProgress;
    }
    
    void modifyCase(td::BYTE noise, float brightness)
    {
        if (_detectionInProgress)
            return;
        
        _noiseLevel = noise;
        _brightness = brightness;
        
        _kernelPoints.reset();
        _bShowRealLocations = false;
        
        resetIndicators();
        
        copyToWorkingTexture();
        reDraw();
    }
    
    void modifySolutionTresholds(float correlationTreshold, float finalTreshold)
    {
        if (_detectionInProgress)
            return;
        
        _correlationTreshold = correlationTreshold;
        _finalTreshold = finalTreshold;
        _kernelPoints.reset();
        _bShowRealLocations = false;

        resetIndicators();
        reDraw();
    }
    
    void reset(int maxRndImages = -1)
    {
        if (_detectionInProgress)
            return;
        
        if (maxRndImages >= 0)
            _maxRndImages = maxRndImages;
        _kernelPoints.reset();
        _bShowRealLocations = false;
        rnd::MinMax<td::INT4> xCoordGen(0, _textureWithoutNoise.getWidth() - _watters.getWidth());
        rnd::MinMax<td::INT4> yCoordGen(0, _textureWithoutNoise.getHeight() - _watters.getHeight());
        for (int i=0;i<3;++i)
        {
            _realFacePositions[i].x = xCoordGen.next();
            _realFacePositions[i].y = yCoordGen.next();
        }
        
        gui::Texture faces[2] = {_merlin, _hetfield};
        _textureWithoutNoise.clean({0,0,0,255});
        fillTextureWithRandomFaces(faces, _maxRndImages);
        copyToWorkingTexture();
        
        resetIndicators();
        reDraw();
    }
    
    int getNumberOfDetectedFaces() const
    {
        return int(_kernelPoints.size());
    }
    
    void setProgressIndicator(gui::ProgressIndicator* progIndicator)
    {
        _progIndicator = progIndicator;
    }
    
    double getTimeInSec() const
    {
        return _timer.getTimeInSec();
    }
};
