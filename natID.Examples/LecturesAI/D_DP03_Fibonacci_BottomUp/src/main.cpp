#include <iostream>
#include <vector>
#include <td/Types.h>

using namespace std;

using Table = vector<td::LUINT8>; 
td::LUINT8 fibBottomUp(unsigned int n) 
{
    if (n <= 1)
        return n;

    Table dp(n + 1);
    dp[0] = 0;
    dp[1] = 1;

    for (int i = 2; i <= n; ++i)
        dp[i] = dp[i - 1] + dp[i - 2];

    return dp[n];
}

int main() {
    unsigned int n = 10;
    cout << "Enter Fibonacci number to compute: ";
    cin >> n;
    cout << "Fibonacci(" << n << ") = " << fibBottomUp(n) << endl;
    return 0;
}
