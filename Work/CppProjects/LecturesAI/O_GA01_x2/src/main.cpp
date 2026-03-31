#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include <limits>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>

class GASolver
{
private:
    // --- configuration ---
    size_t _populationSize;
    double _minX;
    double _maxX;
    double _mutationRate;
    double _crossoverRate;
    double _convergenceEpsilon;
    size_t _maxGenerations;

    // --- state ---
    std::vector<double> _population;
    double _bestX = 0.0;

    std::mt19937 _rng;

    // --- GA internals ---

    void initPopulation()
    {
        std::uniform_real_distribution<double> dist(_minX, _maxX);
        _population.resize(_populationSize);
        for (double& x : _population)
        {
            x = dist(_rng);
        }
    }

    double fitness(double x) const
    {
        return x * x; // minimize y = x^2
    }

    double selectParent()
    {
        std::uniform_int_distribution<size_t> dist(0, _populationSize - 1);
        double a = _population[dist(_rng)];
        double b = _population[dist(_rng)];
        return (fitness(a) < fitness(b)) ? a : b;
    }

    double crossover(double parent1, double parent2)
    {
        // random crossover: pick one parent randomly
        std::uniform_real_distribution<double> chance(0.0, 1.0);
        if (chance(_rng) < _crossoverRate) {
            return (chance(_rng) < 0.5) ? parent1 : parent2;
        }
        return parent1;
    }

    void mutate(double& x)
    {
        std::uniform_real_distribution<double> chance(0.0, 1.0);
        std::uniform_real_distribution<double> delta(-1.0, 1.0);

        if (chance(_rng) < _mutationRate)
        {
            x += delta(_rng);
            x = std::clamp(x, _minX, _maxX);
        }
    }

    void nextGeneration()
    {
        std::vector<double> newPopulation;
        newPopulation.reserve(_populationSize);

        while (newPopulation.size() < _populationSize)
        {
            double parent1 = selectParent();
            double parent2 = selectParent();

            double child = crossover(parent1, parent2);
            mutate(child);

            newPopulation.push_back(child);
        }

        _population = std::move(newPopulation);
    }
public:
    GASolver(size_t populationSize,
             double minX,
             double maxX,
             double mutationRate,
             double crossoverRate,
             double convergenceEpsilon,
             size_t maxGenerations)
        : _populationSize(populationSize),
          _minX(minX),
          _maxX(maxX),
          _mutationRate(mutationRate),
          _crossoverRate(crossoverRate),
          _convergenceEpsilon(convergenceEpsilon),
          _maxGenerations(maxGenerations),
          _rng(std::random_device{}())
    {
        initPopulation();
    }

    bool solve()
    {
        for (size_t generation = 0; generation < _maxGenerations; ++generation)
        {
            auto bestIt = std::min_element(
                _population.begin(), _population.end(),
                [&](double a, double b) {
                    return fitness(a) < fitness(b);
                }
            );

            _bestX = *bestIt;
            double bestFitness = fitness(_bestX);

            if (bestFitness < _convergenceEpsilon)
            {
                std::cout << "Converged at generation " << generation << "\n";
                return true;
            }

            nextGeneration();
        }
        return false;
    }

    double bestX() const {
        return _bestX;
    }
};


int main()
{
    GASolver ga(
        100,     // population size
        -10.0,   // min x
        10.0,    // max x
        0.1,     // mutation rate
        0.7,     // crossover rate
        1e-8,    // convergence epsilon
        1000     // max generations
    );

    if (ga.solve()) {
        std::cout << "Best x ≈ " << ga.bestX()
                  << ", y = " << ga.bestX() * ga.bestX()
                  << std::endl;
    }
}

