#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

double f(double x) {
  return 1.0 / (1 + x * x);
}

//��������
double Lagrange(const vector<double>& x, const vector<double>& y, double point) {
  double ans = 0.0;
  int n = x.size();
  for (int i = 0; i < n; ++i) {
    double term = y[i];
    for (int j = 0; j < n; ++j) {
      if (j != i) {
        term *= (point - x[j]) / (x[i] - x[j]);
      }
    }
    ans += term;
  }
  return ans;
}

//���Բ�ֵ
double Linear(const vector<double>& x, const vector<double>& y, double point) {
  int n = x.size();
  for (int i = 0; i < n - 1; ++i) {
    if (point >= x[i] && point <= x[i + 1]) {
      double t = (y[i + 1] - y[i]) / (x[i + 1] - x[i]);
      return y[i] + t * (point - x[i]);
    }
  }
  return 0.0;
}

void exp1() {
  double left = -5.0, right = 5.0;
  int n = 11;

  vector<double> x(n), y(n);
  double step = (right - left) / (n - 1);
  for (int i = 0; i < n; i++) {
    x[i] = left + i * step;
    y[i] = f(x[i]);
  }

  vector<double> points = { 0.5, 4.5 };

  cout << fixed << setprecision(6);
  cout << "x\t\t"
    << "y(��ȷ)\t\t"
    << "y(��������)\t"
    << "y(�ֶ�����)\t"
    << "���(��)\t"
    << "���(���)" << endl;
  for (const double& point : points) {
    double lag = Lagrange(x, y, point);
    double lin = Linear(x, y, point);
    double real = f(point);
    double lagErr = real - lag;
    double linErr = real - lin;

    cout << point << "\t" << real << "\t" << lag << "\t" << lin << "\t" << lagErr << "\t" << linErr << endl;
  }
}


//ţ�ٲ�ֵ��
double Newton(const vector<double>& x, const vector<double>& y, double point) {
  int n = x.size();
  vector<vector<double>> divDiff(n, vector<double>(n, 0.0));
  for (int i = 0; i < n; ++i) {
    divDiff[i][0] = y[i];
  }
  for (int j = 1; j < n; ++j) {
    for (int i = 0; i < n - j; ++i) {
      divDiff[i][j] = (divDiff[i + 1][j - 1] - divDiff[i][j - 1]) / (x[i + j] - x[i]);
    }
  }

  // �����ֵ���
  double ans = divDiff[0][0];
  double product = 1.0;
  for (int i = 1; i < n; ++i) {
    product *= (point - x[i - 1]);
    ans += divDiff[0][i] * product;
  }
  return ans;
}

void exp2() {
  vector<double> x = { 1, 4, 9 };
  vector<double> y = { 1, 2, 3 };
  double point = 5.0;

  double ans = Newton(x, y, point);

  cout << fixed << setprecision(6);
  cout << "x = " << point << "���Ľ���ֵΪ: " << ans << endl;
}

int main() {
  exp1();
  cout << "-----------------------------------" << endl;
  exp2();
  return 0;
}
