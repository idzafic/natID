//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <td/DateTime.h>
#include <cmath>

#ifndef USE_ODE_MODEL
#define USE_ODE_MODEL
#endif

#ifndef USE_MEMORY_MODELS
#define USE_MEMORY_MODELS
#endif

#define SOLVERTYPE double
#define X0 0.5
#define InitialK 0.2

class SimulationParams
{
public:
    td::DateTime _simulationStartTime;
    double l = 1;
    double g = 9.81;
    double m = 1;
    double k = InitialK;
    double x0 = X0;
    double y0 = std::sqrt(l*l-X0*X0);
    bool animation = false;
    bool useCustomColors = false;
    td::ColorID fillColor = td::ColorID::DarkBlue;
    td::ColorID fontColor = td::ColorID::Yellow;
    SimulationParams()
    {}
};
