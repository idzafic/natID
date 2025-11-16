#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <array>
#include <optional>
#include <string>
#include <algorithm>

// ===== State Definition =====
struct State {
    int F, X, G, S; // Farmer, Fox, Goose, Seed

    bool operator==(const State& other) const {
        return F == other.F && X == other.X && G == other.G && S == other.S;
    }
};

// Hash for unordered_map
struct StateHash {
    std::size_t operator()(const State& s) const noexcept {
        return (s.F << 3) | (s.X << 2) | (s.G << 1) | s.S;
    }
};

// Transition info
struct Transition {
    State state;
    std::string boatInfo;
    bool valid;
};

// ===== Solver Class =====
class FarmerFoxGoosGrainSolver {
public:
    FarmerFoxGoosGrainSolver(bool showIllegalMoves = true, bool showBoatMoves = true)
        : _showIllegalMoves(showIllegalMoves), _showBoatMoves(showBoatMoves) {}

    void solve() {
        printLegend();
        State start{0,0,0,0};
        State goal{1,1,1,1};

        auto path = bfs(start, goal);

        if (!path.empty()) {
            std::cout << "Solution found (BFS shortest path):\n";
            int step = 0;
            for (size_t i = 0; i < path.size(); i++) {
                auto [s, move] = path[i];
                std::cout << "Step " << step++ << ": " << stateLabel(s);
                if (_showBoatMoves && i > 0) {
                    std::cout << "   [Boat: " << move << "]";
                }
                std::cout << "\n";
            }
        } else {
            std::cout << "No solution found.\n";
        }
    }

private:
    bool _showIllegalMoves;
    bool _showBoatMoves;

    // Print legend for clarity
    void printLegend() {
        std::cout << "Legend:\n";
        std::cout << "F = Farmer, X = Fox, G = Goose, S = Seed\n";
        std::cout << "L = Left bank, R = Right bank\n\n";
    }

    // Convert state to readable string
    std::string stateLabel(const State& s) {
        auto bank = [](int v) { return v == 0 ? "L" : "R"; };
        return "F=" + std::string(bank(s.F)) +
               " X=" + bank(s.X) +
               " G=" + bank(s.G) +
               " S=" + bank(s.S);
    }

    // Check validity (no eating rules)
    bool isValid(const State& s) {
        if (s.F != s.X && s.X == s.G) return false; // fox eats goose
        if (s.F != s.G && s.G == s.S) return false; // goose eats seed
        return true;
    }

    // Generate transitions
    std::vector<Transition> getTransitions(const State& s) {
        std::vector<Transition> result;

        // Farmer alone
        {
            State n{1 - s.F, s.X, s.G, s.S};
            bool ok = isValid(n);
            result.push_back({n, "Farmer alone", ok});
        }

        // Farmer + fox
        if (s.F == s.X) {
            State n{1 - s.F, 1 - s.X, s.G, s.S};
            bool ok = isValid(n);
            result.push_back({n, "Farmer with Fox", ok});
        }

        // Farmer + goose
        if (s.F == s.G) {
            State n{1 - s.F, s.X, 1 - s.G, s.S};
            bool ok = isValid(n);
            result.push_back({n, "Farmer with Goose", ok});
        }

        // Farmer + seed
        if (s.F == s.S) {
            State n{1 - s.F, s.X, s.G, 1 - s.S};
            bool ok = isValid(n);
            result.push_back({n, "Farmer with Seed", ok});
        }

        return result;
    }

    // BFS solver
    std::vector<std::pair<State,std::string>> bfs(const State& start, const State& goal) {
        std::queue<State> q;
        std::unordered_map<State, std::pair<std::optional<State>, std::string>, StateHash> parent;

        q.push(start);
        parent[start] = {std::nullopt, ""};

        while (!q.empty()) {
            State cur = q.front();
            q.pop();

            if (cur == goal) {
                // reconstruct path
                std::vector<std::pair<State,std::string>> path;
                std::optional<State> x = cur;
                while (x.has_value()) {
                    auto [par, moveInfo] = parent[*x];
                    path.push_back({*x, moveInfo});
                    x = par;
                }
                std::reverse(path.begin(), path.end());
                return path;
            }

            for (auto& tr : getTransitions(cur)) {
                if (!tr.valid && _showIllegalMoves) {
                    std::cout << "ILLEGAL: " << stateLabel(cur) << " -> "
                              << stateLabel(tr.state) << " (" << tr.boatInfo << ")\n";
                }
                if (tr.valid && !parent.count(tr.state)) {
                    parent[tr.state] = {cur, tr.boatInfo};
                    q.push(tr.state);
                }
            }
        }
        return {};
    }
};

// ===== Main =====
int main()
{
    FarmerFoxGoosGrainSolver solver(true, true); // both options enabled
    solver.solve();
    return 0;
}
