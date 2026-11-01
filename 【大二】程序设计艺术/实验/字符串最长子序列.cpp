#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
// 求两个字符串的最长公共子序列
string solve(const string& X, const string& Y) {
    int m = X.size();
    int n = Y.size();

    // 创建 dp 表格，大小为 (m+1) * (n+1)
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    // 填充 dp 表
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (X[i - 1] == Y[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            }
            else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    // 通过 dp 表倒推最长公共子序列
    string ret;
    int i = m, j = n;
    while (i > 0 && j > 0) {
        if (X[i - 1] == Y[j - 1]) {
            ret = (X[i - 1]) + ret; // 反向插入
            --i;
            --j;
        }
        else if (dp[i - 1][j] > dp[i][j - 1]) {
            --i;
        }
        else {
            --j;
        }
    }
    return ret;
}

int main() {
    string X, Y;
    cout << "输入第一个字符串: ";
    cin >> X;
    cout << "输入第二个字符串: ";
    cin >> Y;

    string result = solve(X, Y);
    if (!result.empty()) {
        cout << "最长公共子序列为: " << result << endl;
    }
    else {
        cout << "不存在最长公共子序列" << endl;
    }
    return 0;
}