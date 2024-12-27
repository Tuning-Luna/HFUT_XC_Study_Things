#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
void show_info(const vector<int>& a,const vector<int>& b,const int L){
	vector<int>::const_iterator ita, itb, it;
	for (it = b.cbegin(); it < b.cend(); it++)	{//在大桶的轨迹b中搜寻指定的刻度
		if (*it == L)
			break;
	}

	if (it == b.cend()){//如果搜寻到b的末尾还没有找到对应刻度，则说明无解
		cout << "无解" << endl;
		exit(0);
	}

	cout << "A" << " | " << "B" << endl;
	if (b.cend() - it - 1 > it - b.cbegin() + 1){
		ita = a.cbegin();
		itb = b.cbegin();
		while (itb <= it){
			cout << *ita << " | " << *itb << endl;
			++ita; 
			++itb;
		}
	}
	else	{
		for (it = b.cend() - 1; it > b.cbegin(); --it)	{
			if (*it == L)
				break;
		}
		ita = a.cend() - 1;
		itb = b.cend() - 1;
		while (itb >= it){
			cout << *ita << " | " << *itb << endl;
			--ita; 
			--itb;
		}
	}
}

void Solve(){
	int A, B, L;
	vector<int> A1, B1;
	int flag = 0;
	cout << "输入两个容器的体积：" << endl;
	cin >> A >> B;
	cout << "输入目标体积：" << endl;
	cin >> L;
	int M = max(A, B);
	int m = min(A, B);
	if (L > M){
		cout << "无解" << endl;
		exit(0);
	}
	if (L == m){
		cout << "A" << " | " << "B" << endl;
		cout << m << " | " << "0" << endl;
		return;
	}
	if (L == M){
		cout << "A" << " | " << "B" << endl;
		cout << "0" << " | " << M << endl;
		return;
	}

	int a = 0, b = 0;
	a = m;
	while (true)	{
		if (b == M){
			b = 0;
			if (a == 0 && b == 0) break;//退出条件
			A1.push_back(a); 
			B1.push_back(b);
			swap(a, b);
			A1.push_back(a);
			B1.push_back(b);
			a = m;
			continue;
		}

		A1.push_back(a); 
		B1.push_back(b);
		b = b + a;
		if (b >= M){
			a = b - M;
			b = M;
			A1.push_back(a); 
			B1.push_back(b);
		}
		else	{
			a = 0;
			A1.push_back(a); 
			B1.push_back(b);
			a = m;
		}
	}
	show_info(A1, B1, L);
}
int main(){
	Solve();
	return 0;
}