#include <iostream>
#include <vector>
using namespace std;
struct Point {
	double _x;
	double _y;
	Point(double x = 0.0f, double y = 0.0f)
		: _x(x), _y(y) {}
	~Point() = default;

	friend ostream& operator<<(ostream& out, const Point& p) {
		out << "(" << p._x << "," << p._y << ")";
		return out;
	}
};

class Circular {
private:
	Point _center; // Բ��
	double _r;     // �뾶

public:
	Circular(const Point& p = {}, double r = 0.0f)
		:_center(p), _r(r) {}
	~Circular() = default;

	const Point& get_center()const {
		return this->_center;
	}

	const double& get_r()const {
		return this->_r;
	}
};

class Rectangle {
private:
	//�ĸ����� �����ǣ����� ���� ���� ����
	//���ε�������ƽ���������� x,y
	vector<Point> _points = {};

public:
	//��˳�� �����ǣ����� ���� ���� ����
	Rectangle(const initializer_list<Point>& points) {
		if (points.size() != 4) {
			cout << "ֻ�ܴ���4���������꣡" << endl;
			return;
		}
		this->_points.clear();
		for (const Point& point : points) {
			this->_points.push_back(point);
		}
	}

	const vector<Point>& get_points()const {
		return this->_points;
	}
};

class Line {
private:
	// ax + by + c = 0 ����ʽ
	double _a;
	double _b;
	double _c;
public:
	friend void test();

	Line(double a = 0.0f, double b = 0.0f, double c = 0.0f)
		:_a(a), _b(b), _c(c) {}

	~Line() = default;


	//ֱ����ֱ�߽���
	vector<Point> intersection_point_with_Line(const Line& line)const {
		vector<Point> ans;
		double a1 = this->_a, b1 = this->_b, c1 = this->_c;
		double a2 = line._a, b2 = line._b, c2 = line._c;

		// ������ʽ��ֵ
		double D = a1 * b2 - a2 * b1;
		//ƽ��or�غ�
		if (fabs(D) < 1e-6) {
			return ans;
		}

		// �󽻵�����
		double Dx = c1 * b2 - c2 * b1;
		double Dy = a1 * c2 - a2 * c1;
		double x = -Dx / D;
		double y = -Dy / D;
		ans.push_back(Point(x, y));
		return ans;
	}

	//ֱ����Բ����                           
	vector<Point> intersection_point_with_Circular(const Circular& cir)const {
		vector<Point> ans;
		// Բ������
		double x0 = cir.get_center()._x;
		double y0 = cir.get_center()._y;
		double r = cir.get_r();

		// ֱ�߷���: ax + by + c = 0
		double a = this->_a, b = this->_b, c = this->_c;

		//��ֱ�ߵ����
		if (fabs(b) < 1e-6) {
			double x = -c / a;
			double delta = r * r - (x - x0) * (x - x0);
			if (delta > 0) {
				double y1 = y0 + sqrt(delta);
				double y2 = y0 - sqrt(delta);
				ans.push_back(Point(x, y1));
				ans.push_back(Point(x, y2));
			}
			if (delta == 0) {
				ans.push_back(Point(x, y0));
			}
		}
		else {
			//��ֱ�߻�Ϊ y = kx + m
			double k = -a / b;
			double m = -c / b;

			//����Բ�ķ���
			double A = 1 + k * k;
			double B = 2 * (k * m - k * y0 - x0);
			double C = x0 * x0 + y0 * y0 + m * m - 2 * m * y0 - r * r;

			//�����б�ʽ
			double delta = B * B - 4 * A * C;
			if (delta == 0) { //һ������
				double x1 = (-B) / (2 * A);
				double y1 = k * x1 + m;
				ans.push_back(Point(x1, y1));
				cout << "Test" << endl;
			}
			if (delta > 0) { //��������
				double x1 = (-B + sqrt(delta)) / (2 * A);
				double y1 = k * x1 + m;
				ans.push_back(Point(x1, y1));

				double x2 = (-B - sqrt(delta)) / (2 * A);
				double y2 = k * x2 + m;
				ans.push_back(Point(x2, y2));
			}
		}
		return ans;
	}

	//ֱ������ν���
	vector<Point> intersection_point_with_Rectangle(Rectangle& rect)const {
		vector<Point> ans;
		vector<Point> points = rect.get_points();

		// ���ε������� �ֱ�����������
		Line edges[4] = {
			Line(0, 1, -points[0]._y),
			Line(0, 1, -points[2]._y),
			Line(1, 0, -points[0]._x),
			Line(1, 0, -points[1]._x)
		};

		//����ÿһ���� ���ο��Ƿ��н���
		for (const Line& edge : edges) {
			//�õ�ÿһ���ߺ�ֱ�ߵĽ���
			vector<Point> i_points = this->intersection_point_with_Line(edge);
			for (const Point& p : i_points) {
				//��齻���Ƿ��ھ��εı���
				if (p._x >= min(points[0]._x, points[1]._x) && p._x <= max(points[0]._x, points[1]._x) &&
					p._y >= min(points[0]._y, points[2]._y) && p._y <= max(points[0]._y, points[2]._y)) {
					ans.push_back(p);
				}
			}
		}
		return ans;
	}
};

void test() {
	Line l1(1, 2, 3);
	cout << "ֱ�� x + 2y + 3 = 0 :" << endl<<endl;

	Line l2(1, -2, 5);
	auto v1 = l1.intersection_point_with_Line(l2);
	cout << "��ֱ�� x - 2y + 5 = 0 �Ľ���Ϊ��" << endl;
	if (v1.empty()) {
		cout << "�޽���" << endl;
	}
	else {
		for (const Point& p : v1) {
			cout << p << " ";
		}
	}cout << endl << endl;

	cout << "��Բ (x+1)^2 + (y-1)^2 = 4^2 �Ľ���Ϊ��" << endl;
	Circular c(Point(-1, 1), 4);
	auto v2 = l1.intersection_point_with_Circular(c);
	if (v2.empty()) {
		cout << "�޽���" << endl;
	}
	else {
		for (const Point& p : v2) {
			cout << p << " ";
		}
	}cout << endl << endl;

	vector<Point>vp;
	vp.push_back(Point(-6, 2));
	vp.push_back(Point(0, 2));
	vp.push_back(Point(-6, -2));
	vp.push_back(Point(0, -2));

	cout << "����� (-6,2) , (0,2) , (-6,-2) , (0,-2) �Ľ���Ϊ��" << endl;
	Rectangle r({ Point(-6, 2), Point(0, 2), Point(-6, -2), Point(0, -2) });
	auto v3 = l1.intersection_point_with_Rectangle(r);
	if (v3.empty()) {
		cout << "�޽���" << endl;
	}
	else {
		for (const Point& p : v3) {
			cout << p << " ";
		}
	}cout << endl;
}


int main() {
	test();
	return 0;
}