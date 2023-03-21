#include"BigInt.h"

BigInt BigInt::Zero(0);
BigInt BigInt::One(1);
BigInt BigInt::Two(2);

BigInt BigInt::moden(const BigInt& exp, const BigInt& p)const
{
	BigInt::bit tmp(exp);
	BigInt d(1);

	//逐步运算，计算中间结果
	for (int i = tmp.size() - 1; i >= 0; --i)
	{
		d = (d * d) % p;
		if (tmp.at(i))
			d = (d * (*this)) % p;
	}
	return d;
}
BigInt BigInt::extendEuclid(const BigInt& m) {
	assert(m.isNegative == false);
	BigInt a[3], b[3], t[3];
	a[0] = 1; a[1] = 0; a[2] = m;
	b[0] = 0; b[1] = 1; b[2] = *this;
	if (b[2] == BigInt::Zero || b[2] == BigInt::One) {
		return b[2];
	}

	while (true) {
		if (b[2] == BigInt::One) {
			if (b[1].isNegative == true)
				b[1] = (b[1] % m + m) % m;
			return b[1];
		}

		BigInt q = a[2] / b[2];
		for (int i = 0; i < 3; ++i) {
			t[i] = a[i] - q * b[i];
			a[i] = b[i];
			b[i] = t[i];
		}
	}
}

