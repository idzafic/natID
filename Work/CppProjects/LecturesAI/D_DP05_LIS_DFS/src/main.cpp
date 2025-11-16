#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>

class LIS
{
public:
    enum class Solver {Recursive, DFSIterative, BottomUp};
private:
    struct Node {
        int value;    // element value
        int memo;     // LIS length starting here
        int destNode; // next node in LIS (-1 if none)
    };
    
private:
    std::vector<Node> _nodes;
    int _n;
    int _maxVal;

    // DP = Recursive DFS + memoization
    int dfsRecursive(int idx)
    {
        if (_nodes[idx].memo != -1) 
            return _nodes[idx].memo;

        int bestLen = 1;
        int nextNode = -1;

        for (int nextChild = idx + 1; nextChild < _nodes.size(); ++nextChild)
        {
            if (_nodes[nextChild].value > _nodes[idx].value)
            {
                int len = 1 + dfsRecursive(nextChild);
                if (len > bestLen) {
                    bestLen = len;
                    nextNode = nextChild;
                }
            }
        }

        _nodes[idx].memo = bestLen;
        _nodes[idx].destNode = nextNode;
        return bestLen;
    }
    
    int dfsIterative(int startIdx) {
        if (_nodes[startIdx].memo != -1)
            return _nodes[startIdx].memo;
        //this one simulates the stack frame
        struct Frame {
            int idx;
            int nextChild;
            int bestLen;
            int nextNode;
        };
        std::vector<Frame> stack;
        stack.push_back({startIdx, startIdx + 1, 1, -1});
        while (!stack.empty())
        {
            Frame &frame = stack.back();

            // If we finished exploring all children
            if (frame.nextChild >= _nodes.size())
            {
                // Store result in memo
                _nodes[frame.idx].memo = frame.bestLen;
                _nodes[frame.idx].destNode = frame.nextNode;

                stack.pop_back();

                // If this node was reached from another node, propagate the result
                if (!stack.empty()) {
                    Frame &parent = stack.back();
                    int len = 1 + _nodes[frame.idx].memo;
                    if (len > parent.bestLen)
                    {
                        parent.bestLen = len;
                        parent.nextNode = frame.idx;
                    }
                }

                continue;
            }

            int j = frame.nextChild++;
            if (_nodes[j].value > _nodes[frame.idx].value)
            {
                if (_nodes[j].memo != -1)
                {
                    int len = 1 + _nodes[j].memo;
                    if (len > frame.bestLen)
                    {
                        frame.bestLen = len;
                        frame.nextNode = j;
                    }
                }
                else
                    stack.push_back({j, j + 1, 1, -1});
            }
        }

        return _nodes[startIdx].memo;
    }
    
    void generateSequence()
    {
        std::vector<int> values;

        // Fill with numbers from 0..maxNum
        for (int i = 0; i <= _maxVal; ++i)
            values.push_back(i);

        std::random_device rd;
        
        std::mt19937 g(rd());
        
        // Shuffle values
        std::shuffle(values.begin(), values.end(), g);

        // Take the first n unique numbers
        values.resize(_n);
        
        _nodes.resize(_n);
        for (int i = 0; i < _n; ++i)
        {
            _nodes[i].value = values[i];
            _nodes[i].memo = -1;
            _nodes[i].destNode = -1;
        }
    }
    
    // Compute LIS and return the sequence
    std::vector<int> solveWithDFS(Solver solver)
    {
        int maxLen = 0;
        int startNode = -1;

        for (int i = 0; i < _nodes.size(); ++i)
        {
            int len = 0;
            if (solver == Solver::DFSIterative)
                len = dfsIterative(i);
            else
                len = dfsRecursive(i);
            if (len > maxLen)
            {
                maxLen = len;
                startNode = i;
            }
        }

        // Reconstruct LIS
        std::vector<int> lis;
        for (int idx = startNode; idx != -1; idx = _nodes[idx].destNode)
            lis.push_back(_nodes[idx].value);

        return lis;
    }
    
    // Compute LIS using bottom-up DP
    std::vector<int> solveBottomUp()
    {
        int n = int(_nodes.size());
        for (int i = 0; i < n; ++i)
        {
            _nodes[i].memo = 1;
            _nodes[i].destNode = -1;
        }

        int maxLen = 1;
        int startNode = 0;

        for (int i = n - 1; i >= 0; --i)
        {
            for (int j = i + 1; j < n; ++j)
            {
                if (_nodes[j].value > _nodes[i].value &&
                    _nodes[j].memo + 1 > _nodes[i].memo)
                {
                    _nodes[i].memo = _nodes[j].memo + 1;
                    _nodes[i].destNode = j;
                }
            }

            if (_nodes[i].memo > maxLen)
            {
                maxLen = _nodes[i].memo;
                startNode = i;
            }
        }

        std::vector<int> lis;
        for (int idx = startNode; idx != -1; idx = _nodes[idx].destNode)
            lis.push_back(_nodes[idx].value);

        return lis;
    }

public:
    LIS(int sequenceLength, int maxNumber)
        : _n(sequenceLength), _maxVal(maxNumber)
    {
        if (_maxVal < _n) {
            throw std::invalid_argument("maxNumber must be >= sequence length to ensure uniqueness");
        }
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        generateSequence();
    }
    

    // Optional: print generated sequence
    void printSequence() const {
        std::cout << "Generated sequence: ";
        for (auto& node : _nodes)
            std::cout << node.value << " ";
        std::cout << "\n";
    }
    
    std::vector<int> solve(Solver solver)
    {
        if (solver < Solver::BottomUp)
            return solveWithDFS(solver);
        return solveBottomUp();
    }
};

void printSolution(const std::vector<int>& lis, const char* lbl)
{
    std::cout << "Longest Increasing Subsequence using method " << lbl << "(" << lis.size() << "): ";
    for (int x : lis)
        std::cout << x << " ";
    std::cout << "\n";
}

// --------------------- Example usage ---------------------
int main() {
    try {
        int length, maxNumber;
        std::cout << "Enter sequence length: ";
        std::cin >> length;
        std::cout << "Enter max number: ";
        std::cin >> maxNumber;

        LIS lisSolver(length, maxNumber);

        lisSolver.printSequence();

        auto lis = lisSolver.solve(LIS::Solver::DFSIterative);
        printSolution(lis, "DFSIterative");
        
        auto lis2 = lisSolver.solve(LIS::Solver::Recursive);
        printSolution(lis2, "Recursive");
        
        auto lis3 = lisSolver.solve(LIS::Solver::BottomUp);
        printSolution(lis2, "BottomUp");
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}
