#include <iostream>
#include <vector>
#include <td/Types.h>
using namespace std;

using Memo = vector<td::LUINT8>;

td::LUINT8 fibMemo(unsigned int n, Memo& memo)
{
    if (n <= 1) //base case
        return n;
    
    if (memo[n] != 0)   //check if already memo-azed
        return memo[n];
    //put result in memo
    memo[n] = fibMemo(n - 1, memo) + fibMemo(n - 2, memo);
    return memo[n];
}

int main() 
{
    unsigned int n = 10;
    cout << "Enter Fibonacci number to compute: ";
    cin >> n;
    Memo memo(n + 1, 0);
    cout << "Fibonacci(" << n << ") = " << fibMemo(n, memo) << endl;
    return 0;
}
