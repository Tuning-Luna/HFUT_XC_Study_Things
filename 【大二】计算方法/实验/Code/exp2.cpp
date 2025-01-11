#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

double f(double x) {
  if (x == 0)
    return 1;
  return sin(x) / x;
}

//�������ι�ʽ
double Trape(double a, double b, double e) {
  double h = b - a;
  double t1 = (h / 2) * (f(a) + f(b)), t2;
  double t;
  double s;
  double x;
  while (fabs(t2 - t) < e) {
    s = 0;
    x = a + (h / 2.0);
    if (x <= b) {
      s = s + f(x);
      x = x + h;
    }
    // cout << s << endl;
    t2 = (t1 / 2.0) + (h / 2.0) * s;

    t = t1;
    t1 = t2;
  }
  return t2;
}


//����Simpson��ʽ
double Simpson(double a, double b, double e) {
  //Page 65 2-1
  double h = b - a;
  double t1 = (h / 6.0) * (f(a) + f(b));
  double t;
  double t2;
  double s;
  double x;
  while (fabs(t2 - t) < e) {
    s = 0;
    x = a + (h / 2.0);
    if (x <= b) {
      s = s + 2 * f(x);
      x = x + (h / 2);
      s = s + 4 * f(x);
      x = x + (h / 2);
    }
    t2 = (t1 / 6.0) + (h / 6.0) * s;
    t = t1;
    t1 = t2;
  }
  return t2;
}


//�������㷨
double Romberg(double a, double b, double e) {
  double h = b - a;
  double t1 = (h / 2) * (f(a) + f(b)), t2;
  double s1, s2;
  double c1, c2;
  double r1, r2;
  int k = 1;
  cout << "0" << "\t" << t1 << "\t";
  while (fabs(r2 - r1) < e) {
    t2 = (h / 2) * (f(a) + f(b));
    cout << endl;
    cout << k << "\t";
    c1 = c2;
    r1 = r2;


    auto func = [](double a, double b, int k) ->double {
      double h = b - a;
      double t1 = (h / 2) * (f(a) + f(b)), t2;
      double t;
      double s;
      double x;
      for (int i = 0;i < k; i++) {
        s = 0;
        x = a + (h / 2.0);
        if (x <= b) {
          s = s + f(x);
          x = x + h;
        }
        t2 = (t1 / 2.0) + (h / 2.0) * s;
        t = t1;
        t1 = t2;
      }
      return t2;
      };

    t2 = func(a, b, k);
    cout << t2 << "\t";
    s2 = (4 * t2 - t1) / 3;
    cout << s2 << "\t";
    if (k == 1) {
      ++k;
      h /= 2;
      t1 = t2;
      s1 = s2;
      continue;
    }
    c2 = (16 * s2 - s1) / 15;
    cout << c2 << "\t";
    if (k == 2) {
      ++k;
      h /= 2;
      t1 = t2;
      s1 = s2;
      c1 = c2;
      continue;
    }
    r2 = (64 * c2 - c1) / 63;
    cout << r2 << "\t";
    if (k == 3) {
      ++k;
      h /= 2;
      t1 = t2;
      s1 = s2;
      c1 = c2;
      r1 = r2;
      continue;
    }
  }
  cout << endl;
  return r2;
}

int main() {
  cout << "�������ι�ʽ=" << Trape(0.0, 1.0, 0.000001) << endl;
  cout << "������������ʽ=" << Simpson(0.0, 1.0, 0.000001) << endl;
  cout << Romberg(0.0, 1.0, 0.000001) << endl;
  return 0;
}