//
//  Created by Izudin Dzafic on 18/10/2025.
//  Copyright © 2025 IDz. All rights reserved.
//

#pragma once
#include "Road.h" 

class Model : public IVisitor
{
public:
    enum class InfoLoc : td::BYTE {TopLeft=0, BottomLeft, TopRight, BottomRight};
    Graph _graph;
    std::vector<Town> _towns;
    std::vector<Road> _roads;
    std::unordered_map<td::String, GraphType> _mapTownIDToNodeID;
    MapTownNameToID _mapTownNameToNodeID;
    std::thread _pathFinder;
    std::mutex _mutex;
    Primitive::Options _options;
    td::UINT4 _pathSegments = 0;
    float _pathDistance = 0;
    GraphType _lastNode = 0;
    bool _goalFound = false;
    float _goalCost = 0;
    GraphType _lastRoadIDToGoalID = 0;  //branch that connects to goalID node
    bool _searching = false;
    InfoLoc _infoLoc = InfoLoc::TopLeft;
    
//    bool _goalFound = false; //no need, can be detected via Status and goal node
protected:
    void resetPath()
    {
        if (_lastNode == 0)
            return;
        for (auto& t : _towns)
        {
            auto status = t.getGUIState();
            if (status == Primitive::Status::OnPath)
            {
                if (t.getID() != _options.goalID)
                    t.setGUIState(Primitive::Status::Visited);
            }
                
        }
        
        for (auto& r : _roads)
        {
            auto status = r.getGUIState();
            if (status == Primitive::Status::OnPath)
                r.setGUIState(Primitive::Status::Visited);
        }
        return;
        //optimized version
        auto nodeID = _lastNode;
        
        while (nodeID != 0)
        {
            auto nodePos = nodeID - 1;
            _towns[nodePos].setGUIState(Primitive::Status::Visited);
            const auto& backTrackTransition = _towns[nodePos].getBackTrackTransition();
            if (backTrackTransition.overBranchID > 0)
                _roads[backTrackTransition.overBranchID - 1].setGUIState(Primitive::Status::Visited);
            
            nodeID = backTrackTransition.toNodeID;
        }
    }
    
    //air distance between any two nodes
    float calcDistance(GraphType nodeA, GraphType nodeB)
    {
        auto fromNode = nodeA - 1;
        auto toNode = nodeB - 1;
        const auto& townFrom = _towns[fromNode];
        const auto& locFrom = townFrom.getLocation();
        const auto& townTo = _towns[toNode];
        const auto& locTo = townTo.getLocation();
        auto dx = math::abs(locFrom.x - locTo.x);
        auto dy = math::abs(locFrom.y - locTo.y);
        float dxKm = float(dx * _options.ratioLong);
        float dyKm = float(dy * _options.ratioLat);
        float distance = std::sqrt(dxKm*dxKm + dyKm*dyKm);
        return distance;
    }
    
    //transition/path length between two connected nodes (using roadID)
    float getTransitionLength(GraphType roadID) const
    {
        const auto& road = _roads[roadID - 1];
        return road.getLength();
    }
  
    float getAirDistance(GraphType nodeID) const
    {
        const Town& t = _towns[nodeID - 1];
        return t.getHeuristicCost();  // Changed from setHeuristicCost()
    }
  
    GraphType getTransitionStartNodeID(const Transition& tr) const
    {
        if (tr.overBranchID == 0)  // Starting node
        {
            assert(false);
            return 0;
        }
        
        const Road& road = _roads[tr.overBranchID - 1];
        GraphType startID = road.getOtherSide(tr.toNodeID);
        return startID;
    }
    
    //get distance to the node that starts the transition
    float getPrevDistance(const Transition& tr) const
    {
        GraphType startID = getTransitionStartNodeID(tr);
        if (startID == 0)
            return math::maxVal<float>();
        
        const Town& t = _towns[startID - 1];
        return t.getCost();
    }
    
