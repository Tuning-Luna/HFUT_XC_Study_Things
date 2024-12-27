#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
//输出编辑距离最少的步骤
void show_info(const vector<vector<int>>& dp, const string& a, const string& b) {
    int i = a.size();
    int j = b.size();

    while (i > 0 || j > 0) {
        if (i > 0 && j > 0 && a[i - 1] == b[j - 1]) {
            // 字符相同，不需要操作
            i--;
            j--;
        }
        else if (i > 0 && dp[i][j] == dp[i - 1][j] + 1) {
            // 从 dp[i-1][j] 转移过来，说明是删除操作
            cout << "在下标" << i << "处插入" << a[i - 1] << endl;
            i--;
        }
        else if (j > 0 && dp[i][j] == dp[i][j - 1] + 1) {
            // 从 dp[i][j-1] 转移过来，说明是插入操作
            cout << "在下标" << i + 1<<"处插入"<< b[j - 1] << endl;
            j--;
        }
        else if (i > 0 && j > 0 && dp[i][j] == dp[i - 1][j - 1] + 1) {
            // 从 dp[i-1][j-1] 转移过来，说明是替换操作
            cout << "在下标" << i << "处把" << a[i - 1] << "替换为" << b[j - 1] << endl;
            i--;
            j--;
        }
    }
}

// 求最少编辑距离并输出步骤
int solve(const string& a, const string& b) {
    int m = a.size();
    int n = b.size();

    // 创建 dp 表格
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    // 初始化边界条件
    for (int i = 0; i <= m; ++i) {
        dp[i][0] = i;  // 把 a 转成空串需要 i 次删除操作
    }
    for (int j = 0; j <= n; ++j) {
        dp[0][j] = j;  // 把空串转成 b 需要 j 次插入操作
    }

    // 填充 dp 表格
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (a[i - 1] == b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];  // 如果字符相同，不需要额外操作
            }
            else {
                dp[i][j] = min({ dp[i - 1][j - 1] + 1,  // 替换操作
                                dp[i - 1][j] + 1,      // 删除操作
                                dp[i][j - 1] + 1 });    // 插入操作
            }
        }
    }

    // 输出编辑步骤
    show_info(dp, a, b);

    // 返回最小编辑距离
    return dp[m][n];
}

int main() {
    string a, b;
    cout << "输入第一个字符串: ";
    cin >> a;
    cout << "输入第二个字符串: ";
    cin >> b;

    int result = solve(a, b);
    cout << "最少操作次数: " << result << endl;

    return 0;
}