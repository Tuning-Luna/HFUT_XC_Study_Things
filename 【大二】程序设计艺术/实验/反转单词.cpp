#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

void func(vector<string>& v) {
    for (auto& str : v) {
        istringstream iss(str);
        string temp, word;
        while (iss >> word) {
            reverse(word.begin(), word.end());
            temp += word + " ";
        }
        if (!temp.empty())
            temp.pop_back();
        str = temp;
    }
}


int main() {
    int n = 0;
    cout << "输入个数：";
    cin >> n;
    cin.ignore(); //忽略换行符 避免getline()读入
    vector<string> v;
    string s;
    cout << "输入" << n << "个字符串：" << endl;
    for (int i = 0; i < n; ++i) {
        getline(cin, s);
        v.emplace_back(s);
    }

    func(v);

    for (const auto& str : v) {
        cout << str << endl;
    }
    return 0;
}