    void setPath()
    {
        _pathSegments = 0;
        _pathDistance = 0;
        
        // For UCS and A*, always backtrack from goal if it's been reached
        GraphType nodeID = 0;
        if (_options.algorithm >= Algorithm::UCS)
        {
            const Town& goalTown = _towns[_options.goalID - 1];
            if (_goalFound)
            {
                nodeID = _options.goalID;
            }
            else
            {
                return; // Goal not reached yet
            }
        }
        else
        {
            // For BFS/DFS, use _lastNode
            if (_lastNode == 0)
                return;
            nodeID = _lastNode;
        }
        
        while (nodeID != 0)
        {
            auto nodePos = nodeID - 1;
            _towns[nodePos].setGUIState(Primitive::Status::OnPath);
            const auto& backTrackTransition = _towns[nodePos].getBackTrackTransition();
            if (backTrackTransition.overBranchID > 0)
            {
                ++_pathSegments;
                _pathDistance += getTransitionLength(backTrackTransition.overBranchID);
                _roads[backTrackTransition.overBranchID - 1].setGUIState(Primitive::Status::OnPath);
            }
            nodeID = backTrackTransition.toNodeID;
        }
    }

    
    float getGoalCost() const
    {
        const Town& t = _towns[_options.goalID - 1];
        return t.getCost();
    }
    
    IVisitor::Action isValidTransition(const Transition& tr) override
    {
        GraphType node = tr.toNodeID;
        GraphType nodePos = node - 1;
        Town& t = _towns[nodePos];
        mu::dbgLog("Starting node: %s", t.getName().c_str());
        
        if (node == _options.goalID)
        {
            if (_options.algorithm < Algorithm::UCS)
            {
                _goalFound = true;
                return IVisitor::Action::Stop;
            }
            
            if (tr.overBranchID != _lastRoadIDToGoalID)
                return IVisitor::Action::DoNotMove;
            return IVisitor::Action::OnGoal;
        }
        
        GraphType toNodePos = node - 1;
        
        if (_towns[toNodePos].isExplored())
            return IVisitor::Action::DoNotMove;
        
        return IVisitor::Action::Move;
    }
    
