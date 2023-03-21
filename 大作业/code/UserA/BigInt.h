#ifndef BIGINT_H
#define BIGINT_H

#include<iostream>
#include <algorithm>
#include<cassert>
#include<vector>
#include<string>
using namespace std;

class BigInt {
public:
	typedef unsigned long base_t;
	typedef vector<base_t> data_t;
	static BigInt Zero;
	static BigInt One;
	static BigInt Two;
	friend class bit;
	friend class Rsa;
	friend string BigIntToString(BigInt& a);

private:
	data_t data;//�洢����
	bool isNegative;//��������

public:
	int size() { return data.size(); }
	//������ģ����
	BigInt moden(const BigInt& exp, const BigInt& p)const;
	//��չŷ������㷨��˷���Ԫ
	BigInt extendEuclid(const BigInt& m);

	//���������
	friend BigInt operator + (const BigInt& a, const BigInt& b);
	friend BigInt operator - (const BigInt& a, const BigInt& b);
	friend BigInt operator * (const BigInt& a, const BigInt& b);
	friend BigInt operator / (const BigInt& a, const BigInt& b);
	friend BigInt operator % (const BigInt& a, const BigInt& b);
	friend bool operator < (const BigInt& a, const BigInt& b);
	friend bool operator <= (const BigInt& a, const BigInt& b);
	friend bool operator == (const BigInt& a, const BigInt& b);
	friend bool operator != (const BigInt& a, const BigInt& b) { return !(a == b); }

	friend BigInt operator + (const BigInt& a, const long b) { BigInt t(b); return a + t; }
	friend BigInt operator - (const BigInt& a, const long b) { BigInt t(b); return a - t; }
	friend BigInt operator * (const BigInt& a, const long b) { BigInt t(b); return a * t; }
	friend BigInt operator / (const BigInt& a, const long b) { BigInt t(b); return a / t; }
	friend BigInt operator % (const BigInt& a, const long b) { BigInt t(b); return a % t; }
	friend bool operator < (const BigInt& a, const long b) { BigInt t(b); return a < t; }
	friend bool operator <= (const BigInt& a, const  long b) { BigInt t(b); return a <= t; }
	friend bool operator == (const BigInt& a, const long b) { BigInt t(b); return a == t; }
	friend bool operator != (const BigInt& a, const long b) { BigInt t(b); return !(a == t); };

	friend ostream& operator << (ostream& out, const BigInt& a);
	friend BigInt operator <<(const BigInt& a, const unsigned int n);
	BigInt& operator =(string s)
	{
		data.clear();
		isNegative = false;
		copyFromHexString(s);
		Eliminate();
		return *this;
	}
	BigInt& operator =(const long n)
	{
		data.clear();
		copyFromLong(n);
		return *this;
	}

	//���캯��
	BigInt() :isNegative(false) { data.push_back(0); }
	BigInt(const string& num) :data(), isNegative(false) { copyFromHexString(num); Eliminate(); }
	BigInt(const long n) :isNegative(false) { copyFromLong(n); }
	BigInt(const BigInt& a, bool isNegative) :data(a.data), isNegative(isNegative) {}

private:
	//����ֵ�ȽϺ���
	bool Less(const BigInt& a)const;
	bool LessEqual(const BigInt& a)const;
	bool Equal(const BigInt& a)const;

	//��������
	BigInt& leftShift(const unsigned int n);
	BigInt& add(const BigInt& b);
	BigInt& sub(const BigInt& b);
	static void abs_div(const BigInt& a, const BigInt& b, BigInt& quotient, BigInt& remainder);

	//���ƺ���
	void copyFromHexString(const string& s);
	char hex2Uchar(char ch);
	void copyFromLong(const long n);

	//����ǰ��0
	BigInt& Eliminate();

	class bit {
	public:
		size_t size();
		bool at(size_t i);
		bit(const BigInt& a);
	private:
		data_t _bitvec;
		size_t _size;
	};
};



#endif // !BIGINT_H
