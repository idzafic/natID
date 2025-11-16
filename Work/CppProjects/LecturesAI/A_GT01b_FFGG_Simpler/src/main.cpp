#include <iostream>
#include <queue>
#include <set>
#include <vector>
#include <algorithm>
#include <limits>

class FarmerFoxGoosGrainSolver
{
private:
    struct State
    {
        enum class Color : unsigned int {Unexplored=0, Explored};
        unsigned int _ffgg = 0;      // bitfield: F<<3 | X<<2 | G<<1 | S<<0
        unsigned int _fromffgg = std::numeric_limits<unsigned int>::max(); // parent ffgg
        Color _color = Color::Unexplored;

        State() = default;

        State(unsigned int farmer, unsigned int fox, unsigned int goose, unsigned int seed)
        {
            _ffgg = ((farmer & 1u) << 3) |
                    ((fox    & 1u) << 2) |
                    ((goose  & 1u) << 1) |
                    ((seed   & 1u) << 0);
        }

        explicit State(unsigned int ffgg) : _ffgg(ffgg) {}

        bool operator<(State const& other) const noexcept { return _ffgg < other._ffgg; }
        bool operator==(State const& other) const noexcept { return _ffgg == other._ffgg; }

        unsigned int farmer() const { return (_ffgg >> 3) & 1u; }
        unsigned int fox()    const { return (_ffgg >> 2) & 1u; }
        unsigned int goose()  const { return (_ffgg >> 1) & 1u; }
        unsigned int seed()   const { return (_ffgg >> 0) & 1u; }
    };

    bool _showIllegalMoves;
    bool _showBoatMoves;
    std::vector<State> _solutionStates;

private:
    bool isValid(const State &s) const
    {
        if (s.fox() == s.goose() && s.farmer() != s.fox()) return false;
        if (s.goose() == s.seed() && s.farmer() != s.goose()) return false;
        return true;
    }

    std::vector<State> generateMoves(const State &cur) const
    {
        std::vector<State> out;
        unsigned int f = cur.farmer();
        unsigned int x = cur.fox();
        unsigned int g = cur.goose();
        unsigned int s = cur.seed();

        // farmer alone
        {
            unsigned int nf = f ^ 1u;   //XOR
            State ns(nf, x, g, s);
            out.push_back(ns);
        }

        // farmer + fox
        if (x == f)
        {
            unsigned int nf = f ^ 1u;
            unsigned int nx = x ^ 1u;
            State ns(nf, nx, g, s);
            out.push_back(ns);
        }

        // farmer + goose
        if (g == f)
        {
            unsigned int nf = f ^ 1u;
            unsigned int ng = g ^ 1u;
            State ns(nf, x, ng, s);
            out.push_back(ns);
        }

        // farmer + seed
        if (s == f)
        {
            unsigned int nf = f ^ 1u;
            unsigned int nsd = s ^ 1u;
            State ns(nf, x, g, nsd);
            out.push_back(ns);
        }

        return out;
    }

