#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <limits>
#include <cmath>

//minimize: y = x^2
//demonstrate tournament selection

class GAMinimizer
{
private:
    struct Individual 
    {
        double x;
        double fitness;
    };

    int _populationSize;
    double _crossoverRate;
    double _mutationRate;
    double _xMin, _xMax;
    std::vector<Individual> _population;
    Individual _bestIndividual;

    std::mt19937 _rng; //random generator

    // --- GA parts ---
    void createInitialPopulation() 
    {
        std::uniform_real_distribution<double> dist(_xMin, _xMax);
        _population.clear();
        for (int i = 0; i < _populationSize; ++i) 
        {
            Individual ind;
            ind.x = dist(_rng);
            ind.fitness = evaluate(ind.x);
            _population.push_back(ind);
        }
    }

    double evaluate(double x) const {
        return x * x; // y = x^2
    }

    void evaluateFitness() 
    {
        for (auto &ind : _population)
            ind.fitness = evaluate(ind.x);
    }

    Individual tournamentSelection(int tournamentSize = 3) 
    {
        std::uniform_int_distribution<int> dist(0, _populationSize - 1);
        Individual best = _population[dist(_rng)];
        for (int i = 1; i < tournamentSize; ++i) {
            Individual challenger = _population[dist(_rng)];
            if (challenger.fitness < best.fitness) best = challenger;
        }
        return best;
    }

    std::pair<Individual, Individual> crossover(const Individual& parent1, const Individual& parent2) 
    {
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        if (dist(_rng) < _crossoverRate) {
            // Simple arithmetic crossover
            double alpha = dist(_rng);
            Individual child1, child2;
            child1.x = alpha * parent1.x + (1 - alpha) * parent2.x;
            child2.x = alpha * parent2.x + (1 - alpha) * parent1.x;
            return {child1, child2};
        } else {
            return {parent1, parent2}; // no crossover
        }
    }

    void mutate(Individual& ind) 
    {
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        std::uniform_real_distribution<double> mutDist(-0.1*(_xMax-_xMin), 0.1*(_xMax-_xMin));
        if (dist(_rng) < _mutationRate) {
            ind.x += mutDist(_rng);
            if (ind.x < _xMin) ind.x = _xMin;
            if (ind.x > _xMax) ind.x = _xMax;
        }
    }

    void createNewPopulation() 
    {
        std::vector<Individual> newPopulation;

        // Elitism: keep best individual
        newPopulation.push_back(_bestIndividual);

        while ((int)newPopulation.size() < _populationSize) 
        {
            Individual parent1 = tournamentSelection();
            Individual parent2 = tournamentSelection();
            auto [child1, child2] = crossover(parent1, parent2);
            mutate(child1);
            mutate(child2);
            child1.fitness = evaluate(child1.x);
            child2.fitness = evaluate(child2.x);
            newPopulation.push_back(child1);
            if ((int)newPopulation.size() < _populationSize) newPopulation.push_back(child2);
        }

        _population.swap(newPopulation);
    }
public:
    GAMinimizer(int populationSize = 20, double crossoverRate = 0.7, double mutationRate = 0.1)
        : _populationSize(populationSize), _crossoverRate(crossoverRate), _mutationRate(mutationRate), _rng(std::random_device{}()) {}

    // Solve for minimum in [xMin, xMax] with precision eps and max number of generations
    std::tuple<double, double, int> solve(double xMin, double xMax, double eps, int maxNumberOfGenerations) 
    {
        this->_xMin = xMin;
        this->_xMax = xMax;

        createInitialPopulation();
        int generation = 0;
        while (generation < maxNumberOfGenerations)
        {
            evaluateFitness();

            // Find best solution in current population
            auto bestIt = std::min_element(_population.begin(), _population.end(),
                                           [](const Individual &a, const Individual &b) { return a.fitness < b.fitness; });
            _bestIndividual = *bestIt;

            //check termination
            if (_bestIndividual.fitness <= eps)
                break; // Termination criteria

            createNewPopulation();
            ++generation;
        }

        return {_bestIndividual.x, _bestIndividual.fitness, generation};
    }

    double getMinimumX() const { return _bestIndividual.x; }
    double getMinimumY() const { return _bestIndividual.fitness; }
};


int main()
{
    GAMinimizer minimizer;
    
    double xMin = -10.0;
    double xMax = 5.0;
    double eps = 1e-6;
    int maxGenerations = 50;
    
    auto [solution, fitness, nIter] = minimizer.solve(xMin, xMax, eps, maxGenerations);

    std::cout << "GA completed with " << nIter << " iterations. Best solution: " << solution << ", best fitness: " << fitness << std::endl;
    std::cout << "Minimum x: " << minimizer.getMinimumX() << std::endl;
    std::cout << "Minimum y: " << minimizer.getMinimumY() << std::endl;

    return 0;
}
