#include<iostream>
#include<ctime>
#include"BigInt.h"
#include"RSA.h"
#include"AES.h"
#include"Socket.h"
using namespace std;
Rsa UserA, UserB;


int main() {

	init(UserA, 256);
	cout << UserA << endl;
	Connect();
	RSAshare();
	AESshare();
	communicate();

}