#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

// 定义方程 f(x) 和其导数 f'(x)
double f(double x) {
  return pow(x, 3) - x - 1;
}

double df(double x) {
  return 3 * pow(x, 2) - 1;
}

void solve(double init, double tol, int iter_times, int max_steps) {
  double x = init; // 初值
  int iter = 0;

  cout << "迭代步骤\t近似根\t\t下山因子" << endl;

  while (iter < iter_times) {
    double fx = f(x);
    double dfx = df(x);

    double delta = -fx / dfx;
    double factor = 1.0;
    int steps = 0;

    while (steps < max_steps) {
      double new_x = x + factor * delta;
      if (fabs(f(new_x)) < fabs(fx)) {
        x = new_x;
        break;
      }
      factor /= 2.0; //下山因子减半
      steps++;
    }

    cout << iter + 1 << "\t\t" << setprecision(10) << x << "\t\t" << factor << endl;

    if (fabs(f(x)) < tol) {
      cout << "找到近似根：" << setprecision(10) << x << endl;
      return;
    }
    ++iter;
  }
}

int main() {
  double init = 0.6f;
  double tol;
  int iter_times;
  int max_steps;

  // 输入初值、误差限、最大迭代次数和最大下山次数
  cout << "初值为0.6" << endl;
  cout << "输入误差限: ";
  cin >> tol;
  cout << "输入最大迭代次数: ";
  cin >> iter_times;
  cout << "输入最大下山次数: ";
  cin >> max_steps;

  solve(init, tol, iter_times, max_steps);
  return 0;
}