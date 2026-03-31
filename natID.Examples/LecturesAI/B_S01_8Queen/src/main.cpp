#include <iostream>
#include <vector>
#include <stack>
#include <cmath>
#include <random>
#include <chrono>

using namespace std;

const int N = 8;

struct State
{
    vector<int> queens; // queens[row] = column
    int row;
};

// Check if placing a queen at (row, col) is safe
bool isSafe(const vector<int>& queens, int row, int col)
{
    for (int i = 0; i < row; ++i)
    {
        int qCol = queens[i];
        if (qCol == col || abs(qCol - col) == abs(i - row))
            return false;
    }
    return true;
}

// Print the board
void printBoard(const vector<int>& queens) {
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c)
            cout << (queens[r] == c ? " Q " : " . ");
        cout << endl;
    }
    cout << "----------------------" << endl;
}

int randomInt(int N)
{
    // Static so that we don't reseed every call
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, N-1);
    return dis(gen);
}

std::vector<int> getRandomColons(int N)
{
    // Create vector with numbers 0..N-1
    std::vector<int> vec(N);
    for (int i = 0; i < N; ++i) {
        vec[i] = i;
    }

    // Seed with a real random value, different each run
    static std::random_device rd;
    static std::mt19937 gen(rd());

    // Shuffle vector randomly
    std::shuffle(vec.begin(), vec.end(), gen);

    return vec;
}

int main()
{
    unsigned seed = unsigned(std::chrono::system_clock::now().time_since_epoch().count());
    std::mt19937 gen(seed);  // Mersenne Twister seeded differently each run
    stack<State> st;
    st.push({{}, 0}); // start: empty board, row 0

    while (!st.empty())
    {
        State s = st.top();
        st.pop();

        if (s.row == N)
        {
            cout << "✅ Found solution:\n";
            printBoard(s.queens);
            return 0; // stop after first solution
        }

        // Try placing queens in columns (DFS: push later options first)
        bool bRandom = true;
        if (bRandom)
        {
            auto cols = getRandomColons(N);
            for (int col : cols)
            {
                if (isSafe(s.queens, s.row, col))
                {
                    State next = s;
                    next.queens.push_back(col);
                    next.row++;
                    st.push(next);
                }
            }
        }
        else
        {
            for (int col = N - 1; col >= 0; --col)
            {
                if (isSafe(s.queens, s.row, col))
                {
                    State next = s;
                    next.queens.push_back(col);
                    next.row++;
                    st.push(next);
                }
            }
        }
        
    }

    cout << "❌ No solution found!!\n";
    return 0;
}
