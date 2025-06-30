#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

// 按照空格分割字符串
vector<string> splitSpace(const string& s) {
  vector<string> res;
  stringstream ss(s);
  string token;
  while (ss >> token) {
    res.push_back(token);
  }
  return res;
}

// 终结符集合
set<string> t = {};
// 非终结符集合
set<string> not_t = {};
// 产生式
map<string, vector<vector<string>>>productions = {};
// 函数：计算 FIRST 集
map<string, set<string>> getFIRST() {
  // 定义结果映射，键为非终结符，值为其 FIRST 集
  map<string, set<string>> ans;

  // 初始化：为每个非终结符创建一个空的 FIRST 集
  for (const auto& nt : not_t) {
    ans[nt] = set<string>();
  }

  // 使用固定点迭代法，直到 FIRST 集不再变化
  bool changed = true;
  while (changed) {
    changed = false;

    // 遍历每个非终结符
    for (const auto& nt : not_t) {
      // 遍历该非终结符的所有产生式
      for (const auto& rhs : productions.at(nt)) {
        // 临时存储当前产生式右部符号的 FIRST 集贡献
        set<string> first_alpha;
        // 标志是否所有符号都能推导出空串
        bool can_derive_epsilon = true;

        // 处理空产生式 (如 A -> ε)
        if (rhs.empty()) {
          // 如果 EPSILON 不在 FIRST(nt) 中，加入并标记变化
          if (ans[nt].find("EPSILON") == ans[nt].end()) {
            ans[nt].insert("EPSILON");
            changed = true;
          }
          continue;
        }

        // 遍历产生式右部的每个符号
        for (size_t i = 0; i < rhs.size(); ++i) {
          const string& symbol = rhs[i];

          // 如果符号是终结符或 EPSILON
          if (t.find(symbol) != t.end() || symbol == "EPSILON") {
            // 将终结符加入 FIRST(nt)，如果新加入则标记变化
            if (ans[nt].find(symbol) == ans[nt].end()) {
              ans[nt].insert(symbol);
              changed = true;
            }
            // 终结符不能推导出空串，停止处理后续符号
            can_derive_epsilon = false;
            break;
          }
          // 如果符号是非终结符
          else {
            // 将 FIRST(symbol) 中除 EPSILON 外的符号加入 FIRST(nt)
            for (const auto& t : ans[symbol]) {
              if (t != "EPSILON") {
                if (ans[nt].find(t) == ans[nt].end()) {
                  ans[nt].insert(t);
                  changed = true;
                }
              }
            }
            // 如果 FIRST(symbol) 不含 EPSILON，停止处理后续符号
            if (ans[symbol].find("EPSILON") == ans[symbol].end()) {
              can_derive_epsilon = false;
              break;
            }
          }
        }

        // 如果产生式右部所有符号都能推导出空串，且非空产生式
        if (can_derive_epsilon && !rhs.empty()) {
          // 将 EPSILON 加入 FIRST(nt)，如果新加入则标记变化
          if (ans[nt].find("EPSILON") == ans[nt].end()) {
            ans[nt].insert("EPSILON");
            changed = true;
          }
        }

      }
    }
  }

  // 返回计算好的 FIRST 集
  return ans;
}

int main() {
  /*
第一行：空格分隔的终结符列表（如“a b”）。
第二行：空格分隔的非终结符列表（如“S A B”）。
第三行：起始符号（一个非终结符，如“S”）。
后续行：产生式，格式为“A -> X1 X2 ... Xk”，其中A为非终结符，X1 X2 ... Xk为终结符或非终结符。空串用“EPSILON”表示。
  */

  /*
Example1:
Input :
a b
S A B
S
S -> A B
A -> a A
A -> EPSILON
B -> b

Output :
FIRST(A) = { 空串, a }
FIRST(B) = { b }
FIRST(S) = { a, b }
  */

  /*
Example2:
Input:
+ * i ( )
E E1 T T1 F
E
E -> T E1
E1 -> + T E1
E1 -> EPSILON
T -> F T1
T1 -> * F T1
T1 -> EPSILON
F -> ( E )
F -> i

Output:
FIRST(E) = { (, i }
FIRST(E1) = { +, ε }
FIRST(F) = { (, i }
FIRST(T) = { (, i }
FIRST(T1) = { *, ε }
  */

  string line = "";

  // 读取VT
  getline(cin, line);
  auto tList = splitSpace(line);
  t.insert(tList.begin(), tList.end());

  // 读取VN
  getline(cin, line);
  auto ntList = splitSpace(line);
  not_t.insert(ntList.begin(), ntList.end());

  // 读取起始符号
  getline(cin, line);
  string start_symbol = line;

  // 读取产生式
  for (const auto& nt : not_t) {
    productions[nt] = vector<vector<string>>();
  }
  while (getline(cin, line) && !line.empty()) {
    vector<string> tokens = splitSpace(line);
    if (tokens.size() < 2 || tokens[1] != "->") {
      cerr << "无效的产生式格式：" << line << endl;
      return 1;
    }
    string lhs = tokens[0];
    vector<string> rhs(tokens.begin() + 2, tokens.end());
    productions[lhs].push_back(rhs);
  }

  auto first_sets = getFIRST();

  for (const auto& nt : not_t) {
    cout << "FIRST(" << nt << ") = { ";
    vector<string> first_list(first_sets[nt].begin(), first_sets[nt].end());
    sort(first_list.begin(), first_list.end());
    for (size_t i = 0; i < first_list.size(); ++i) {
      if (first_list[i] != "EPSILON") {
        cout << first_list[i];
      }
      else {
        cout << "ε";
      }
      if (i < first_list.size() - 1) cout << ", ";
    }
    cout << " }" << endl;
  }

  return 0;
}