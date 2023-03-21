#include"AES.h"
using namespace std;
const u_char Plaintext1[16] = { 0x00,0x01,0x00,0x01,0x01,0xa1,0x98,0xaf,0xda,0x78,0x17,0x34,0x86,0x15,0x35,0x66 };
const u_char Plaintext2[16] = { 0x32,0x43,0xf6,0xa8,0x88,0x5a,0x30,0x8d,0x31,0x31,0x98,0xa2,0xe0,0x37,0x07,0x34 };
const u_char Ciphertext1[16] = { 0x6c,0xdd,0x59,0x6b,0x8f,0x56,0x42,0xcb,0xd2,0x3b,0x47,0x98,0x1a,0x65,0x42,0x2a };
const u_char Ciphertext2[16] = { 0x39,0x25,0x84,0x1d,0x02,0xdc,0x09,0xfb,0xdc,0x11,0x85,0x97,0x19,0x6a,0x0b,0x32 };
const u_char Key1[16] = { 0x00,0x01,0x20,0x01,0x71,0x01,0x98,0xae,0xda,0x79,0x17,0x14,0x60,0x15,0x35,0x94 };
const u_char Key2[16] = { 0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c };

int bit_get(const u_char* bits, int byte) {
	return ((bits[byte / 8] & (0x80 >> (byte % 8))) != 0);
}
//设置字符序列的第byte位为num(0或1)，从0开始计算
void bit_set(u_char* bits, int byte, int num) {
	if (num == 1)
	{
		bits[byte / 8] |= (0x80 >> (byte % 8));
	}
	else {
		bits[byte / 8] &= ~(0x80 >> (byte % 8));
	}
}

int compare(u_char before[16], u_char after[16]) {
	int num = 0;
	for (int i = 0; i < 128; i++) {
		if (bit_get(before, i) != bit_get(after, i)) {
			num++;
		}
	}
	return num;
}
int main() {
	cout << "------分组1------" << endl;
	memset(Key, 0, 4 * Nk);
	memcpy(Key, Key1, 4 * Nk);
	BytetoWord(Plaintext, Plaintext1, Nb);
	cout << "明文" << endl;
	printWord(Plaintext, Nb);
	AES_Encryption(Plaintext, Ciphertext, Key);
	cout << "加密后的密文" << endl;
	printWord(Ciphertext, Nb);
	cout << "解密后的明文" << endl;
	AES_Decryption(Plaintext, Ciphertext, Key);
	printWord(Plaintext, Nb);

	cout << "------分组2------" << endl;
	memset(Key, 0, 4 * Nk);
	memcpy(Key, Key2, 4 * Nk);
	BytetoWord(Plaintext, Plaintext2, Nb);
	cout << "明文" << endl;
	printWord(Plaintext, Nb);
	AES_Encryption(Plaintext, Ciphertext, Key);
	cout << "加密后的密文" << endl;
	printWord(Ciphertext, Nb);
	cout << "解密后的明文" << endl;
	AES_Decryption(Plaintext, Ciphertext, Key);
	printWord(Plaintext, Nb);


	cout << "----------------检测雪崩效应----------------" << endl;
	cout << "----------------固定密钥，改变明文----------------" << endl;
	{
		cout << "分组1" << endl;
		memset(Key, 0, 4 * Nk);
		memcpy(Key, Key1, 4 * Nk);
		int num[16] = { 0 };
		double totnum = 0;
		for (int i = 0; i < 16; i++) {
			u_char tmp[16];
			memset(tmp, 0, 16);
			memcpy(tmp, Plaintext1, 16);
			bit_set(tmp, 8 * i, (1 - bit_get(tmp, 8 * i)));
			BytetoWord(Plaintext, tmp, Nb);
			//cout << "明文" << endl;
			//printWord(Plaintext, Nb);
			AES_Encryption(Plaintext, Ciphertext, Key);
			//cout << "加密后的密文" << endl;
			//printWord(Ciphertext, Nb);
			u_char temp[16];
			memset(tmp, 0, 16);
			WordtoByte(Ciphertext, temp, 4);
			num[i] = compare(temp, (u_char*)Ciphertext1);
			totnum += num[i];
			cout << "改变明文的第" << i * 8 << "位，密文变化" << num[i] << "位" << endl;
		}
		cout << "平均变化" << totnum / 16 << "位" << endl;
	}

	{
		cout << "分组2" << endl;
		memset(Key, 0, 4 * Nk);
		memcpy(Key, Key2, 4 * Nk);
		int num[16] = { 0 };
		double totnum = 0;
		for (int i = 0; i < 16; i++) {
			u_char tmp[16];
			memset(tmp, 0, 16);
			memcpy(tmp, Plaintext2, 16);
			bit_set(tmp, 8 * i, (1 - bit_get(tmp, 8 * i)));
			BytetoWord(Plaintext, tmp, Nb);
			//cout << "明文" << endl;
			//printWord(Plaintext, Nb);
			AES_Encryption(Plaintext, Ciphertext, Key);
			//cout << "加密后的密文" << endl;
			//printWord(Ciphertext, Nb);
			u_char temp[16];
			memset(tmp, 0, 16);
			WordtoByte(Ciphertext, temp, 4);
			num[i] = compare(temp, (u_char*)Ciphertext2);
			totnum += num[i];
			cout << "改变明文的第" << i * 8 << "位，密文变化" << num[i] << "位" << endl;
		}
		cout << "平均变化" << totnum / 16 << "位" << endl;
	}

	cout << "----------------固定明文，改变密钥----------------" << endl;
	{
		cout << "分组1" << endl;
		int num[16] = { 0 };
		double totnum = 0;
		for (int i = 0; i < 16; i++) {
			memset(Key, 0, 4 * Nk);
			memcpy(Key, Key1, 4 * Nk);
			bit_set(Key, 8 * i, (1 - bit_get(Key, 8 * i)));

			BytetoWord(Plaintext, Plaintext1, Nb);
			//cout << "明文" << endl;
			//printWord(Plaintext, Nb);
			AES_Encryption(Plaintext, Ciphertext, Key);
			//cout << "加密后的密文" << endl;
			//printWord(Ciphertext, Nb);
			u_char tmp[16];
			memset(tmp, 0, 16);
			WordtoByte(Ciphertext, tmp, 4);
			num[i] = compare(tmp, (u_char*)Ciphertext1);
			totnum += num[i];
			cout << "改变明文的第" << i * 8 << "位，密文变化" << num[i] << "位" << endl;
		}
		cout << "平均变化" << totnum / 16 << "位" << endl;
	}

	{
		cout << "分组2" << endl;
		int num[16] = { 0 };
		double totnum = 0;
		for (int i = 0; i < 16; i++) {
			memset(Key, 0, 4 * Nk);
			memcpy(Key, Key2, 4 * Nk);
			bit_set(Key, 8 * i, (1 - bit_get(Key, 8 * i)));

			BytetoWord(Plaintext, Plaintext1, Nb);
			//cout << "明文" << endl;
			//printWord(Plaintext, Nb);
			AES_Encryption(Plaintext, Ciphertext, Key);
			//cout << "加密后的密文" << endl;
			//printWord(Ciphertext, Nb);
			u_char tmp[16];
			memset(tmp, 0, 16);
			WordtoByte(Ciphertext, tmp, 4);
			num[i] = compare(tmp, (u_char*)Ciphertext1);
			totnum += num[i];
			cout << "改变明文的第" << i * 8 << "位，密文变化" << num[i] << "位" << endl;
		}
		cout << "平均变化" << totnum / 16 << "位" << endl;
	}
	system("pause");
}

