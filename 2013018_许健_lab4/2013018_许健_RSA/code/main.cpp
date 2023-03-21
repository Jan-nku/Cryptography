#include<iostream>
#include<ctime>
#include"BigInt.h"
#include"RSA.h"
using namespace std;

void menu()
{//�˵���ʾ����
	cout << "==========Welcome to use RSA encoder==========" << endl;
	cout << "               e.encrypt ����              " << endl;
	cout << "               d.decrypt ����              " << endl;
	cout << "               s.setkey ����               " << endl;
	cout << "               p.print ��ʾ               " << endl;
	cout << "               q.quit �˳�                 " << endl;
	cout << "input your choice:" << endl;
}
int run() {
	char ch;
	string str;
	Rsa rsa;
	BigInt c, m;

	cout << "����λ��:";
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
			cout << "��������" << endl;
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