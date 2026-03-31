#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>
#include <algorithm>

// Function to generate a random test case with unique numbers
std::vector<int> generateCase(int n, int maxNum) 
{
    std::vector<int> values;

    // Fill with numbers from 0..maxNum
    for (int i = 0; i <= maxNum; ++i)
        values.push_back(i);

    std::random_device rd;
    
    std::mt19937 g(rd());
    
    // Shuffle values
    std::shuffle(values.begin(), values.end(), g);

    // Take the first n unique numbers
    values.resize(n);

    return values;
}

int main() 
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    int n, maxNum;
    std::cout << "Enter array length: ";
    std::cin >> n;
    std::cout << "Enter maximum number in sequence: ";
    std::cin >> maxNum;

    if (maxNum + 1 < n) {
        std::cerr << "Error: maxNum must be at least n - 1 to ensure uniqueness.\n";
        return 1;
    }

    // Generate unique sequence
    std::vector<int> A = generateCase(n, maxNum);

    std::cout << "\nGenerated unique sequence:\n";
    for (int x : A)
        std::cout << x << " ";
    std::cout << "\n\n";

    // DP array: L[i] = length of LIS ending at i
    std::vector<int> L(n, 1);
    std::vector<int> prev(n, -1); // to reconstruct sequence

    int maxLen = 1;
    int lastIndex = 0;

    for (int i = 1; i < n; ++i) 
    {
        for (int j = 0; j < i; ++j) 
        {
            if (A[j] < A[i] && L[j] + 1 > L[i]) 
            {
                L[i] = L[j] + 1;
                prev[i] = j;
            }
        }
        if (L[i] > maxLen) 
        {
            maxLen = L[i];
            lastIndex = i;
        }
    }

    // Reconstruct LIS
    std::vector<int> lis;
    for (int i = lastIndex; i != -1; i = prev[i])
        lis.push_back(A[i]);
    std::reverse(lis.begin(), lis.end());

    std::cout << "Length of Longest Increasing Subsequence: " << maxLen << "\n";
    std::cout << "LIS: ";
    for (int x : lis)
        std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
