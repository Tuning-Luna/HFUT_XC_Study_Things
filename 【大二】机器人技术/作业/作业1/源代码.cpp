#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
using namespace std;

class Analysis {
public:
	Analysis() = delete;
	Analysis(const Analysis& other) = delete;
	~Analysis() = delete;

	static void parse(string str) {
		string ans = "";
		if (str[1] == 'h') {
			Analysis::hear_begin(str);
			Analysis::see_last(str);
		}
		else if (str[1] == 's') {
			Analysis::see_begin(str);
			Analysis::hear_last(str);
		}
	}

private:
	static void hear_begin(string str) {
		string data = str.substr(6, 4);
		string dir;
		if (str[9] == '-') {
			dir = str.substr(10, 3);
		}
		else {
			dir = str.substr(11, 2);
		}
		string hear_something = str.substr(str.find("pass"), 13);
		cout << "��" << data << "����hear��" << dir << "����������" << hear_something << ";" << endl;
	}

	static void see_last(string str) {
		string see_part = str.substr(str.find("(see"));
		string data = see_part.substr(see_part.find("ball"), 4);
		cout << "��" << data << "����see ";
		cout << see_part.substr(11, 4)
			<< "�����ҵ�Direction��" << see_part.substr(16, 3) << ","
			<< "Distance��" << see_part.substr(20, 2) << ","
			<< "DirChng��" << see_part.substr(23, 1) << ","
			<< "DistChng��" << see_part.substr(24, 2) << ";";
		if (str.find("(player") != string::npos) {
			Analysis::see_last_player(str);
		}
	}

	static void see_last_player(string str) {
		string player_part = str.substr(str.find("(player"));
		cout << player_part.substr(1, 14)
			<< "�����ҵ�Direction��" << player_part.substr(16, 2) << ","
			<< "Distance��" << player_part.substr(19, 2) << ","
			<< "DirChng��" << player_part.substr(22, 3) << ","
			<< "DistChng��" << player_part.substr(26, 1) << ","
			<< "����BodyDir��" << player_part.substr(28, 2)
			<< "��HeadDir��" << player_part.substr(31, 2) << ";";
		if (str.find("(goal") != string::npos) {
			Analysis::see_last_goal(str);
		}
	}

	static void see_last_goal(string str) {
		string goal_part = str.substr(str.find("(goal"));
		cout << goal_part.substr(1, 6)
			<< "�����ҵ�Direction��" << goal_part.substr(8, 2) << ","
			<< "Distance��" << goal_part.substr(11, 2) << ";";
		if (str.find("(Line r") != string::npos) {
			Analysis::see_last_Line(str);
		}
	}

	static void see_last_Line(string str) {
		string line_part = str.substr(str.find("(Line"));
		cout << line_part.substr(1, 6)
			<< "�����ҵ�Direction��" << line_part.substr(8, 3);
		if (str.substr(str.size() - 2) == "))") {
			cout << endl;
			return;
		}
		else {
			Analysis::hear_last(str);
		}
	}

	static void see_begin(string str) {
		string data = str.substr(5, 4);
		cout << "��" << data << "����see ";
		if (str.find("((player") == 9) {
			Analysis::see_begin_player(str);
		}
	}

	static void see_begin_player(string str) {
		string player_part = str.substr(str.find("(player"));
		cout << player_part.substr(1, 14)
			<< "�����ҵ�Direction��" << player_part.substr(16, 2) << ","
			<< "Distance��" << player_part.substr(19, 2) << ","
			<< "DirChng��" << player_part.substr(22, 1) << ","
			<< "DistChng��" << player_part.substr(24, 1) << ",";
		if ((player_part[26] >= '0' && player_part[26] <= '9') && (player_part[27] >= '0' && player_part[27] <= '9')) {
			cout << "����BodyDir��" << player_part.substr(26, 2) << "��HeadDir��" << player_part.substr(29, 2) << ";";
		}
		else {
			cout << "����BodyDir��" << player_part.substr(26, 1) << "��HeadDir��" << player_part.substr(28, 1) << ";";
		}
		if (str.find("((player", str.find("((player") + 1) == string::npos) {
			if (str.find("((goal") != string::npos) {
				Analysis::see_begin_goal(str);
			}
			else {
				Analysis::see_begin_player(str);
			}
		}
		else {
			Analysis::see_begin_player_2(str);
		}
	}

