#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
void get_next(vector<int>& next, const string& t) {
    int j = -1;
    next[0] = j;
    for (int i = 1; i < t.size(); ++i) {
        while (j >= 0 && next[i] != next[j + 1]) {
            j = next[j];
        }
        if (next[i] == next[j + 1]) {
            ++j;
        }
        next[i] = j;
    }
}

int KMP(const string& s, const string& t) {
    vector<int> next(t.size(), 0);
    get_next(next, t);
    int j = -1;
    for (int i = 0; i < s.size(); ++i) {
        while (j >= 0 && s[i] != t[j + 1]) {
            j = next[j];
        }
        if (s[i] == t[j + 1]) {
            ++j;
        }
        if (j == t.size() - 1) {
            return i - j;
        }
    }
    return -1;
}

int main() {
    string s, t;
    cout << "输入第一个字符串: ";
    cin >> s;
    cout << "输入第二个字符串: ";
    cin >> t;
    int index = KMP(s, t);
    if (index == -1) {
        cout << "不存在" << endl;
    }
    else {
        cout << "下标为：" << index << endl;
    }
    return 0;
}