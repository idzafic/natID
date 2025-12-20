#include <iostream>
#include <cmath>
#include <random>
#include <functional>

class SASolver 
{
private:
    std::function<double(double)> _objectiveFunction;
    double _initialState;
    int _maxIterations;
    double _initialTemperature;
    double _coolingRate;
    double _neighborhoodSize;
    double _current;
    double _bestSolution;
    double _bestValue;
    std::mt19937 _rng;
    std::uniform_real_distribution<double> _uniformDist;

    double _temperature(int t) 
    {
        return _initialTemperature * std::pow(_coolingRate, t);
    }

    double _getRandomNeighbor(double state) 
    {
        double offset = _uniformDist(_rng) * 2.0 * _neighborhoodSize - _neighborhoodSize;
        return state + offset;
    }

    double _acceptanceProbability(double deltaE, double temperature) 
    {
        if (temperature == 0)
            return 0;
        
        return std::exp(deltaE / temperature);
    }

public:
    SASolver(std::function<double(double)> objectiveFunction,
             double initialState,
             int maxIterations = 1000,
             double initialTemperature = 100.0,
             double coolingRate = 0.95,
             double neighborhoodSize = 1.0)
        : _objectiveFunction(objectiveFunction),
          _initialState(initialState),
          _maxIterations(maxIterations),
          _initialTemperature(initialTemperature),
          _coolingRate(coolingRate),
          _neighborhoodSize(neighborhoodSize),
          _current(0),
          _bestSolution(0),
          _bestValue(0),
          _rng(std::random_device{}()),
          _uniformDist(0.0, 1.0) {}

    std::pair<double, double> solve() 
    {
        // Initialize current state
        _current = _initialState;
        double currentValue = _objectiveFunction(_current);

        // Track best solution found
        _bestSolution = _current;
        _bestValue = currentValue;

        // Main simulated annealing loop
        for (int t = 1; t <= _maxIterations; ++t) {
            // Calculate current temperature
            double T = _temperature(t);

            // Generate random neighbor
            double neighbor = _getRandomNeighbor(_current);
            double neighborValue = _objectiveFunction(neighbor);

            // Calculate how much better neighbor is than current
            // For minimization: positive ΔE means neighbor is better (lower value)
            double deltaE = currentValue - neighborValue;

            // Decide whether to accept the neighbor
            if (deltaE > 0) {
                // Neighbor is better, always accept
                _current = neighbor;
                currentValue = neighborValue;
            } else {
                // Neighbor is worse, accept with probability e^(ΔE/T)
                double probability = _acceptanceProbability(deltaE, T);
                if (_uniformDist(_rng) < probability) {
                    _current = neighbor;
                    currentValue = neighborValue;
                }
            }

            // Update best solution if current is better
            if (currentValue < _bestValue) {
                _bestSolution = _current;
                _bestValue = currentValue;
            }
        }

        return std::make_pair(_bestSolution, _bestValue);
    }
};

// Example usage
int main() 
{
    // Define the objective function: y = x^2
    auto objective = [](double x) { return x * x; };

    // Create solver instance
    SASolver solver(
        objective,
        10.0,      // initialState: Start from x=10
        1000,      // maxIterations
        100.0,     // initialTemperature
        0.95,      // coolingRate
        1.0        // neighborhoodSize
    );

    // Solve
    auto [bestSolution, bestValue] = solver.solve();

    std::cout << "Best solution found: x = " << bestSolution << std::endl;
    std::cout << "Minimum value: y = " << bestValue << std::endl;
    std::cout << "Expected minimum: x = 0, y = 0" << std::endl;

    return 0;
}