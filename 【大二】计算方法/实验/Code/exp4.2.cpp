#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

double solve(vector<vector<double>> m) {
  int n = m.size();
  double ret = 1.0;
  int swaps = 0;

  for (int i = 0; i < n; ++i) {
    // ѡ��Ԫ
    int maxRow = i;
    for (int k = i + 1; k < n; ++k) {
      if (fabs(m[k][i]) > fabs(m[maxRow][i])) {
        maxRow = k;
      }
    }

    // ������
    if (i != maxRow) {
      swap(m[i], m[maxRow]);
      swaps++;
    }

    // �����Ԫ�Ƿ�Ϊ 0
    if (fabs(m[i][i]) < 1e-10) {
      return 0;  // ����ʽΪ 0
    }

    for (int k = i + 1; k < n; ++k) {
      double factor = m[k][i] / m[i][i];
      for (int j = i; j < n; ++j) {
        m[k][j] -= factor * m[i][j];
      }
    }
  }

  // ��������ʽ
  for (int i = 0; i < n; i++) {
    ret *= m[i][i];
  }

  return (swaps % 2 == 0) ? ret : -ret;
}

int main() {
  vector<vector<double>> m = {
      {3, -2, 1, 4},
      {-7, 5, -3, -6},
      {2, 1, -1, 3},
      {4, -3, 2, 8}
  };

  cout << "��Ϊ: " << solve(m) << endl;

  return 0;
}
