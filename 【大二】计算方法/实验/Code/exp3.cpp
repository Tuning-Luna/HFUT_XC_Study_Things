#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

// ���巽�� f(x) ���䵼�� f'(x)
double f(double x) {
  return pow(x, 3) - x - 1;
}

double df(double x) {
  return 3 * pow(x, 2) - 1;
}

void solve(double init, double tol, int iter_times, int max_steps) {
  double x = init; // ��ֵ
  int iter = 0;

  cout << "��������\t���Ƹ�\t\t��ɽ����" << endl;

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
      factor /= 2.0; //��ɽ���Ӽ���
      steps++;
    }

    cout << iter + 1 << "\t\t" << setprecision(10) << x << "\t\t" << factor << endl;

    if (fabs(f(x)) < tol) {
      cout << "�ҵ����Ƹ���" << setprecision(10) << x << endl;
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

  // �����ֵ������ޡ������������������ɽ����
  cout << "��ֵΪ0.6" << endl;
  cout << "���������: ";
  cin >> tol;
  cout << "��������������: ";
  cin >> iter_times;
  cout << "���������ɽ����: ";
  cin >> max_steps;

  solve(init, tol, iter_times, max_steps);
  return 0;
}