BigInt operator + (const BigInt& a, const BigInt& b) {
	BigInt tmp(a);
	return tmp.add(b);
}
BigInt operator - (const BigInt& a, const BigInt& b) {
	BigInt tmp(a);
	return tmp.sub(b);
}
BigInt operator * (const BigInt& a, const BigInt& b)
{
	if (a == (BigInt::Zero) || b == (BigInt::Zero))
		return BigInt::Zero;

	const BigInt& big = a.data.size() > b.data.size() ? a : b;
	const BigInt& small = (&big) == (&a) ? b : a;
	BigInt result(0);
	BigInt::bit tmp(small);

	//根据位数左移相加
	for (int i = tmp.size() - 1; i >= 0; --i)
	{
		if (tmp.at(i))
		{
			BigInt temp(big, false);
			temp.leftShift(i);
			result.add(temp);
		}
	}
	result.isNegative = !(a.isNegative == b.isNegative);
	return result;
}
BigInt operator / (const BigInt& a, const BigInt& b) {
	assert(b != (BigInt::Zero));
	if (a.Equal(b))
		return (a.isNegative == b.isNegative) ? BigInt(1) : BigInt(-1);
	else if (a.Less(b))
		return BigInt::Zero;
	else {
		BigInt quotient, remainder;
		BigInt::abs_div(a, b, quotient, remainder);
		return quotient;
	}
}
BigInt operator % (const BigInt& a, const BigInt& b)
{
	assert(b != (BigInt::Zero));
	if (a.Equal(b))
		return BigInt::Zero;
	else if (a.LessEqual(b))
		return a;
	else {
		BigInt quotient, remainder;
		BigInt::abs_div(a, b, quotient, remainder);
		return remainder;
	}
}
bool operator < (const BigInt& a, const BigInt& b) {
	if (a.isNegative == b.isNegative) {
		if (a.isNegative == false)
			return a.Less(b);
		else
			return !(a.LessEqual(b));
	}
	else {
		if (a.isNegative == false)
			return true;
		else
			return false;
	}
}
bool operator <= (const BigInt& a, const BigInt& b)
{
	if (a.isNegative == b.isNegative) {
		if (a.isNegative == false)
			return a.LessEqual(b);
		else
			return !(a.Less(b));
	}
	else {
		if (a.isNegative == false)
			return true;
		else
			return false;
	}
}
bool operator == (const BigInt& a, const BigInt& b)
{
	return a.data == b.data && a.isNegative == b.isNegative;
}
ostream& operator << (ostream& out, const BigInt& a) {
	char hex[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	if (a.isNegative)
		out << "-";
	BigInt::base_t T = 0x0F;
	string str;
	for (BigInt::data_t::const_iterator it = a.data.begin(); it != a.data.end(); ++it) {
		BigInt::base_t ch = (*it);
		for (int j = 0; j < 8; ++j) {
			str.push_back(hex[ch & 0x0F]);
			ch = ch >> 4;
		}
	}
	reverse(str.begin(), str.end());
	out << str;
	return out;
}
BigInt operator <<(const BigInt& a, const unsigned int n) {
	BigInt tmp(a);
	return tmp.leftShift(n);
}

void BigInt::copyFromHexString(const string& s) {
	string str(s);
	//区分正负
	if (str.length() && str.at(0) == '-') {
		if (str.length() > 1)
			isNegative = true;
		str = str.substr(1);
	}

	//补齐前导0，方便计算
	int count = (8 - (str.length() % 8)) % 8;
	string temp;
	for (int i = 0; i < count; ++i) {
		temp.push_back(0);
	}
	str = temp + str;

	//string转int存储
	for (int i = 0; i < str.length(); i += 8) {
		base_t sum = 0;
		for (int j = 0; j < 8; ++j)
		{
			char ch = str[i + j];
			ch = hex2Uchar(ch);
			sum = ((sum << 4) | (ch));
		}
		data.push_back(sum);
	}
	reverse(data.begin(), data.end());
}
char BigInt::hex2Uchar(char ch) {
	char table[] = { 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f };
	if (isdigit(ch))
		ch -= '0';
	else if (islower(ch))
		ch -= 'a' - 10;
	else if (isupper(ch))
		ch -= 'A' - 10;

	return table[ch];
}
void BigInt::copyFromLong(const long n) {
	long a = n;
	if (a < 0) {
		isNegative = true;
		a = -a;
	}
	do {
		BigInt::base_t ch = (a & 0xFFFFFFFF);
		data.push_back(ch);
		a = a >> 32;
	} while (a);
}
BigInt& BigInt::Eliminate() {
	int count = 0;
	//检查不为0的元素的数量		
	for (data_t::reverse_iterator it = data.rbegin(); it != data.rend(); ++it)
		if ((*it) == 0)
			++count;
		else
			break;
	//只有零的情况保留一块
	if (count == data.size())
		--count;
	for (int i = 0; i < count; ++i)
		data.pop_back();
	return *this;
}

bool BigInt::Less(const BigInt& b)const
{
	if (data.size() == b.data.size()) {
		for (BigInt::data_t::const_reverse_iterator it = data.rbegin(), it_b = b.data.rbegin(); it != data.rend(); ++it, ++it_b) {
			if ((*it) != (*it_b)) {
				return (*it) < (*it_b);
			}
		}
		return false;
	}
	else
		return data.size() < b.data.size();
}
bool BigInt::LessEqual(const BigInt& b)const
{
	if (data.size() == b.data.size()) {
		for (BigInt::data_t::const_reverse_iterator it = data.rbegin(), it_b = b.data.rbegin(); it != data.rend(); ++it, ++it_b) {
			if ((*it) != (*it_b)) {
				return (*it) < (*it_b);
			}
		}
		//相等的情况
		return true;
	}
	else
		return data.size() < b.data.size();
}
bool BigInt::Equal(const BigInt& a)const {
	return data == a.data;
}

BigInt& BigInt::leftShift(const unsigned int n)
{
	int k = n >> 5;
	int off = n & 0x1F;
	int num = (off == 0) ? k : 1 + k;
	for (int i = 0; i < num; ++i) {
		data.push_back(0);
	}

	if (k) {
		num = (off == 0) ? 1 : 2;
		for (int i = data.size() - num; i >= k; --i)
			data[i] = data[i - k];
		for (int i = 0; i < k; ++i)
			data[i] = 0;
	}

	if (off) {
		BigInt::base_t T = 0xffffffff << (32 - off);
		BigInt::base_t ch = 0;
		for (size_t i = 0; i < data.size(); ++i) {
			BigInt::base_t t = data[i];
			data[i] = (t << off) | ch;
			ch = (t & T) >> (32 - off);
		}
	}

	Eliminate();
	return *this;
}
BigInt& BigInt::add(const BigInt& b) {
	//同号的情况
	if (isNegative == b.isNegative) {
		//如果大整数b位数更多，需要补上前导0
		BigInt::data_t& res = data;
		int len = b.data.size() - data.size();
		while ((len--) > 0)
			res.push_back(0);

		//记录进位
		int cn = 0;
		for (size_t i = 0; i < b.data.size(); ++i) {
			BigInt::base_t temp = res[i];
			res[i] = res[i] + b.data[i] + cn;
			//计算新一轮进位
			cn = temp > res[i] ? 1 : temp > (temp + b.data[i]) ? 1 : 0;
		}

		//大整数b位数少的情况
		for (size_t i = b.data.size(); i < data.size() && cn != 0; ++i) {
			BigInt::base_t temp = res[i];
			res[i] = (res[i] + cn);
			cn = temp > res[i];
		}

		if (cn != 0) {
			res.push_back(cn);
		}
		//消除前导0的块
		Eliminate();
	}
	else {
		//异号的情况
		bool tmp;
		if (Less(b))
			tmp = b.isNegative;
		else if (Equal(b))
			tmp = false;
		else {
			tmp = this->isNegative;
		}

		this->isNegative = b.isNegative;
		sub(b);
		this->isNegative = tmp;
	}
	//返回计算结果
	return *this;
}
BigInt& BigInt::sub(const BigInt& b) {
	//同号的情况
	if (b.isNegative == isNegative) {
		BigInt::data_t& res = data;

		//绝对值不小于b
		if (!(Less(b))) {
			//记录借位
			int cn = 0;
			for (size_t i = 0; i < b.data.size(); ++i)
			{
				BigInt::base_t temp = res[i];
				res[i] = (res[i] - b.data[i] - cn);
				//计算新一轮借位
				cn = temp < res[i] ? 1 : temp < b.data[i] ? 1 : 0;
			}

			for (size_t i = b.data.size(); i < data.size() && cn != 0; ++i)
			{
				BigInt::base_t temp = res[i];
				res[i] = res[i] - cn;
				cn = temp < cn;
			}
			Eliminate();
		}
		//绝对值小于b
		else {
			data = (b - (*this)).data;
			isNegative = !isNegative;
		}
	}

	//异号的情况
	else {
		bool tmp = this->isNegative;
		this->isNegative = b.isNegative;
		add(b);
		this->isNegative = tmp;
	}

	//返回计算结果
	return *this;
}
void BigInt::abs_div(const BigInt& a, const BigInt& b, BigInt& quotient, BigInt& remainder) {
	BigInt abs_b(b, false);
	remainder.isNegative = false;
	remainder.data = a.data;
	quotient = BigInt::Zero;
	BigInt::bit bit_b(abs_b);

	while (true) {
		BigInt::bit bit_a(remainder);
		int len = bit_a.size() - bit_b.size();
		BigInt temp;

		while (len >= 0)
		{
			temp = abs_b << len;
			if (temp.LessEqual(remainder)) {
				break;
			}
			len--;
		}

		//退出循环条件
		if (len < 0) {
			break;
		}

		BigInt::base_t n = 0;
		while (temp.LessEqual(remainder))
		{
			remainder.sub(temp);
			n++;
		}
		BigInt result(n);
		if (len) {
			result.leftShift(len);
		}
		quotient.add(result);
	}
	quotient.Eliminate();
}

size_t BigInt::bit::size() {
	return _size;
}
bool BigInt::bit::at(size_t i) {
	size_t index = i >> 5;
	size_t off = i & 0x1F;
	return (_bitvec[index] & (1 << off));
}
BigInt::bit::bit(const BigInt& a)
{
	_bitvec = a.data;
	BigInt::base_t tmp = _bitvec[_bitvec.size() - 1];
	_size = _bitvec.size() << 5;
	BigInt::base_t t = 1 << 31;

	if (a == 0) {
		_size -= 32;
	}
	else {
		while (!(tmp & t)) {
			--_size;
			t = t >> 1;
		}
	}
}


string BigIntToString(BigInt& a) {
	char hex[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	BigInt::base_t T = 0x0F;
	string str;
	if (a.isNegative)
		str.push_back('-');
	for (BigInt::data_t::const_iterator it = a.data.begin(); it != a.data.end(); ++it) {
		BigInt::base_t ch = (*it);
		for (int j = 0; j < 8; ++j) {
			str.push_back(hex[ch & 0x0F]);
			ch = ch >> 4;
		}
	}
	reverse(str.begin(), str.end());
	return str;
}