    //graph will be generated based on possible combinations and valid actions
    bool bfs(State const &start, State const &goal)
    {
        std::queue<State> frontier;
        std::set<State> states;  // all valid states, with color markers (Explored, Unexplored)

        // Insert all valid states (16 possible combinations)
        for (unsigned int i = 0; i < 16; ++i)
        {
            State s(i);         //unexplored states
            if (isValid(s))
                states.insert(s);
        }
        
        // initialize
        auto findState = [&](unsigned int ffgg) -> State* {
            auto it = states.find(State(ffgg));
            return (it != states.end()) ? const_cast<State*>(&*it) : nullptr;
        };

        State* startPtr = findState(start._ffgg);
        if (!startPtr)
            return false;

        startPtr->_fromffgg = std::numeric_limits<unsigned int>::max();
        startPtr->_color = State::Color::Explored;
        frontier.push(*startPtr);

        bool found = false;
        unsigned int goal_ffgg = goal._ffgg;

        while (!frontier.empty())
        {
            State cur = frontier.front();
            frontier.pop();

            if (cur._ffgg == goal_ffgg)
            {
                found = true;
                break;
            }

            auto moves = generateMoves(cur);
            for (auto &cand : moves)
            {
                if (!isValid(cand))
                {
                    if (_showIllegalMoves)
                        std::cout << stateToString(cur) << " -> ILLEGAL move -> " << stateToString(cand) << "\n";
                    continue;
                }

                State* next = findState(cand._ffgg);
                if (!next || next->_color == State::Color::Explored)
                    continue;

                next->_fromffgg = cur._ffgg;
                next->_color = State::Color::Explored;
                frontier.push(*next);

                if (next->_ffgg == goal_ffgg)
                {
                    found = true;
                    break;
                }
            }
            if (found)
                break;
        }

        if (!found)
            return false;

        // reconstruct solution
        _solutionStates.clear();
        unsigned int cur_ffgg = goal_ffgg;

        while (true)
        {
            State* node = findState(cur_ffgg);
            if (!node) break;
            _solutionStates.push_back(*node);
            if (node->_fromffgg == std::numeric_limits<unsigned int>::max())
                break;
            cur_ffgg = node->_fromffgg;
        }

        std::reverse(_solutionStates.begin(), _solutionStates.end());
        return true;
    }

    std::string sideChar(unsigned int bit) const
    {
        return bit ? "R" : "L";
    }

    std::string stateToString(State const &st) const
    {
        std::string s = "[B:";
        s += sideChar(st.farmer()) + std::string(" ");
        s += "F:" + sideChar(st.farmer());
        s += " X:" + sideChar(st.fox());
        s += " G:" + sideChar(st.goose());
        s += " S:" + sideChar(st.seed());
        s += "]";
        return s;
    }

    std::string moveDir(State const &from, State const &to) const
    {
        return to.farmer() ? "MoveRight" : "MoveLeft";
    }

    std::string boatItems(State const &from, State const &to) const
    {
        std::string items = "F";
        if (from.fox() != to.fox()) items += ",X";
        if (from.goose() != to.goose()) items += ",G";
        if (from.seed() != to.seed()) items += ",S";
        return items;
    }

    void printLegend() const
    {
        std::cout << "Legend: F=Farmer, X=Fox, G=Goose, S=Seed\n";
        std::cout << "Format: [B:<bank> F:<bank> X:<bank> G:<bank> S:<bank>] -> MoveDir[items] -> [nextState]\n\n";
    }

    void printSolutionTransitions() const
    {
        if (_solutionStates.empty())
        {
            std::cout << "No solution to print.\n";
            return;
        }
        std::cout << "-----------------------------------------------------------\n";
        std::cout << "SOURCE STATE          ->  ACTION      ->  DESTINATION STATE\n";
        std::cout << "-----------------------------------------------------------\n";
        for (size_t i = 0; i + 1 < _solutionStates.size(); ++i)
        {
            State const &cur = _solutionStates[i];
            State const &nxt = _solutionStates[i+1];
            std::string dir = moveDir(cur, nxt);
            std::string items = boatItems(cur, nxt);
            std::cout << stateToString(cur)
                      << " -> " << dir << "[" << items << "] -> "
                      << stateToString(nxt) << "\n";
        }
        std::cout << "-----------------------------------------------------------\n";
    }

public:
    FarmerFoxGoosGrainSolver(bool showIllegalMoves = true, bool showBoatMoves = true)
        : _showIllegalMoves(showIllegalMoves), _showBoatMoves(showBoatMoves) {}

    void solve()
    {
        printLegend();

        State start(0,0,0,0);
        State goal(1,1,1,1);

        bool found = bfs(start, goal);
        if (!found)
        {
            std::cout << "No solution found.\n";
            return;
        }

        printSolutionTransitions();
    }
};

int main()
{
    FarmerFoxGoosGrainSolver solver(false, true);
    solver.solve();
    return 0;
}

