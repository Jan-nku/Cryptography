#include<iostream>
#include<ctime>
#include"BigInt.h"
#include"RSA.h"
#include"AES.h"
#include"Socket.h"
using namespace std;
Rsa UserA, UserB;

int main() {
	Sleep(1000);
	init(UserB, 256);
	cout << UserB << endl;

	Connect();
	RSAshare();
	AESshare();
	communicate();

}