    IVisitor::Action onStartedTransition(const Transition& trans) override
    {
        GraphType toNodeID = trans.toNodeID;
        GraphType toNodePos = toNodeID - 1;
        Town& destinationTown = _towns[toNodePos];
        
        if (_options.algorithm == Algorithm::UCS)
        {
            auto prevPathLength = getPrevDistance(trans);
            auto segLength = getTransitionLength(trans.overBranchID);
            auto newTotalLength = prevPathLength + segLength;
            
            mu::dbgLog("  ---> to node: %s, totalCost=%.1f+%.1f=%.1f ", destinationTown.getName().c_str(), prevPathLength, segLength, newTotalLength);
            
            Transition backTrack {getTransitionStartNodeID(trans), trans.overBranchID};
            Town::CostChange costChange = destinationTown.setCost(newTotalLength, backTrack);
            switch(costChange)
            {
                case Town::CostChange::InitialSet:
                    mu::dbgLog("  -- initil cost set to the destination Town.");
                    break;
                case Town::CostChange::UpdatedWithBetterCost:
                    mu::dbgLog("  -- cost updated with better cost in the destination town.");
                    break;
                case Town::CostChange::UpdateNotPossibleNodeExplored:
                    mu::dbgLog("  -- cost update not possible since the destination town was explored");
                    return IVisitor::Action::DoNotMove;
                    break;
                case Town::CostChange::UpdateNotPossibleDueToLargerCost:
                    mu::dbgLog(" -- cost update not possible since the transition would increase the cost in the destination town");
                    return IVisitor::Action::DoNotMove;
                    break;
            }

            if (toNodeID == _options.goalID)
            {
                if (isGoalFound())
                {
                    float totalPevGoalCost = _goalCost;
                    if (totalPevGoalCost <= newTotalLength)
                    {
                        mu::dbgLog("  -- goal transition rejected due to larger cost");
                        return IVisitor::Action::DoNotMove;
                    }
                    else
                    {
                        _goalCost = newTotalLength;
                        _lastRoadIDToGoalID = trans.overBranchID;
                        const Road& r = _roads[trans.overBranchID -1];
                        mu::dbgLog("  ++ new better goal cost: %.1f, over branch=%s", _goalCost, r.getName().c_str());
                        return IVisitor::Action::Move;
                    }
                }
                else
                {
                    _goalFound = true;
                    _goalCost = newTotalLength;
                    _lastRoadIDToGoalID = trans.overBranchID;
                    const Road& r = _roads[trans.overBranchID -1];
                    mu::dbgLog("  ++ first goal cost: %.1f, over branch=%s", _goalCost, r.getName().c_str());
                    return IVisitor::Action::Move;
                }
            }
            else
            {
                if (isGoalFound())
                {
                    if (_goalCost <= newTotalLength)
                    {
                        mu::dbgLog("  -- transition rejected due to larger cost than the goal cost");
                        return IVisitor::Action::DoNotMove;
                    }
                }
            }
        }
        else if (_options.algorithm == Algorithm::AStar)
        {
            auto prevPathLength = getPrevDistance(trans);
            auto segLength = getTransitionLength(trans.overBranchID);
            auto airDistanceToGoal = getAirDistance(trans.toNodeID);  // Heuristic to goal
            auto newLengthToTransNode = prevPathLength + segLength;
            auto fCost = prevPathLength + segLength + airDistanceToGoal; //Forecasted cost (estimatted length)
            
            mu::dbgLog("  ---> to node: %s, fCost=%.1f+%.1f+%.1f=%.1f ", destinationTown.getName().c_str(), prevPathLength, segLength, airDistanceToGoal, fCost);
            
            Transition backTrack {getTransitionStartNodeID(trans), trans.overBranchID};
            Town::CostChange costChange = destinationTown.setFCost(fCost, newLengthToTransNode, backTrack);
            switch(costChange)
            {
                case Town::CostChange::InitialSet:
                    mu::dbgLog("  -- initil cost set to the destination Town.");
                    break;
                case Town::CostChange::UpdatedWithBetterCost:
                    mu::dbgLog("  -- cost updated with better cost in the destination town.");
                    break;
                case Town::CostChange::UpdateNotPossibleNodeExplored:
                    mu::dbgLog("  -- cost update not possible since the destination town was explored");
                    return IVisitor::Action::DoNotMove;
                    break;
                case Town::CostChange::UpdateNotPossibleDueToLargerCost:
                    mu::dbgLog(" -- cost update not possible since the transition would increase the cost in the destination town");
                    return IVisitor::Action::DoNotMove;
                    break;
            }

            if (toNodeID == _options.goalID)
            {
                if (isGoalFound())
                {
                    float totalPevGoalCost = _goalCost;
                    if (totalPevGoalCost <= fCost)
                    {
                        mu::dbgLog("  -- goal transition rejected due to larger cost");
                        return IVisitor::Action::DoNotMove;
                    }
                    else
                    {
                        _goalCost = fCost;
                        _lastRoadIDToGoalID = trans.overBranchID;
                        const Road& r = _roads[trans.overBranchID -1];
                        mu::dbgLog("  ++ new better goal cost: %.1f, over branch=%s", _goalCost, r.getName().c_str());
                        return IVisitor::Action::Move;
                    }
                }
                else
                {
                    _goalFound = true;
                    _goalCost = fCost;
                    _lastRoadIDToGoalID = trans.overBranchID;
                    const Road& r = _roads[trans.overBranchID -1];
                    mu::dbgLog("  ++ first goal cost: %.1f, over branch=%s", _goalCost, r.getName().c_str());
                    return IVisitor::Action::Move;
                }
            }
            else
            {
                if (isGoalFound())
                {
                    if (_goalCost <= fCost)
                    {
                        mu::dbgLog("  -- transition rejected due to larger cost than the goal cost");
                        return IVisitor::Action::DoNotMove;
                    }
                }
            }
        }
        else
        {
            if (destinationTown.isVisited())
                return IVisitor::Action::DoNotMove;
        }
        
        // For all algorithms, continue exploring, with setting back-track node
        Transition backTrackTrans{0, trans.overBranchID};
        if (trans.overBranchID != 0)
        {
            const auto& branch = _roads[trans.overBranchID - 1];
            const auto& edge = branch.getEdge();
            if (edge.fromNodeID != trans.toNodeID)
                backTrackTrans.toNodeID = edge.fromNodeID;
            else
                backTrackTrans.toNodeID = edge.toNodeID;
        }
        
        destinationTown.setBackTrackTransition(backTrackTrans);
        return IVisitor::Action::Move;
    }
    
    void onCompletedTransition(const Transition& trans, IVisitor::Action nextAction) override
    {
        GraphType toNodePos = trans.toNodeID - 1;
        Town& destinationTown = _towns[toNodePos];
        if (_options.algorithm < Algorithm::UCS)
        {
            Transition backTrackTrans{0, trans.overBranchID};
            if (trans.overBranchID != 0)
            {
                const auto& branch = _roads[trans.overBranchID - 1];
                const auto& edge = branch.getEdge();
                if (edge.fromNodeID != trans.toNodeID)
                    backTrackTrans.toNodeID = edge.fromNodeID;
                else
                    backTrackTrans.toNodeID = edge.toNodeID;
            }
            destinationTown.setBackTrackTransition(backTrackTrans);
            destinationTown.setState(Town::State::Explored);
        }
        
        lock();
        
        destinationTown.setState(Town::State::Explored);
        destinationTown.setGUIState(Primitive::Status::OnPath);
        
        if (trans.overBranchID > 0)
            _roads[trans.overBranchID-1].setGUIState(Primitive::Status::OnPath);
        if (_lastNode != 0)
            resetPath();
        _lastNode = trans.toNodeID;
        setPath();
        unlock();
        if (nextAction != IVisitor::Action::Stop)
            thread::sleepMilliSeconds(_options.sleepMS);
    }
    
