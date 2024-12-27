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
	Point _center; // 圆心
	double _r;     // 半径

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
	//四个顶点 依次是：左上 右上 左下 右下
	//矩形的四条边平行于坐标轴 x,y
	vector<Point> _points = {};

public:
	//按顺序 依次是：左上 右上 左下 右下
	Rectangle(const initializer_list<Point>& points) {
		if (points.size() != 4) {
			cout << "只能传入4个顶点坐标！" << endl;
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
	// ax + by + c = 0 的形式
	double _a;
	double _b;
	double _c;
public:
	friend void test();

	Line(double a = 0.0f, double b = 0.0f, double c = 0.0f)
		:_a(a), _b(b), _c(c) {}

	~Line() = default;


	//直线与直线交点
	vector<Point> intersection_point_with_Line(const Line& line)const {
		vector<Point> ans;
		double a1 = this->_a, b1 = this->_b, c1 = this->_c;
		double a2 = line._a, b2 = line._b, c2 = line._c;

		// 求行列式的值
		double D = a1 * b2 - a2 * b1;
		//平行or重合
		if (fabs(D) < 1e-6) {
			return ans;
		}

		// 求交点坐标
		double Dx = c1 * b2 - c2 * b1;
		double Dy = a1 * c2 - a2 * c1;
		double x = -Dx / D;
		double y = -Dy / D;
		ans.push_back(Point(x, y));
		return ans;
	}

	//直线与圆交点                           
	vector<Point> intersection_point_with_Circular(const Circular& cir)const {
		vector<Point> ans;
		// 圆心坐标
		double x0 = cir.get_center()._x;
		double y0 = cir.get_center()._y;
		double r = cir.get_r();

		// 直线方程: ax + by + c = 0
		double a = this->_a, b = this->_b, c = this->_c;

		//竖直线的情况
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
			//将直线化为 y = kx + m
			double k = -a / b;
			double m = -c / b;

			//带入圆的方程
			double A = 1 + k * k;
			double B = 2 * (k * m - k * y0 - x0);
			double C = x0 * x0 + y0 * y0 + m * m - 2 * m * y0 - r * r;

			//根的判别式
			double delta = B * B - 4 * A * C;
			if (delta == 0) { //一个交点
				double x1 = (-B) / (2 * A);
				double y1 = k * x1 + m;
				ans.push_back(Point(x1, y1));
				cout << "Test" << endl;
			}
			if (delta > 0) { //两个交点
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

	//直线与矩形交点
	vector<Point> intersection_point_with_Rectangle(Rectangle& rect)const {
		vector<Point> ans;
		vector<Point> points = rect.get_points();

		// 矩形的四条边 分别是上下左右
		Line edges[4] = {
			Line(0, 1, -points[0]._y),
			Line(0, 1, -points[2]._y),
			Line(1, 0, -points[0]._x),
			Line(1, 0, -points[1]._x)
		};

		//遍历每一条边 依次看是否有交点
		for (const Line& edge : edges) {
			//得到每一条边和直线的交点
			vector<Point> i_points = this->intersection_point_with_Line(edge);
			for (const Point& p : i_points) {
				//检查交点是否在矩形的边上
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
	cout << "直线 x + 2y + 3 = 0 :" << endl<<endl;

	Line l2(1, -2, 5);
	auto v1 = l1.intersection_point_with_Line(l2);
	cout << "与直线 x - 2y + 5 = 0 的交点为：" << endl;
	if (v1.empty()) {
		cout << "无交点" << endl;
	}
	else {
		for (const Point& p : v1) {
			cout << p << " ";
		}
	}cout << endl << endl;

	cout << "与圆 (x+1)^2 + (y-1)^2 = 4^2 的交点为：" << endl;
	Circular c(Point(-1, 1), 4);
	auto v2 = l1.intersection_point_with_Circular(c);
	if (v2.empty()) {
		cout << "无交点" << endl;
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

	cout << "与矩形 (-6,2) , (0,2) , (-6,-2) , (0,-2) 的交点为：" << endl;
	Rectangle r({ Point(-6, 2), Point(0, 2), Point(-6, -2), Point(0, -2) });
	auto v3 = l1.intersection_point_with_Rectangle(r);
	if (v3.empty()) {
		cout << "无交点" << endl;
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