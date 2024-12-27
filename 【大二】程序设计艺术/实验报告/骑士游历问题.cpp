int cnt = 0; //统计走的顺序
vector<vector<int>> Rect(8, vector<int>(8,0));

bool is_legal(int x, int y){
	return (x >= 0 && x <= 7 && y >= 0 && y <= 7);
}

void DFS(int x, int y){
	if (cnt == 64) //走完了 棋盘上有 1-64 直接返回
		return;
	//如果没有出界且没有被访问,访问之
	if (is_legal(x, y) && Rect[x][y] == 0){
		++cnt; //走完一步增加一步
		Rect[x][y] = cnt;
		cout << "(" << x << "," << y << ") "; // 打印当前位置
		if (cnt % 8 == 0)
			cout << endl;

		DFS(x + 2, y + 1);	
		DFS(x + 2, y - 1);

		DFS(x - 2, y - 1);  
		DFS(x - 2, y + 1);

		DFS(x + 1, y + 2);
		DFS(x + 1, y - 2);
					   
		DFS(x - 1, y - 2);
		DFS(x - 1, y + 2);
		return;
	}
}
int main(){
	int x, y;
	cout << "输入起始坐标：";
	cin >> x >> y;
	if (!is_legal(x, y))
		throw runtime_error("Out of range.");
	cout << "依次经过的点为：" << endl;
	DFS(x, y);

	//for (const auto& v : Rect) {
	//	for (const auto& n : v) {
	//		cout << n <<"\t";
	//	}
	//	cout << endl << endl << endl;
	//}
	return 0;
}