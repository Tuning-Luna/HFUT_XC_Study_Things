#include <iostream>
#include <vector>
#include <set>
using namespace std;
vector<int> helper(int x) {
    set<int> v;
    for (int a = 2; a * a <= 2 * x; ++a) {
        for (int b = 1; b < a; ++b) {
            int magic_num = a * a - b * b;
            if (magic_num > x) break;
            v.insert(magic_num);
        }
    }

    return vector<int>(v.begin(), v.end());
}

int main() {
    int T;
    cin >> T;
    vector<int> ans(T);
    int max_x = 0;

    for (int i = 0; i < T; ++i) {
        cin >> ans[i];
        if (ans[i] > max_x) {
            max_x = ans[i];
        }
    }
    vector<int> v2 = helper(1e9);

    for (int i = 0; i < T; ++i) {
        cout << v2[ans[i] - 1] << endl;
    }
    return 0;
}
