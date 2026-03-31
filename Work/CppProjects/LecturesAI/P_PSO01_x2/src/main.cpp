#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>

class PSOSolver
{
private:
    struct Particle {
        double position;
        double velocity;
        double pbestPosition;
        double pbestValue;
    };

    // --- configuration ---
    size_t _swarmSize;
    size_t _maxIterations;
    double _minX;
    double _maxX;
    double _minV;
    double _maxV;
    double _w;
    double _c1;
    double _c2;
    double _convergenceEpsilon;

    // --- state ---
    std::vector<Particle> _swarm;
    double _gbestPosition = 0.0;
    double _gbestValue = std::numeric_limits<double>::max();

    std::mt19937 _rng;

    // --- helpers ---

    double fitness(double x) const {
        return x * x; // minimize y = x^2
    }

    double randomDouble(double min, double max) {
        std::uniform_real_distribution<double> dist(min, max);
        return dist(_rng);
    }

    void initSwarm() {
        _swarm.clear();
        _swarm.reserve(_swarmSize);

        for (size_t i = 0; i < _swarmSize; ++i) {
            double pos = randomDouble(_minX, _maxX);
            double vel = randomDouble(_minV, _maxV);

            Particle p;
            p.position = pos;
            p.velocity = vel;
            p.pbestPosition = pos;
            p.pbestValue = fitness(pos);

            _swarm.push_back(p);

            if (p.pbestValue < _gbestValue) {
                _gbestValue = p.pbestValue;
                _gbestPosition = p.position;
            }
        }
    }

    void updateBests() {
        for (auto& p : _swarm) {
            double value = fitness(p.position);

            if (value < p.pbestValue) {
                p.pbestValue = value;
                p.pbestPosition = p.position;
            }

            if (value < _gbestValue) {
                _gbestValue = value;
                _gbestPosition = p.position;
            }
        }
    }

    void updateParticles() {
        for (auto& p : _swarm) {
            double r1 = randomDouble(0.0, 1.0);
            double r2 = randomDouble(0.0, 1.0);

            // velocity update
            p.velocity =
                _w * p.velocity +
                _c1 * r1 * (p.pbestPosition - p.position) +
                _c2 * r2 * (_gbestPosition - p.position);

            p.velocity = std::clamp(p.velocity, _minV, _maxV);

            // position update
            p.position += p.velocity;
            p.position = std::clamp(p.position, _minX, _maxX);
        }
    }

public:
    PSOSolver(size_t swarmSize,
              size_t maxIterations,
              double minX,
              double maxX,
              double minV,
              double maxV,
              double inertiaWeight,
              double cognitiveWeight,
              double socialWeight,
              double convergenceEpsilon)
        : _swarmSize(swarmSize),
          _maxIterations(maxIterations),
          _minX(minX),
          _maxX(maxX),
          _minV(minV),
          _maxV(maxV),
          _w(inertiaWeight),
          _c1(cognitiveWeight),
          _c2(socialWeight),
          _convergenceEpsilon(convergenceEpsilon),
          _rng(std::random_device{}())
    {
        initSwarm();
    }

    bool solve() {
        for (size_t iter = 0; iter < _maxIterations; ++iter) {
            updateBests();

            if (_gbestValue < _convergenceEpsilon) {
                std::cout << "Converged at iteration " << iter << "\n";
                return true;
            }

            updateParticles();

            std::cout << "Iteration " << iter
                      << "  Best x = " << _gbestPosition
                      << "  Fitness = " << _gbestValue << "\n";
        }
        return false;
    }

    double bestX() const {
        return _gbestPosition;
    }

    double bestFitness() const {
        return _gbestValue;
    }
};


int main()
{
    PSOSolver pso(
        30,     // swarm size
        1000,   // max iterations
        -10.0,  // min x
        10.0,   // max x
        -1.0,   // min velocity
        1.0,    // max velocity
        0.7,    // inertia weight
        1.5,    // cognitive weight
        1.5,    // social weight
        1e-8    // convergence epsilon
    );

    if (pso.solve()) {
        std::cout << "\nFinal solution: x = "
                  << pso.bestX()
                  << ", f(x) = "
                  << pso.bestFitness()
                  << "\n";
    }
    else
        std::cout << "\nDid not converge\n";
}
