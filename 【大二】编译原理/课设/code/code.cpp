#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

// ���տո�ָ��ַ���
vector<string> splitSpace(const string& s) {
  vector<string> res;
  stringstream ss(s);
  string token;
  while (ss >> token) {
    res.push_back(token);
  }
  return res;
}

// �ս������
set<string> t = {};
// ���ս������
set<string> not_t = {};
// ����ʽ
map<string, vector<vector<string>>>productions = {};
// ���������� FIRST ��
map<string, set<string>> getFIRST() {
  // ������ӳ�䣬��Ϊ���ս����ֵΪ�� FIRST ��
  map<string, set<string>> ans;

  // ��ʼ����Ϊÿ�����ս������һ���յ� FIRST ��
  for (const auto& nt : not_t) {
    ans[nt] = set<string>();
  }

  // ʹ�ù̶����������ֱ�� FIRST �����ٱ仯
  bool changed = true;
  while (changed) {
    changed = false;

    // ����ÿ�����ս��
    for (const auto& nt : not_t) {
      // �����÷��ս�������в���ʽ
      for (const auto& rhs : productions.at(nt)) {
        // ��ʱ�洢��ǰ����ʽ�Ҳ����ŵ� FIRST ������
        set<string> first_alpha;
        // ��־�Ƿ����з��Ŷ����Ƶ����մ�
        bool can_derive_epsilon = true;

        // ����ղ���ʽ (�� A -> ��)
        if (rhs.empty()) {
          // ��� EPSILON ���� FIRST(nt) �У����벢��Ǳ仯
          if (ans[nt].find("EPSILON") == ans[nt].end()) {
            ans[nt].insert("EPSILON");
            changed = true;
          }
          continue;
        }

        // ��������ʽ�Ҳ���ÿ������
        for (size_t i = 0; i < rhs.size(); ++i) {
          const string& symbol = rhs[i];

          // ����������ս���� EPSILON
          if (t.find(symbol) != t.end() || symbol == "EPSILON") {
            // ���ս������ FIRST(nt)������¼������Ǳ仯
            if (ans[nt].find(symbol) == ans[nt].end()) {
              ans[nt].insert(symbol);
              changed = true;
            }
            // �ս�������Ƶ����մ���ֹͣ�����������
            can_derive_epsilon = false;
            break;
          }
          // ��������Ƿ��ս��
          else {
            // �� FIRST(symbol) �г� EPSILON ��ķ��ż��� FIRST(nt)
            for (const auto& t : ans[symbol]) {
              if (t != "EPSILON") {
                if (ans[nt].find(t) == ans[nt].end()) {
                  ans[nt].insert(t);
                  changed = true;
                }
              }
            }
            // ��� FIRST(symbol) ���� EPSILON��ֹͣ�����������
            if (ans[symbol].find("EPSILON") == ans[symbol].end()) {
              can_derive_epsilon = false;
              break;
            }
          }
        }

        // �������ʽ�Ҳ����з��Ŷ����Ƶ����մ����ҷǿղ���ʽ
        if (can_derive_epsilon && !rhs.empty()) {
          // �� EPSILON ���� FIRST(nt)������¼������Ǳ仯
          if (ans[nt].find("EPSILON") == ans[nt].end()) {
            ans[nt].insert("EPSILON");
            changed = true;
          }
        }

      }
    }
  }

  // ���ؼ���õ� FIRST ��
  return ans;
}

int main() {
  /*
��һ�У��ո�ָ����ս���б��硰a b������
�ڶ��У��ո�ָ��ķ��ս���б��硰S A B������
�����У���ʼ���ţ�һ�����ս�����硰S������
�����У�����ʽ����ʽΪ��A -> X1 X2 ... Xk��������AΪ���ս����X1 X2 ... XkΪ�ս������ս�����մ��á�EPSILON����ʾ��
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
FIRST(A) = { �մ�, a }
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
FIRST(E1) = { +, �� }
FIRST(F) = { (, i }
FIRST(T) = { (, i }
FIRST(T1) = { *, �� }
  */

  string line = "";

  // ��ȡVT
  getline(cin, line);
  auto tList = splitSpace(line);
  t.insert(tList.begin(), tList.end());

  // ��ȡVN
  getline(cin, line);
  auto ntList = splitSpace(line);
  not_t.insert(ntList.begin(), ntList.end());

  // ��ȡ��ʼ����
  getline(cin, line);
  string start_symbol = line;

  // ��ȡ����ʽ
  for (const auto& nt : not_t) {
    productions[nt] = vector<vector<string>>();
  }
  while (getline(cin, line) && !line.empty()) {
    vector<string> tokens = splitSpace(line);
    if (tokens.size() < 2 || tokens[1] != "->") {
      cerr << "��Ч�Ĳ���ʽ��ʽ��" << line << endl;
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
        cout << "��";
      }
      if (i < first_list.size() - 1) cout << ", ";
    }
    cout << " }" << endl;
  }

  return 0;
}