	static void see_begin_player_2(string str) {
		string player_part = str.substr(str.find("(player", str.find("(player") + 5));
		cout << player_part.substr(1, 14)
			<< "�����ҵ�Direction��" << player_part.substr(16, 1) << ","
			<< "Distance��" << player_part.substr(18, 1) << ","
			<< "DirChng��" << player_part.substr(20, 1) << ","
			<< "DistChng��" << player_part.substr(22, 2) << ","
			<< "����BodyDir��" << player_part.substr(25, 2)
			<< "��HeadDir��" << player_part.substr(28, 1) << ";";
		if (player_part.find("(f r t") != string::npos) {
			Analysis::see_begin_frt(str);
		}
		else {
			Analysis::see_begin_goal(str);
			cout << endl;
		}
	}

	static void see_begin_goal(string str) {
		string goal_part = str.substr(str.find("(goal"));
		cout << goal_part.substr(1, 6)
			<< "�����ҵ�Direction��" << goal_part.substr(8, 2) << ","
			<< "Distance��" << goal_part.substr(11, 2) << ";";
		if (str.find("(Line r") != string::npos) {
			Analysis::see_last_Line(str);
		}
		else if (str.find("(f r t") != string::npos) {
			Analysis::see_begin_frt(str);
		}
	}

	static void see_begin_frt(string str) {
		string str_part = str.substr(str.find("(f r t"));
		if (str_part[11] == ' ') {
			cout << str_part.substr(1, 8)
				<< "�����ҵ�Direction��" << str_part.substr(10, 1) << ","
				<< "Distance��" << str_part.substr(12, 2) << ";" << endl;
		}
		else {
			cout << str_part.substr(1, 8)
				<< "�����ҵ�Direction��" << str_part.substr(10, 2) << ","
				<< "Distance��" << str_part.substr(13, 2) << ";" << endl;
		}
	}

	static void hear_last(string str) {
		string hear_part = str.substr(str.find("(hear"));
		string data = hear_part.substr(6, 4);
		string dir;
		if (hear_part[10] == '-') {
			dir = hear_part.substr(10, 3);
		}
		else {
			dir = hear_part.substr(11, 2);
		}
		if (hear_part.find("pass") != string::npos) {
			string hear_something = hear_part.substr(hear_part.find("p"), 9);
			cout << "��" << data << "����hear��" << dir << "����������" << hear_something << ";" << endl;
		}
		else {
			string hear_something = hear_part.substr(hear_part.find("d"), 4);
			cout << "��" << data << "����hear��" << dir << "����������" << hear_something << ";" << endl;
		}
	}
};

int main() {
	vector<string> v = {};
	v.push_back("(hear 1022-30 passto(23,24))(see 1022((ball)-20 20 1-2)((player hfut1 2)45 23 0.5 1 22 40)((goal r)12 20)((Line r)-30))");
	v.push_back("(see 2022((player hfut2 8)40 23 1 6 21 33)((goal r)15 30)((f r t 20)5 24))(hear 2022-10 ��pass ball��)");
	v.push_back("(see 2023((player hfut2 8)10 20 1 2 6 8)((player hfut1 9)6 7 8 10 20 3)((f r t 10)15 20))(hear 2023 10��dash��)");

	for_each(v.begin(), v.end(), [](const string& str) {
		cout << "ԭ��Ϣ��" << endl;
		cout << str << endl;
		cout << "������" << endl;
		Analysis::parse(str); cout << endl;
		});
	return 0;
}
