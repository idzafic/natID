//
//  Created by Izudin Dzafic on 18/10/2025.
//  Copyright © 2025 IDz. All rights reserved.
//

#pragma once
#include "IVisitor.h"
#include <vector>
#include <stack>
#include <queue>
#include <cstdint>
#include <cnt/SafeFullVector.h>

class Graph
{
public:
    using Info = struct info
    {
        td::UINT4 nMemorizedTransitions = 0;
        td::UINT4 nCompletedTransactions = 0;
        td::UINT4 nMaxMemorizedTransitions = 0;
        void clean()
        {
            nMemorizedTransitions = 0;
            nCompletedTransactions = 0;
            nMaxMemorizedTransitions = 0;
        }
    };
    
protected:
    IVisitor* _pVisitor;
    cnt::SafeFullVector<std::vector<Transition>> _edges;
    Info _info;
    GraphType _goalID = 0;
    
    bool _isDirected;
    bool _stopSearching = false;
public:
    Graph()
    : _pVisitor(nullptr)
    , _isDirected(false)
    {
    }
    
    Graph(IVisitor* pVisitor, size_t nVertices, bool directed = false)
    {
        init(pVisitor, nVertices, directed);
    }
    
    void setGoalNode(GraphType goalNode)
    {
        _goalID = goalNode;
    }
    
    bool isEmpty() const
    {
        return (_edges.size() == 0);
    }
    
    void addEdge(const Edge& e, GraphType branchID)
    {
        assert(e.fromNodeID > 0);
        assert(e.toNodeID > 0);
        assert(branchID > 0);
        
        assert(e.fromNodeID != e.toNodeID);
        
        Transition t{e.toNodeID, branchID};
        _edges[e.fromNodeID-1].push_back(t);
        if (!_isDirected)
        {
            t.toNodeID = e.fromNodeID;
            _edges[e.toNodeID - 1].push_back(t);
        }
    }
    
    void init(IVisitor* pVisitor, size_t vertices, bool directed = false)
    {
        _pVisitor = pVisitor;
        _edges.reserve(vertices);
        _isDirected = directed;
    }
    
    const Graph::Info& getInfo() const
    {
        return _info;
    }
    
protected:
    //returns false when stop is required
    template <class TMEMCONTAINER>
    bool processTransition(TMEMCONTAINER& memContainer, const Transition& currentTransition)
    {
        auto action = _pVisitor->isValidTransition(currentTransition);
        if (action == IVisitor::Action::OnGoal)
        {
            _pVisitor->onCompletedTransition(currentTransition, IVisitor::Action::OnGoal);
            return true;
        }
        else if (action == IVisitor::Action::Stop)
        {
            _pVisitor->onCompletedTransition(currentTransition, action);
            return false;
        }
        else if (action == IVisitor::Action::Move)
        {
            for (const auto& transition : _edges[currentTransition.toNodeID-1])
            {
                if (_stopSearching)
                    return false;
                
                auto action = _pVisitor->onStartedTransition(transition);
                if (action == IVisitor::Action::Move)
                {
                    //if (transition.toNodeID != _goalID)
                    {
                        memContainer.push(transition);
                        _info.nMemorizedTransitions++;
                        _info.nMaxMemorizedTransitions = math::Max(_info.nMaxMemorizedTransitions, _info.nMemorizedTransitions);
                    }
                }
            }
            
            _info.nCompletedTransactions++;
            _pVisitor->onCompletedTransition(currentTransition, IVisitor::Action::Move);
        }
        return true;
    }
    
public:
    void BFS(GraphType start, GraphType end)
    {
        _info.clean();
        
        if (_edges.size() == 0)
            return;
        std::queue<Transition> frontier;
        Transition startingTransition{start, 0};
        frontier.push(startingTransition);
        _info.nMemorizedTransitions = 1;
        _info.nMaxMemorizedTransitions = 1;
        _stopSearching = false;
        
        while (!frontier.empty())
        {
            auto currentTransition = frontier.front();
            frontier.pop();
            _info.nMemorizedTransitions--;
            
            if (!processTransition(frontier, currentTransition))
                return;
        }
    }

    void DFS(GraphType start, GraphType end)
    {
        _info.clean();
        
        if (_edges.size() == 0)
            return;
        
        std::stack<Transition> frontier;
        Transition startingTransition{start, 0};
        frontier.push(startingTransition);
        _info.nMemorizedTransitions = 1;
        _info.nMaxMemorizedTransitions = 1;
        _stopSearching = false;

        while (!frontier.empty())
        {
            auto currentTransition = frontier.top();
            frontier.pop();
            if (!processTransition(frontier, currentTransition))
                return;
        }
    }
    
    //just for demonstration with vector + sort (later use priority queue)
    void DFSWithHeuristics(GraphType start)
    {
        _info.clean();
        
        if (_edges.size() == 0)
            return;
        
        std::stack<Transition> frontier;
        Transition startingTransition{start, 0};
        frontier.push(startingTransition);
        _info.nMemorizedTransitions = 1;
        _info.nMaxMemorizedTransitions = 1;
        _stopSearching = false;
        
        cnt::PushBackVector<Transition> availableTransitions;
        availableTransitions.reserve(16); //should be equal to breadth to eliminate resizing

        while (!frontier.empty())
        {
            auto currentTransition = frontier.top();
            frontier.pop();

            auto action = _pVisitor->isValidTransition(currentTransition);
            if (action == IVisitor::Action::Stop)
            {
                _pVisitor->onCompletedTransition(currentTransition, action);
                return;
            }
            else if (action == IVisitor::Action::Move)
            {
                availableTransitions.reset(); //we are not deleting it, just reseting
                for (const auto& transition : _edges[currentTransition.toNodeID-1])
                {
                    if (_stopSearching)
                        return;
                    
                    auto action = _pVisitor->onStartedTransition(transition);
                    if (action == IVisitor::Action::Move)
                    {
                        //instead on stack, we keep it in local buffer first
                        availableTransitions.push_back(transition);
                        _info.nMemorizedTransitions++;
                        _info.nMaxMemorizedTransitions = math::Max(_info.nMaxMemorizedTransitions, _info.nMemorizedTransitions);
                    }
                }
                
                //call visition to sort transitions based on heuristics
                if (!availableTransitions.isEmpty())
                {
                    _pVisitor->sortTransitions(currentTransition.toNodeID, availableTransitions);
                    for (const auto& t : availableTransitions)
                        frontier.push(t);
                }
                
                _info.nCompletedTransactions++;
                _pVisitor->onCompletedTransition(currentTransition, IVisitor::Action::Move);
            }
        }
    }
    
    template <typename CompareTrans>
    void BestFS(GraphType start, CompareTrans compareTrans)
    {
        _info.clean();
        
        if (_edges.size() == 0)
            return;
        
        std::priority_queue<Transition, std::vector<Transition>, CompareTrans> frontier(compareTrans);
        
        Transition startingTransition{start, 0};
        frontier.push(startingTransition);
        _info.nMemorizedTransitions = 1;
        _info.nMaxMemorizedTransitions = 1;
        _stopSearching = false;

        while (!frontier.empty())
        {
            auto currentTransition = frontier.top();
            frontier.pop();

            if (!processTransition(frontier, currentTransition))
                return;
        }
    }
    
    void stopSearching()
    {
        _stopSearching = true;
    }
};
