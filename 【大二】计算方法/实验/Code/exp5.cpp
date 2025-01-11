#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

double f(double x, double y) {
  return y - (2 * x) / y;
}

double Exact(double x) {
  return sqrt(1 + 2 * x);
}

void Euler(double x0, double y0, double h, int n) {
  cout << "Euler法:" << endl;
  cout << setw(10) << "x" << setw(15) << "y (计算值)" << setw(15) << "y (精确值)" << setw(15) << "误差" << endl;

  for (int i = 0; i <= n; i++) {
    double exact = Exact(x0);
    cout << setw(10) << x0 << setw(15) << y0 << setw(15) << exact << setw(15) << abs(y0 - exact) << endl;
    y0 += h * f(x0, y0);
    x0 += h;
  }
  cout << endl;
}

void Euler_improved(double x0, double y0, double h, int n) {
  cout << "改进Euler法:" << endl;
  cout << setw(10) << "x" << setw(15) << "y (计算值)" << setw(15) << "y (精确值)" << setw(15) << "误差" << endl;

  for (int i = 0; i <= n; i++) {
    double exact = Exact(x0);
    cout << setw(10) << x0 << setw(15) << y0 << setw(15) << exact << setw(15) << abs(y0 - exact) << endl;
    double predict = y0 + h * f(x0, y0);
    y0 += h / 2 * (f(x0, y0) + f(x0 + h, predict));
    x0 += h;
  }
  cout << endl;
}

void Rung_Kutta(double x0, double y0, double h, int n) {
  cout << "Rung-Kutta方法:" << endl;
  cout << setw(10) << "x" << setw(15) << "y (计算值)" << setw(15) << "y (精确值)" << setw(15) << "误差" << endl;

  for (int i = 0; i <= n; i++) {
    double exact = Exact(x0);
    cout << setw(10) << x0 << setw(15) << y0 << setw(15) << exact << setw(15) << abs(y0 - exact) << endl;
    double k1 = f(x0, y0);
    double k2 = f(x0 + h / 2, y0 + h / 2 * k1);
    double k3 = f(x0 + h / 2, y0 + h / 2 * k2);
    double k4 = f(x0 + h, y0 + h * k3);
    y0 += h / 6 * (k1 + 2 * k2 + 2 * k3 + k4);
    x0 += h;
  }
  cout << endl;
}

int main() {
  double x0 = 0.0, y0 = 1.0;
  double h = 0.1;
  double x_end = 1.0;
  int n = (x_end - x0) / h;

  Euler(x0, y0, h, n);
  Euler_improved(x0, y0, h, n);
  Rung_Kutta(x0, y0, h, n);

  return 0;
}
