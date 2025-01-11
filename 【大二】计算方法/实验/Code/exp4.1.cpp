#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

vector<double> solve(const vector<vector<double>>& A, const vector<double>& b, int iter_times, double tol) {
  int n = A.size();
  vector<double> x(n, 0);
  vector<double> xOld(n);

  for (int iter = 0; iter < iter_times; iter++) {
    xOld = x;
    for (int i = 0; i < n; i++) {
      double sigma = 0;
      for (int j = 0; j < n; j++) {
        if (j != i) sigma += A[i][j] * x[j];
      }
      x[i] = (b[i] - sigma) / A[i][i];
    }

    double t = 0;
    for (int i = 0; i < n; i++) {
      t = max(t, fabs(x[i] - xOld[i]));
    }
    if (t < tol) {
      cout << "收敛" << endl;
      return x;
    }
  }
  cout << "未收敛" << endl;
  return x;
}

int main() {
  vector<vector<double>> A = {
      {10, -1, -2},
      {-1, 10, -2},
      {-1, -1, 5}
  };
  vector<double> b = { 7.2, 8.3, 4.2 };
  int iter_times = 100;
  double tol = 1e-6;

  vector<double> ans = solve(A, b, iter_times, tol);
  cout << "结果为: ";
  for (const double num : ans) {
    cout << num << " ";
  }
  return 0;
}
