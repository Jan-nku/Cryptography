#include<iostream>
#include<ctime>
#include"BigInt.h"
#include"RSA.h"
using namespace std;

void menu()
{//菜单显示函数
	cout << "==========Welcome to use RSA encoder==========" << endl;
	cout << "               e.encrypt 加密              " << endl;
	cout << "               d.decrypt 解密              " << endl;
	cout << "               s.setkey 重置               " << endl;
	cout << "               p.print 显示               " << endl;
	cout << "               q.quit 退出                 " << endl;
	cout << "input your choice:" << endl;
}
int run() {
	char ch;
	string str;
	Rsa rsa;
	BigInt c, m;

	cout << "输入位数:";
	int n;
	cin >> n;
	init(rsa, n / 2);

	while (true) {
		menu();
		cout << ">";
		cin >> str;
		if (!cin)
			return 0;

		if (str.length() < 1)
			cout << "重新输入" << endl;
		else {
			ch = str.at(0);
			switch (ch)
			{
			case 'e':
			case 'E':
				Encrypt(rsa, c);
				break;
			case 'd':
			case 'D':
				Decrypt(rsa, m);
				break;
			case 's':
			case 'S':
				run();
				break;
			case 'p':
			case 'P':
				cout << rsa << endl;
				break;
			case 'q':
			case 'Q':
				return 0;
			}
		}
	}
}

int main() {
	run();
}