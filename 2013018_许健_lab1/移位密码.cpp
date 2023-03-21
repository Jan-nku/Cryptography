#include <iostream>
#include <string>
using namespace std;
string Plaintext;//明文
string Ciphertext;//密文
string Unknown;//要攻击的未知字符串
int Key = 3;//密钥，默认为3
string Encryption(string plaintext, int key)
{
	for (auto& i : plaintext)
	{
		if (i >= 'a' && i <= 'z') {
			i = (i - 97 + key) % 26 + 97;
		}
		else if (i >= 'A' && i <= 'Z') {
			i = (i - 65 + key) % 26 + 65;
		}
		else if (i == ' ');
		else {
			cerr << "请输入正确的明文" << endl;
			break;
		}
	}
	return plaintext;
}
string Decryption(string ciphertext, int key)
{
	for (auto& i : ciphertext)
	{
		if (i >= 'a' && i <= 'z') {
			i = (i - 97 - key + 26) % 26 + 97;
		}
		else if (i >= 'A' && i <= 'Z') {
			i = (i - 65 - key + 26) % 26 + 65;
		}
		else if (i == ' ');
		else {
			cerr << "请输入正确的密文" << endl;
			break;
		}
	}
	return ciphertext;
}
int main()
{
	int flag;
	while (1) {
		cout << "欢迎使用移位密码系统，功能如下：" << endl;
		cout << "	1: 加密解密" << endl;
		cout << "	2: 攻击密文" << endl;
		cout << "	3: 退出" << endl;
		cout << "请输入要进行的操作:";
		(cin >> flag).get();
		switch (flag) {
		case 1:
			cout << "请输入要加密的字符串:";
			getline(cin, Plaintext);
			cout << "请输入要使用的密钥:";
			cin >> Key;
			cout << "明文:" << Plaintext << endl;
			Ciphertext = Encryption(Plaintext, Key);
			cout << "加密后:" << Ciphertext << endl;
			cout << "解密后:" << Decryption(Ciphertext, Key) << endl;
			break;
		case 2:
			cout << "请输入要攻击的密文:";
			getline(cin, Unknown);
			for (int i = 1; i < 26; i++) {
				cout << "Key = " << i << "解密后" << Decryption(Unknown, i) << endl;
			}
			break;
		case 3:
			goto L;
			break;
		default:
			cerr << "请输入正确的操作！" << endl;
		}
	}
L: 
	return 0;
}