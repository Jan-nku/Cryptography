#include "RSA.h"
#include<iostream>
#include<sstream>
#include<ctime>
#include<cstdlib>
using namespace std;

void Rsa::init(unsigned int n) {
	srand(time(NULL));
	p = createPrime(n, 10);
	q = createPrime(n, 10);
	N = p * q;
	ou = (p - 1) * (q - 1);
	createExp(ou);
}

BigInt Rsa::encryption(const BigInt& m) {
	return m.moden(e, N);
}

BigInt Rsa::inv_encryption(const BigInt& m) {
	return m.moden(d, N);
}

BigInt Rsa::decryption(const BigInt& c)
{
	return c.moden(d, N);
}

BigInt Rsa::inv_decryption(const BigInt& c)
{
	return c.moden(e, N);
}

ostream& operator <<(ostream& out, const Rsa& rsa)
{
	out << "N:" << rsa.N << "\n";
	out << "p:" << rsa.p << "\n";
	out << "q:" << rsa.q << "\n";
	out << "e:" << rsa.e << "\n";
	out << "d:" << rsa.d;
	return out;
}

BigInt Rsa::createOddNum(unsigned int n) {
	n = n / 4;
	unsigned char hex_table[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	if (n) {
		ostringstream oss;
		for (size_t i = 0; i < n - 1; ++i) {
			oss << hex_table[rand() % 16];
		}
		int k;
		do {
			k = rand() % 16;
		} while (k % 2 == 0);
		oss << hex_table[k];
		string str(oss.str());
		return BigInt(str);
	}
	else
		return BigInt::Zero;
}
bool Rsa::rabinMiller(const BigInt& n, const unsigned int k) {
	assert(n != BigInt::Zero);
	if (n == BigInt::Two)
		return true;

	BigInt n_1 = n - 1;
	BigInt::bit b(n_1);
	if (b.at(0) == 1)
		return false;

	for (size_t t = 0; t < k; ++t) {
		BigInt a = createRandom(n_1);
		BigInt d(BigInt::One);
		for (int i = b.size() - 1; i >= 0; --i) {
			BigInt x = d;
			d = (d * d) % n;
			if (d == BigInt::One && x != BigInt::One && x != n_1) {
				return false;
			}

			if (b.at(i)) {
				assert(d != BigInt::Zero);
				d = (a * d) % n;
			}
		}
		if (d != BigInt::One) {
			return false;
		}
	}
	return true;
}
BigInt Rsa::createPrime(unsigned int n, int k) {
	assert(k > 0);
	BigInt res = createOddNum(n);
	while (!rabinMiller(res, k)) {
		res.add(BigInt::Two);
	}
	return res;
}
void Rsa::createExp(const BigInt& ou) {
	e = 65537;
	d = e.extendEuclid(ou);
}
BigInt Rsa::createRandom(const BigInt& a)
{
	unsigned long t = 0;
	do {
		t = rand();
	} while (t == 0);

	BigInt mod(t);
	BigInt r = mod % a;
	if (r == BigInt::Zero)
		r = a - BigInt::One;
	return r;
}

void init(Rsa& rsa, int n)
{
	cout << "��ʼ��...." << endl;
	long t1 = clock();
	rsa.init(n);
	long t2 = clock();
	cout << "��ʼ�����." << endl;
	cout << "��ʱ:" << (t2 - t1) / 1000 << "s." << endl;
}
bool islegal(const string& str) {
	for (string::const_iterator it = str.begin(); it != str.end(); ++it)
		if (!isalnum(*it))
			return false;
	return true;
}
bool Encrypt(Rsa& rsa, BigInt& c) {
	string str;
	do {
		cout << ">����16��������:";
		cin >> str;
	} while (cin && str.length() < 1);
	if (!cin || islegal(str) == false)
		return false;
	BigInt m(str);

	c = rsa.encryption(m);

	cout << "����:" << m << endl << "����:" << c << endl;
	return true;
}
bool Decrypt(Rsa& rsa, BigInt& m) {
	string str;
	do {
		cout << ">����16��������:";
		cin >> str;
	} while (cin && str.length() < 1);
	if (!cin || islegal(str) == false)
		return false;
	BigInt c(str);

	long t1 = clock();
	m = rsa.decryption(c);
	long t2 = clock();
	cout << "��ʱ:" << (t2 - t1) << "ms." << endl;
	cout << "����:" << c << endl << "����:" << m << endl;
	return true;
}

