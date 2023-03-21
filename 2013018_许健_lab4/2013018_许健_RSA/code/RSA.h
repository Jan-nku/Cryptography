#ifndef RSA_H
#define RSA_H
#include"BigInt.h"

class Rsa {
public:
	BigInt e, N;
private:
	BigInt d;
	BigInt p, q;
	BigInt ou;

public:
	void init(unsigned int n);										//初始化
	BigInt encryption(const BigInt& m);								//公钥加密
	BigInt decryption(const BigInt& c);								//私钥解密
	friend ostream& operator <<(ostream& out, const Rsa& rsa);		//重载输出运算符

private:
	BigInt createOddNum(unsigned int n);							//生成长度为n的奇数
	bool rabinMiller(const BigInt& a, const unsigned int k);		//判断素数
	BigInt createPrime(unsigned int n, int it_cout);				//生成长度为n的素数
	void createExp(const BigInt& ou);								//从一个欧拉数中生成公钥、私钥
	BigInt createRandom(const BigInt& a);							//创建小于a的随机数
};

void init(Rsa& rsa, int n);											//初始化n位RSA
bool islegal(const string& str);									//判读输入合法性
bool Encrypt(Rsa& rsa, BigInt& c);									//RSA加密封装
bool Decrypt(Rsa& rsa, BigInt& m);									//RSA解密封装

#endif // !RSA_H

