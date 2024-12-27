#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n, k;
    cin >> n >> k;
    vector<int> peoples(n - 1);
    for (int i = 0; i < n - 1; ++i) {
        cin >> peoples[i];
    }
    int cur = 0;
    for (int i = 0; i < k; ++i) {
        cur += peoples[i];
    }
    int ans = cur;
    for (int i = k; i < n - 1; ++i) {
        cur = cur + peoples[i] - peoples[i - k];
        if (cur > ans) {
            ans = cur;
        }
    }
    cout << ans << endl;

    return 0;
}