    void sortTransitions(GraphType currentNodeID, cnt::PushBackVector<Transition>& transitions) override
    {
        std::sort(transitions.begin(), transitions.end(),
                  [this, currentNodeID](const Transition& a, const Transition& b){
            auto transCost1 = getTransitionLength(a.overBranchID);
            auto estimatedDistance1 = transCost1 + calcDistance(a.toNodeID, _options.goalID);
            
            auto transCost2 = getTransitionLength(b.overBranchID);
            auto estimatedDistance2 = transCost2+ calcDistance(b.toNodeID, _options.goalID);
            
            return estimatedDistance2 < estimatedDistance1;
            
        });
    }
    
    
public:
    
    Model()
    {
        auto pApp = gui::getApplication();
        if (pApp->isDarkMode())
        {
            _options.normalColor = td::ColorID::Gray;
            _options.visitedColor = td::ColorID::SysText;
            _options.pathColor = td::ColorID::Orange;
            _options.txtColor = td::ColorID::Cyan;
        }
        else
        {
            _options.normalColor = td::ColorID::Gray;
            _options.visitedColor = td::ColorID::SysText;
            _options.pathColor = td::ColorID::DarkRed;
            _options.txtColor = td::ColorID::DarkBlue;
        }
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
        _lastNode = 0;
        _pathSegments = 0;
        _pathDistance = 0;
        
        _goalCost = 0;
        _lastRoadIDToGoalID = 0;
        _goalFound = false;
        
        _searching = false;
        
        
        Transition empty{0,0};
        for (auto& town : _towns)
            town.reset();
        
        for (auto& road : _roads)
            road.setGUIState(Primitive::Status::Unvisited);
    }
    
    void clean()
    {
        _towns.clear();
        _roads.clear();
        _mapTownIDToNodeID.clear();
        _mapTownNameToNodeID.clear();
        reset();
    }
    
    bool isEmpty() const
    {
        return (_towns.size() == 0);
    }
    
    void init(unsigned int nTowns)
    {
        _towns.reserve(nTowns);
        _graph.init(this, nTowns);
    }
    
    void reserveRoads(unsigned int nRoads)
    {
        _roads.reserve(nRoads);
    }
    
    void draw(td::MutableString& strInfo)
    {
        lock();
        
        for (const auto& road : _roads)
            road.draw(_towns, _options);
        
        for (const auto& town : _towns)
            town.draw(_options);
        
        const auto& info = _graph.getInfo();
        
        strInfo.appendFormat("\nMemorized Trans = %d\nMax Memorized Trans = %d\nCompleted Trans = %d\nPath Segments=%d\nPath length = %.3f km", info.nMemorizedTransitions, info.nMaxMemorizedTransitions, info.nCompletedTransactions, _pathSegments, _pathDistance);
        
        unlock();
    }
    
    GraphType getNodeID(const td::String& strNodeID) const
    {
        auto it = _mapTownIDToNodeID.find(strNodeID);
        if (it != _mapTownIDToNodeID.end())
            return it->second;
        return 0;
    }
    
    void updateModelSize(const gui::Size& sz)
    {
        _options.viewSize = sz;
        for (auto& town : _towns)
            town.updatePosition(sz);
        
        for (auto& road : _roads)
            road.updatePosition(_towns, sz);
    }
    
    void append(Town& t, const td::String& strTownID)
    {
        GraphType townID = GraphType(_towns.size()+1);
        t.setID(townID);
        _towns.emplace_back(std::move(t));
        
        _mapTownIDToNodeID[strTownID] = townID;
        _mapTownNameToNodeID[t.getName()] = townID;
    }
    
