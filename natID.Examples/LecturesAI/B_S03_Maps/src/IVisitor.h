//
//  Created by Izudin Dzafic on 18/10/2025.
//  Copyright © 2025 IDz. All rights reserved.
//

#pragma once
#include <cnt/PushBackVector.h>

using GraphType = std::uint32_t;

using Transition = struct
{
    GraphType toNodeID;   //destination node ID (1-based)
    GraphType overBranchID;    //destination branch ID (1-based)
};

using Edge = struct
{
    GraphType fromNodeID;   //terminal 1 (1-based)
    GraphType toNodeID;     //termainal 2 (1-based)
};

class IVisitor
{
public:
    enum class Action : td::BYTE {DoNotMove=0, Move, OnGoal, Stop};
    virtual Action isValidTransition(const Transition& trans) = 0;
    virtual Action onStartedTransition(const Transition& trans) = 0;
    virtual void onCompletedTransition(const Transition& trans, IVisitor::Action nextAction) = 0;
    virtual void sortTransitions(GraphType currentNodeID, cnt::PushBackVector<Transition>& transitions) = 0; //for DFS with Heuristics
};
