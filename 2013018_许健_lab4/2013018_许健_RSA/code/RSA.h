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
	void init(unsigned int n);										//��ʼ��
	BigInt encryption(const BigInt& m);								//��Կ����
	BigInt decryption(const BigInt& c);								//˽Կ����
	friend ostream& operator <<(ostream& out, const Rsa& rsa);		//������������

private:
	BigInt createOddNum(unsigned int n);							//���ɳ���Ϊn������
	bool rabinMiller(const BigInt& a, const unsigned int k);		//�ж�����
	BigInt createPrime(unsigned int n, int it_cout);				//���ɳ���Ϊn������
	void createExp(const BigInt& ou);								//��һ��ŷ���������ɹ�Կ��˽Կ
	BigInt createRandom(const BigInt& a);							//����С��a�������
};

void init(Rsa& rsa, int n);											//��ʼ��nλRSA
bool islegal(const string& str);									//�ж�����Ϸ���
bool Encrypt(Rsa& rsa, BigInt& c);									//RSA���ܷ�װ
bool Decrypt(Rsa& rsa, BigInt& m);									//RSA���ܷ�װ

#endif // !RSA_H