    bool addRoad(const td::String& roadName, const td::String& fromNode, const td::String& toNode, const gui::Size& sz, float cost)
    {
        GraphType fromID = getNodeID(fromNode);
        if (fromID == 0)
            return false;
        
        GraphType toID = getNodeID(toNode);
        if (toID == 0)
            return false;
        if (cost < 0)
            cost = calcDistance(fromID, toID);
        Road r(_towns, roadName, sz, fromID, toID, cost);
        _towns[fromID-1].increementNoOfConnectedRoads();
        _towns[toID-1].increementNoOfConnectedRoads();
        
        _roads.emplace_back(std::move(r));
        GraphType branchID = (GraphType)_roads.size();
        Edge e{fromID, toID};
        _graph.addEdge(e, branchID);
        return true;
    }
    
    void checkTownsWithoutConnection() const
    {
        for (const Town& t : _towns)
        {
            if (t.getNoOfConnectedRoads() == 0)
            {
                mu::dbgLog("Town '%s' is not connected to any other town!!", t.getName().c_str());
            }
        }
    }
    
    std::thread& getThread()
    {
        return _pathFinder;
    }
    
    //this one starts the quest
    //it can be stopped by calling stopSearching()
    bool findPath(const gui::thread::MainThreadSharedFunction& completionCallBack)
    {
        if (_searching)
            return true;
        
        GraphType fromID = _options.startID; //getNodeID(fromNode);
        if (fromID == 0)
            return false;
        
        GraphType toID = _options.goalID; //getNodeID(toNode);
        if (toID == 0)
            return false;
        
        reset(); //reset graph (
        _graph.setGoalNode(_options.goalID);
        
        _searching = true;
        _pathFinder = std::thread([this, completionCallBack]()
            {
            switch (_options.algorithm)
            {
                case Algorithm::BFS:
                    _graph.BFS(_options.startID, _options.goalID);
                    break;
                case Algorithm::DFS:
                    _graph.DFS(_options.startID, _options.goalID);
                    break;
                case Algorithm::DFSWithHeuristics:
                    _graph.DFSWithHeuristics(_options.startID);
                    break;
                
                case Algorithm::GreedyBestFS:
                case Algorithm::AStar:
                    calcHeuristics();
                case Algorithm::UCS:
                {
                    auto cmp = [this](const Transition& a, const Transition& b)
                    {
                        auto nodeIDa = a.toNodeID;
                        const Town& townA = _towns[nodeIDa - 1];
                        
                        auto nodeIDb = b.toNodeID;
                        const Town& townB = _towns[nodeIDb - 1];
                        
                        // For A*, use f-cost; for UCS, use g-cost (stored in _pathDistance)
                        if (_options.algorithm == Algorithm::AStar)
                            return townA.getFCost() > townB.getFCost();
                        else if (_options.algorithm == Algorithm::UCS)
                            return townA.getCost() > townB.getCost();
                        else //Gready Best FS
                            return townA.getHeuristicCost() > townB.getHeuristicCost();
                            
                    };
                    
                    _graph.BestFS(_options.startID, cmp);
                }
                break;
                    
                default:
                    assert(false);
            }
            _searching = false;
            gui::thread::asyncExecInMainThread(completionCallBack);
        });
        return true;
    }
    
    void stopSearching()
    {
        _graph.stopSearching();
    }
    
    Primitive::Options& getOptions()
    {
        return _options;
    }
    
    const MapTownNameToID& getMapNameToID() const
    {
        return _mapTownNameToNodeID;
    }
    
    void setDistanceRatios(float rLat, float rLong)
    {
        _options.ratioLat = rLat;
        _options.ratioLong = rLong;
    }
    
    InfoLoc getInfoLocation() const
    {
        return _infoLoc;
    }
    
    void setInfoLocPosition(const td::String& strLoc)
    {
        _infoLoc = InfoLoc::TopLeft;
        if (strLoc.length() != 2)
            return;
        if (strLoc.compare("BR", 2))
            _infoLoc = InfoLoc::BottomRight;
        else if (strLoc.compare("BL", 2))
            _infoLoc = InfoLoc::BottomLeft;
        else if (strLoc.compare("TR", 2))
            _infoLoc = InfoLoc::TopRight;
    }
    
    void calcHeuristics()
    {
        GraphType goalTownID = _options.goalID;
        GraphType townID = 0;
        for (Town& t : _towns)
        {
            ++townID;
            if (townID == goalTownID)
            {
                t.setHeuristicCost(0);
                continue;
            }
            float airDistance = calcDistance(townID, goalTownID);
            t.setHeuristicCost(airDistance);
        }
    }
    
    bool isSearching() const
    {
        return _searching;
    }
    
    bool isGoalFound() const
    {
        return _goalFound;
    }
};

